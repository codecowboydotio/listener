# listener
This is a simple listener that will print the IP address of the remote computer and the time.
You run this externally on a server, and connect to it from your inside network.
While services like "what's your IP" are good - not all traffic is HTTP based.

Simply run the listener, and telnet to the port that it's running on.


I wrote this in response to working on firewalls and needing to know the external address of a server before and after NAT changes.

The typical use case for this is to run on an external host on the internet, for NAT and firewall testing.

This version is a daemon version of the code.
This is so that the code can run as a daemon. 
The other branch is no a deamon.

 - Major changes so far are removal of log file.
 - Syslog capability.

TO DO: 
------------
1) Signal handling.

2) OS Fingerprinting (may be a while off).

3) More verbose logging.

4) General code cleanup.

5) Makefile
