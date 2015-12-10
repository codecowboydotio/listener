# listener
This is a simple listener that will print the IP address of the remote computer and the time.
You run this externally on a server, and connect to it from your inside network.
While services like "what's your IP" are good - not all traffic is HTTP based.

Simply run the listener, and telnet to the port that it's running on.


I wrote this in response to working on firewalls and needing to know the external address of a server before and after NAT changes.

TODO:

1) Change the logfile name to be a command line argument (i.e. configurable).

2) Add option for proper syslog logging.
