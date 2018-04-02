all:server client

server:
	gcc -o server Server.c encryption.c dbutil.c  mappers.c -I "/usr/local/include/json-c" -I "/usr/local/mysql-5.7.16-osx10.11-x86_64/include" -L "/usr/local/mysql/lib" -lmysqlclient -lz -ljson-c -l mcrypt
client: 
	gcc -o client encryption.c Client.c -l mcrypt
clean:
	rm -f server client