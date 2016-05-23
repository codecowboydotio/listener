# listener
This is a simple listener that will print the IP address of the remote computer and the time.
You run this externally on a server, and connect to it from your inside network.
While services like "what's your IP" are good - not all traffic is HTTP based.

Simply run the listener, and telnet to the port that it's running on.


I wrote this in response to working on firewalls and needing to know the external address of a server before and after NAT changes.

The typical use case for this is to run on an external host on the internet, for NAT and firewall testing.

This version is a daemon version of the code.


RUN TIME:
----------------
The program takes two arguments

```
./listener -p <port>

./listener -c

```
-p <port> = The port number to run the listener on.

-c = run in console mode. 

By default the program will run as a daemon, if you would like the program to run in the foreground - for example, in a docker container - then the -c argument can be used.
It should be noted that when in console mode, a <ctrl>-<c> key combination will send a SIGHUP which will not quit the program. See signal section below.


SIGNAL HANDLING:
-----------------
If you send the code the following signals you will get the behaviour described below.

SIGINT 1 = Toggle logging of client information to syslog.

SIGHUP 2 = Print out current settings of all variables. This includes versions.

SIGQUIT 3 = Write a final entry to syslog and shut down.

SIGUSR2 12 = I'm alive. Signal to be used by monitoring system to generate syslog event.

```
[root@host listener]# kill -2 6564

[root@host listener]# tail /var/log/messages

May 11 19:38:15 host listener_daemon[6564]: Settings Dump
May 11 19:38:15 host listener_daemon[6564]: Version: 1.2
May 11 19:38:15 host listener_daemon[6491]: Listening on port 4444
May 11 19:38:15 host listener_daemon[6564]: logswitch: setting is currently set to 0





[root@host listener]# kill -12 6564

[root@host listener]# tail /var/log/messages

May 11 19:38:26 host listener_daemon[6564]: Process is still alive

```


BUILD:
-----------
To build the executable, you just do a make.

There is a makefile, but no configure script.

A configure script seemed a little heavy handed for such a small implementation.

By default, will install in /usr/local/listener/bin/

...perhaps a configure script with PREFIX would be helpful after all? :)



TO DO: 
------------
1) OS Fingerprinting (may be a while off).

2) More verbose logging.

3) General code cleanup.

4) Configure script

5) Error handling and checking around socket initialization
