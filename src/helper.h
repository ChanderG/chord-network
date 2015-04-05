/*
 * Copyright (c) 2015 Govindarajan, Chander <chandergovind@gmail.com>
 * Author: Govindarajan, Chander <chandergovind@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

//Place for misc helper functions.

#include "node.h"

#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h> 

using namespace std;

//to get IP address of the current machine
//OUTPUT: the IP address as a string
string getIP(void);

/*
 * INPUT: ip and port
 * OUTPUT: the hashed value, assuming a "ip:port" string system
 */
identifier hashNode(string ip, int port);


/*
 * Setup the predecessor and successor links
 * Also checks of the node is in the config file.
 * INPUT: 
 * 	self  : reference to main node
 * 	nodes : list of nodes in SORTED order
 */ 	
void setupPredAndSucc(Node &self, vector<Node> &nodes); 

/*
 * Bind a UDP self socket for all incoming connections.
 * INPUT: sockfd : for listening for incoming connections    
 */
void initSocketSelfServer(Node &self, int &sockfd);

/*
 * Create a client socket to a destination Node.
 * To be used for sending messages to say succ, pred
 * INPUT: destNode :  the destination node
 *        sockFd   :  the socket where we will send messages    		
 *        addrInfo :  standard addrinfo structure for the same
 */
void initSocketClientToNode(Node &destNode, int &sockFd, struct addrinfo* &addrInfo);

/*
 * Close both sockets.
 */
void closeNormalSockets(int &predSockFd, int &succSockFd);
