/* https://github.com/scottorama1/listener */

#include <fcntl.h>
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
#include <signal.h>
#include <stdbool.h>

#define DAEMON_NAME "listener_daemon"

/* declare interrupt handler */
void     INThandler(int);

/* declare global variable - bad practice but easier than struct passing */
bool logswitch=true;
const char * version = "1.2";
unsigned short portNumber;

int main(int argc, char *argv[]) {

    int listenfd = 0, connfd = 0, rc;
    struct sockaddr_in serv_addr;
    struct sockaddr_in their_addr;
    socklen_t addr_size;
    char sendBuff[1025];
    time_t ticks;


    signal(SIGINT, INThandler);
    signal(SIGHUP, INThandler);
    signal(SIGUSR2, INThandler);
    signal(SIGQUIT, INThandler);

    //Set our Logging Mask and open the Log
    setlogmask(LOG_UPTO(LOG_NOTICE));
    openlog(DAEMON_NAME, LOG_CONS | LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_USER);


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

    rc = bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (rc < 0 )
    {
	syslog (LOG_NOTICE, "Cannot bind to port %d ", portNumber);
        exit(-1);
    }

    listen(listenfd, 10);

    pid_t pid, sid;

   //Fork the Parent Process
    pid = fork();

    if (pid < 0) { exit(EXIT_FAILURE); }

    //We got a good pid, Close the Parent Process
    if (pid > 0) { exit(EXIT_SUCCESS); }

    //Change File Mask
    umask(0);

    //Create a new Signature Id for our child
    sid = setsid();
    if (sid < 0) { exit(EXIT_FAILURE); }

    //Change Directory
    //If we cant find the directory we exit with failure.
    //Should probably not run in / as this is somewhat unsafe.
    if ((chdir("/")) < 0) { exit(EXIT_FAILURE); }

    //Close Standard File Descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    //----------------
    //Main Process
    //----------------

    syslog (LOG_NOTICE, "Current Version: %s\n", version);
    syslog (LOG_NOTICE, "Starting up...");
    syslog (LOG_NOTICE, "Listening on port %i\n",portNumber);

    while(1){
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

        /* Write connection attempts to syslog*/
	
	if ( logswitch == true)
	{
        	syslog (LOG_NOTICE, "Connection from %s\n",inet_ntoa(their_addr.sin_addr));
        }
	close(connfd);
    }

    //Close the log
    closelog ();
}

void  INThandler(int sig)
{
	
	/* Signals to control behaviour.
	   SIGINT 1 = Toggle logging of client information to syslog.
	   SIGHUP 2 = Print out current settings of all variables. This includes versions.
	   SIGQUIT 3 = quit the process.
	   SIGUSR2 12 = I'm alive. Signal to be used by monitoring system to generate syslog event.
	*/

	signal(1, SIG_IGN);
	signal(2, SIG_IGN);
	signal(3, SIG_IGN);
	signal(12, SIG_IGN);
	if (sig == 2)
	{
		syslog (LOG_NOTICE, "Settings Dump");
		syslog (LOG_NOTICE, "Version: %s", version);
		syslog (LOG_NOTICE, "Listening on port: %i", portNumber);
		syslog (LOG_NOTICE, "Current PID is: %d", getpid());
		syslog (LOG_NOTICE, "logswitch: setting is currently set to %d", logswitch);
	}
	if (sig == 1)
	{
	/* If we receive a HUP we want to make sure that we just toggle the value of logswitch variable.
	   This will mean that we either turn off logging to syslog if it's on, or if it's not we turn it on */

		if (logswitch == 1 )
		{
			logswitch = 0;
		}
		else
		{
			logswitch = 1;
		}
	}
	/* This may seem a little pointless here because I'm just doing a KILL anyway.
	   The point here is that in the future I may be actively writing to log files or databases
	   and may need a way to gracefully shut down. For now a SIGKILL will do */
	if (sig == 3)
	{
		syslog (LOG_NOTICE, "Exiting.....");
		kill (getpid (), SIGKILL);
	}
	/* If SIGUSR2 then just print message to syslog. Primarily used for monitoring applications so we have a way of
	   determining that we are still running */
	if (sig == 12)
	{
		syslog (LOG_NOTICE, "Process is still alive");
	}

	/* Reinstall the handler before exiting */
	signal(SIGINT, INThandler); 
	signal(SIGHUP, INThandler); 
	signal(SIGUSR2, INThandler); 
}
