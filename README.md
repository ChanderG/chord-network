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
