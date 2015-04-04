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
 * Reads the "conf/config.chord" file and parses out the chord network information.
 * INPUT: reference variables to store 
 * 	m     : the number of peers/nodes
 * 	n     : the size of the chord
 * 	nodes : the list of nodes extracted from the file 
 * ACTION: parses out the info and stores them in these variables 	
 */
void readConfiguration(int &n, int &m, vector<Node> &nodes){
  //try to read config file
  ifstream configfile;
  configfile.open("conf/config.chord", ios::in);
  if(!configfile.good()){
    cout << "Config file does not exist!!" << endl;  
    cout << "Rename config.chord.test to config.chord for a sample chord file." << endl;
    exit(1);
  }

  cout << "Config file found. Loading configuration." << endl;

  //parse the file
  string line;
  stringstream ss;
  string ip;
  int port;

  int chordLength;

  string commentPrefix = ";";
  string param_chordSize = "chordSize";
  string param_numPeers = "numPeers";

  while(getline(configfile, line)){

    // commented out lines 
    if(line.compare(0, commentPrefix.length(), commentPrefix) == 0){
      continue;
    }

    if(line.compare(0, param_chordSize.length(), param_chordSize) == 0){
      line.erase(0, param_chordSize.length());   
      ss << line;
      ss >> n;
      ss.clear();
      cout << "Extracted n: " << n << endl;
      chordLength = pow(2, n);
    }

    if(line.compare(0, param_numPeers.length(), param_numPeers) == 0){
      line.erase(0, param_numPeers.length());   
      ss << line;
      ss >> m;
      ss.clear();
      cout << "Extracted m: " << m << endl;


      //also read the next m lines for the nodes
      for(int i = 0;i < m; i++){
	if(!getline(configfile, line)){
	  //incomplete config file
	  cout << "Bad config file!!" << endl;
	  exit(1);
        }

        //create the node and add it to the list 
        ss << line;
	ss >> ip;
	ss >> port;
	ss.clear();

	cout << "Reads " << ip << " : " << port << endl;

	Node newnode(ip, port);
	newnode.setID(hashNode(ip,port));
	newnode.setSimpleId(chordLength);
        nodes.push_back(newnode);

      }
      break;
    }
  }

  configfile.close();
}

/*
 * Contact a suspected member of the ring, to get complete info about the chord
 * Info that we would otherwise have got from a config file.
 * The contact would have to take care that the new guy fits with the network.
 * INPUT: self : self node
 *        ip   : ipaddr of suspect  
 *        port : port of the same 
 *        m    : number of peers including self  
 *        n    : size of the chord
 *        nodes: the list if all nodes including self
 */
void startupFromExisting(Node &self, string ip, int port, int &n, int &m, vector<Node> &nodes){
  //eshtablish a connection with the contact
  //send a join request to the contact
  cout << "WIP" << endl;

  //our contact in the ring
  Node contact(ip, port);
  int csockfd;
  struct addrinfo* caddrInfo;

  //open our communication channel to our contact
  initSocketClientToNode(contact, csockfd, caddrInfo);
  //TODO: if target does not exist print some message and exit

  //send a REQ_JOIN message
  Comm joinreq;
  joinreq.type = REQ_JOIN;
  joinreq.src = self.getPort();
  strcpy(joinreq.ipaddr, self.getIp().c_str());
  
  //send a normal request
  sendComm(csockfd, caddrInfo, joinreq);

  //now switch to recieving the other format
  ChordMeta joinrep;  

  //first get conformation/ rejection
  recvChordMeta(csockfd, joinrep);

  if(joinrep.type == JOIN_REJECT){
    cout << "Request rejected." << endl; 
    cout << "COMMENT: " << joinrep.comment << endl;
    exit(1);
  }
  else if(joinrep.type == JOIN_ACCEPT){
    cout << "Application accepted." << endl;
    cout << "COMMENT: " << joinrep.comment << endl;

    //recieve for further details
    recvChordMeta(csockfd, joinrep);
    if(joinrep.type != JOIN_CHORDSIZE){
      cout << "Error in communication of CHORDSIZE." << endl;
      exit(1);
    }
    cout << "Recieved chord size aka n: " << joinrep.payload << endl;
    self.setN(joinrep.payload);

    int chordLength = pow(2, self.getN());
    // set the simple id
    self.setSimpleId(chordLength);

    recvChordMeta(csockfd, joinrep);
    if(joinrep.type != JOIN_NONODES){
      cout << "Error in communication of NONODES." << endl;
      exit(1);
    }
    cout << "Recieved no of nodes aka m: " << joinrep.payload << endl;
    self.setM(joinrep.payload);

    
  }
  else{
    cout << "Error in response." << endl;
    exit(1);
  }
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
