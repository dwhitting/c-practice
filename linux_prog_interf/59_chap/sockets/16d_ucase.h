#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ctype.h>
#include "../../headers/tlpi_hdr.h"

#define BUF_SIZE 10         //max size of messages exchanged between client and server

#define PORT_NUM 50002      // server port number