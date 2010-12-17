#ifndef SOCKET_H_INCLUDED
#define SOCKET_H_INCLUDED

#include <stdio.h>
#include <iostream>
#include <winsock2.h>
using namespace std;

class Socket
{
	private:
		SOCKET m_socket;
	public:
		Socket() {;};
		bool socketConnect();
		void envia(char *s, int len);
		void recebe(char *ans);
		void quit();
};

#endif
