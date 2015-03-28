#Chord

###Organization
There are programs:
* Chord server/client : which does the distributed indexing and searching
* p2p\_server : a server running waiting for incoming TCP requests to transfer files
* p2p\_client : a client to connect to a specific peer to download files

They could be modules in a single program too. There are 4 main functionality.

* Share a file - which means hash it and send the index to the correct machine
* Search a file - which means search the above distributed index
* Offer the file for download
* Download a file from a machine


###The chord network connections

This was the key behind the entire network. How many sockets and what does each socket do?

My understanding now is as follows:
A node (every node) creates a sockfd to which it binds. It then creates a succSockFd and a predSockFd to act as clients to the successor and predecessor nodes. So it sends messages via the latter sockets and uses it's own socket only to recieve messages.
