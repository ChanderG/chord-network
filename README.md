#Chord

## Part 1: Simple Chord network

Systems can connect only to neighbours on either side.

###Organization
There are 4 main functionalities.

* Share a file - which means hash it and send the index to the correct machine
* Search a file - which means search the above distributed index
* Offer the file for download
* Download a file from a machine

The later 2 are TCP based and quite trivial. The main code here is the chord network itself.

###The chord network connections

This was the key behind the entire network. How many sockets and what does each socket do?

My understanding now is as follows:
A node (every node) creates a sockfd to which it binds. It then creates a succSockFd and a predSockFd to act as clients to the successor and predecessor nodes. So it sends messages via the latter sockets and uses it's own socket only to recieve messages.

Though a node could theoretically reply via it's bound socket, that is not being done here for simplicity.


### Simulation

The perl script simulator.pl spins up the same program on different terminal simultaneously by using a different port on each instance all controllable from the conf/config.chord file. Each IP:Port in the file is considered a different virtual machine. Use with a tiling window manager for best effects. 

## Part 2: Finger tables and dynamic nodes 

For the dynamic nodes, the changes in plan are as follows. The program will now have 2 different ways of starting. If called without a flag, the program reads the config file and goes ahead. If called with a flag, we need to input an ip:port pair that is a valid member of an existing chord network. Then that nodes after checking everything initiates the newcomer into the circle.

As of now, the network has to be booted with a config file.

####Known Issues
* If 2 nodes apply to join at the same time, they both may be entered with the same id which will lead to unpredictable performance. In real life, the probabilities are low for this kind of thing to happen.

###ISSUES
* Only some connections seem to work. Need to test exhaustively.


###CHANGELOG

*v1.6*
Complete chord network in terms of connnections. Uses fingertable to lookup and send incomming requests and replies to the right node.

*v1.3*
Adds changelog.

*v1.2*
Updates list to vector internally.
No breaking funcionality changes.

*v1.0*
First working version of simple chord network.
