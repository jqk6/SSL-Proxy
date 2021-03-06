#ifndef __IOSOCKET_H
#define __IOSOCKET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <time.h>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <list>
#include <sstream>
#include "IOSelect.h"
#include<stdio.h>

using namespace std;

#define TRACE_CALL() do{cerr << __FILE__ << "::" << __FUNCTION__ << " called" << endl;}while(0)

/**
 * Buffer size for network I/O
 */
#define IOSOCKET_NET_BUF_SIZE	4096

/**
 * Max number of retry operation on EINTR
 */
#define IOSOCKET_MAX_RETRY		3

/**
 * Socket type
 */
enum socket_type {
	IOSOCKET_LISTEN_T,
	IOSOCKET_CONNECT_T
};
typedef socket_type socket_type;

/**
 * Opaque handler for network
 * transfert. Just buffer content
 * is transfered, not the whole struct
 */
struct io_buf {
	char 	*content;
	size_t	length;

	io_buf() {
		TRACE_CALL();
		content = new char[IOSOCKET_NET_BUF_SIZE];
		length = 0;
	}

	~io_buf() {
		TRACE_CALL();
		delete content;
	}
};

extern "C" {
	/**
	 * Struct for keeping
	 * track of connection statistics
	 */
	struct io_connect_stat {
		time_t		startTime;
		time_t		endTime;
		long long	bytesSent;
		long long	bytesReceived;
	};

	/**
	 * Struct for keeping
	 * track of listen sockets stats
	 */
	struct io_listen_stat {
		long long	accepted;
	};

	/**
	 * Union keeping stats depending on
	 * socket type
	 */
	union io_stat {
		struct io_connect_stat 	client;
		struct io_listen_stat	server;
	};
	typedef io_stat io_stat;
};

/**
 * @desc This class aims to provide easy access to network function
 * and to provide a socket factory for server functionnalities
 */
class IOSocket {

	/**
	 * Attributes
	 */
	protected:
		int					sock;
		int					port;
		char				*host;
		bool				connected;
		socket_type			socket_t;

		/* Remote */
		int			remote_port;
		char		remote_addr[16];

	public:
		/* For stats */
		io_stat		stats;

	/**
	 * Methods
	 */
	protected:
		virtual void connectToServer(const char *host, const int &port);
		virtual void bindSocket(const int &port);
		IOSocket(const int &socket);

	public:
		IOSocket(const socket_type sock_t, const char *host, const int port);
		virtual IOSocket *accept();
		virtual ~IOSocket();

		/* Getter */
		virtual int getFd(void);

		/* I/O */
		virtual size_t write(const struct io_buf &buffer);
		virtual size_t write(const char *msg);
		virtual size_t read(struct io_buf *buffer);

		virtual void close();
};

#endif
