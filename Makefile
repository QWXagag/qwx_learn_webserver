server:
	g++ util.cpp client.cpp Buffer.cpp -o client && \
	g++ util.cpp server.cpp Epoll.cpp InetAddress.cpp Socket.cpp Channel.cpp EventLoop.cpp Server.cpp Acceptor.cpp Connection.cpp Buffer.cpp Thread.cpp -o server -pthread

clean:
	rm server && rm client