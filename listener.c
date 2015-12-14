#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <syslog.h>
#include "version.h"

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0, size=0;
    struct sockaddr_in serv_addr;
    struct sockaddr_in their_addr;
    socklen_t addr_size;
    unsigned short portNumber;

    char sendBuff[1025];
    time_t ticks;

    FILE *file;
    struct stat st;
    
    const char * version = "1.2";

/* Set up syslog mask */
    setlogmask (LOG_UPTO (LOG_NOTICE));
    openlog ("exampleprog", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);

        if(argc != 2) {
        fprintf(stderr,"Usage: %s <Port Number>\n", argv[0]);
        exit(1);
    }

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));

        portNumber = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(portNumber);

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(listenfd, 10);

    fprintf(stderr, "Current Version: %s\n", version);


    while(1)
    {
        stat("logfile.log", &st);
        size = st.st_size;

        printf("File size:   %lld bytes\n", (long long) st.st_size);
        file = fopen("logfile.log", "a+");



        addr_size = sizeof their_addr;
        connfd = accept(listenfd, (struct sockaddr *)&their_addr, &addr_size);

        ticks = time(NULL);
        snprintf(sendBuff, sizeof(sendBuff), "The Current Time is: ");
        write(connfd, sendBuff, strlen(sendBuff));
        snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
        write(connfd, sendBuff, strlen(sendBuff));

        snprintf(sendBuff, sizeof(sendBuff), "Your IP Address is: ");
        write(connfd, sendBuff, strlen(sendBuff));
        snprintf(sendBuff, sizeof(sendBuff), "%s\n", inet_ntoa(their_addr.sin_addr));
        write(connfd, sendBuff, strlen(sendBuff));

/* Write connection attempts to log file*/
        fprintf(file, "%.24s\t", ctime(&ticks));
        fprintf(file,"%s\n",inet_ntoa(their_addr.sin_addr));
/* Write connection attempts to syslog*/
	syslog (LOG_NOTICE, "Connection from %s\n",inet_ntoa(their_addr.sin_addr));
	closelog ();

        close(connfd);
        fclose(file);
        sleep(1);
     }
}
