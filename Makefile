all:
	g++ server.cpp -pthread -l sqlite3 -o server
	g++ `pkg-config --cflags gtk+-3.0` client.cpp -o client -lm `pkg-config --libs gtk+-3.0` -export-dynamic
clean:
	rm -f *~ client server
