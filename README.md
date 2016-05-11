# listener
This is a simple listener that will print the IP address of the remote computer and the time.
You run this externally on a server, and connect to it from your inside network.
While services like "what's your IP" are good - not all traffic is HTTP based.

Simply run the listener, and telnet to the port that it's running on.


I wrote this in response to working on firewalls and needing to know the external address of a server before and after NAT changes.

The typical use case for this is to run on an external host on the internet, for NAT and firewall testing.

This version is a daemon version of the code.


SIGNAL HANDLING:
-----------------
If you send the code the following signals you will get the behaviour described below.

SIGINT 1 = Toggle logging of client information to syslog.
SIGHUP 2 = Print out current settings of all variables. This includes versions.
SIGUSR2 12 = I'm alive. Signal to be used by monitoring system to generate syslog event.


TO DO: 
------------
1) OS Fingerprinting (may be a while off).

2) More verbose logging.

3) General code cleanup.

4) Configure script
