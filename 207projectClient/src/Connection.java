/**
 * Created by lam on 4/9/17.
 */
public class Connection {
    private static Connection ourInstance = new Connection();
    private static OnMessageReceivedListener mListener;

    public interface OnMessageReceivedListener {
        void onMessageReceived(String message);
    }

    public static void setListener(OnMessageReceivedListener listener) {
        mListener = listener;
    }

    public static void removeListener() {
        mListener = null;
    }

    public static Connection getInstance() {
        return ourInstance;
    }

    static {
        System.loadLibrary("clientJNI");
    }

    private Connection() {
    }

    public void getMessageCallback(String message) {
        System.out.print(message);
        if (mListener != null) {
            mListener.onMessageReceived(message);
        }
    }

    public native void connectHost();

    public native void sendMessage(String st);

}
