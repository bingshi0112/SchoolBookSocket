#include <jni.h>
#include <stdio.h>
#include "Client.h"

static jmethodID midStr;
static char * sigStr = "(Ljava/lang/String;)V";
static jobject smObject;
static JNIEnv smEnv;

/*
* Call back to Java
*/
static void getMessageCallback(JNIEnv * env, jobject o, char * message) {
  midStr = (*env)->GetMethodID(env, (*env)->GetObjectClass(env, o), "getMessageCallback", sigStr);
  jstring string = (*env)->NewStringUTF(env, message);
  (*env)->CallVoidMethod(env, o, midStr, string);
}

/*
* Call back From Client
*/
static void getMessageFromClient(const char * mess) {
    getMessageCallback(smEnv, smObject, mess);
}

/*
* Connect to host
*/
JNIEXPORT void JNICALL Java_Connection_connectHost
  (JNIEnv * env, jobject jobj)
{
    smEnv = env;
    smObject = jobj;
    void (*mess) (char *) = getMessageFromClient;
    connectHost("localhost", "9201", mess);
}

/*
* Send message
*/
JNIEXPORT void JNICALL Java_Connection_sendMessage
  (JNIEnv * env, jobject jobj, jstring string) {
     const char *nativeString = (*env)->GetStringUTFChars(env, string, 0);

    setInput(nativeString);
}