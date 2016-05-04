# listener
This is a simple listener that will print the IP address of the remote computer and the time.
You run this externally on a server, and connect to it from your inside network.
While services like "what's your IP" are good - not all traffic is HTTP based.

Simply run the listener, and telnet to the port that it's running on.


I wrote this in response to working on firewalls and needing to know the external address of a server before and after NAT changes.

The typical use case for this is to run on an external host on the internet, for NAT and firewall testing.

This version is a daemon version of the code.

I've pushed the daemon branch back to main and will be working off main for a while.

 - Major changes so far are removal of log file.
 - Syslog capability.

TO DO: 
------------
1) Signal handling.

2) Logging levels via signals.

3) OS Fingerprinting (may be a while off).

4) More verbose logging.

5) General code cleanup.

6) Configure script
