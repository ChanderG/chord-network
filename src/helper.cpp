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

#include "helper.h"
#include "communication.h"

#include "hash.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <cmath>

using namespace std;

//to get IP address of the current machine
//OUTPUT: the IP address as a string
std::string getIP(void){
  std::string ip = "127.0.0.1";
  return ip;
}

/*
 * INPUT: ip and port
 * OUTPUT: the hashed value, assuming a "ip:port" string system
 */
identifier hashNode(string ip, int port){
  string ipport;
  ipport = ip + string(":") + to_string(port);
  //cout << "Identifier string is " << ipport << endl;

  identifier id = hashfunc(ipport.c_str(), ipport.length());

  return id;
}


/*
 * Setup the predecessor and successor links
 * Also checks of the node is in the config file.
 * INPUT: 
 * 	self  : reference to main node
 * 	nodes : list of nodes in SORTED order
 */ 	
void setupPredAndSucc(Node &self, vector<Node> &nodes){
  bool presence = false;   //to see if self has an entry in the config file

  //setup predecessor and successor nodes
  for (unsigned int i = 0; i < nodes.size(); i++) {
    if(nodes[i].getSimpleId() == self.getSimpleId()){
      presence = true;
      self.setPredecessor(&nodes[(nodes.size()-1 + i)%nodes.size()]);
      self.setSuccessor(&nodes[(i+1)%nodes.size()]);
      break;
    }
  }

  if(presence == false){
    //not there
    cout << "This node is not there in the config file. " << endl;
    cout << "Update the file and try again. " << endl;
    exit(0);
  }

  //testing predecessor
  cout << "Predecessor : " << self.getPredecessor()->getIp() << ": " << self.getPredecessor()->getPort() << " " << self.getPredecessor()->getSimpleId() << endl;
}

/*
 * Bind a UDP self socket for all incoming connections.
 * INPUT: sockFd : for listening for incoming connections    
 */
void initSocketSelfServer(Node &self, int &sockfd){

  //for self (server-like for everyone)
  struct sockaddr_in sa;

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if(-1 == sockfd){
    perror("socket");
    exit(1);
  }

  sa.sin_family = AF_INET;
  sa.sin_port = htons(self.getPort());
  sa.sin_addr.s_addr = INADDR_ANY;   //(self.getIp()).c_str();  
  if(-1 == bind(sockfd, (struct sockaddr *)&sa, sizeof(sa))){
    perror("bind");
    exit(1);
  }
}

/*
 * Create a client socket to a destination Node.
 * To be used for sending messages to say succ, pred
 * INPUT: destNode :  the destination node
 *        sockFd   :  the socket where we will send messages    		
 *        addrInfo :  standard addrinfo structure for the same
 */
void initSocketClientToNode(Node &destNode, int &sockFd, struct addrinfo* &addrInfo){

  //for input destination node
  struct addrinfo hints, *servinfo;

  bzero(&hints, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  
  if(0 != getaddrinfo((destNode.getIp()).c_str(), to_string(destNode.getPort()).c_str(), &hints, &servinfo)){
    perror("getaddrinfo");
    exit(1);
  }

  for(addrInfo = servinfo; addrInfo!= NULL; addrInfo = addrInfo->ai_next){
    if(-1 == (sockFd = socket(addrInfo->ai_family, addrInfo->ai_socktype, addrInfo->ai_protocol))){
      perror("socket");
      continue;
    }
    break;
  }

  if(addrInfo == NULL){
    cout << "Failed to create socket to succecessor. Check if the node is up" << endl;
    exit(1);
  }
  freeaddrinfo(servinfo);
}

/*
 * Close both sockets.
 */
void closeSockets(int &predSockFd, int &succSockFd){

  close(predSockFd);
  close(succSockFd);
  cout << "All sockets closed." << endl;
}
