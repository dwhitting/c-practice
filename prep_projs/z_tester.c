#include "wrap_prox.h"

int main(void) 
{

    char url[MAXLINE] = "http://www.google.com:80/index.html";
    char hostname[MAXLINE];
    char target_port[MAXLINE];
    char path_outbound[MAXLINE];

    parse_url(url, hostname, target_port, path_outbound);

    printf("host: %s, port %s, path: %s\n", hostname, target_port, path_outbound);


    return 0;
}