//clientSocket.h 
// Marcoux Tristan Avril 2024 
#ifndef clientSocket
#define clientSocket 

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
Class clientSocket{
private  :
const char* ip;   
int port;
public : 
bool envoyerMessage(const char* msg); 
clientSocket(const char ipSrv , int portsrv)
}

#endif 