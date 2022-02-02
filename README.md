# testsock2
This is a simple test of the Sock2 class, the class I use in htmlttyd
The class has 3 connection type - 2 byte len, 4 byte len and a websocket protocol

Sock2 s;
Sock2 c;

s.open(5001); // serves port 5001
c.open(s);    // accepts a waiting connection

c.DoHandShake() // establishes the protocol

At this point the object c - c.get c.put and c.getwait looks identical to the programmer

DoHandShake, sockwait and put_data were made public so I could add a 4th connection type of HTML
to serve up the Single Page App htmltty that does a websocket connection back to htmlttyd

to build ./m0 - this should build on linux and windows.

the server executable does a single connection test and quits
SO:
./server 5001
will start the test server - it will test all 4 type

The HTML is tested by bring up a browser and going to page 127.0.0.1:5001

The websock is tested by bring up client.html

The 2 and 4 byte len are tested with the client executable (modify client.cpp to change len).
Note that the Sock2 class handles the client side of these 2 connections.
