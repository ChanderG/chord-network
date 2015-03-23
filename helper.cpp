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

#include "hash.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <cstdlib>
#include <cstring>

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
void readConfiguration(int &n, int &m, list<Node> &nodes){
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
        nodes.push_back(newnode);

      }
      break;
    }
  }

  configfile.close();
}

/*
 * Setup the predecessor and successor links
 * Also checks of the node is in the config file.
 * INPUT: 
 * 	self  : reference to main node
 * 	nodes : list of nodes in SORTED order
 */ 	
void setupPredAndSucc(Node &self, list<Node> &nodes){
  bool presence = false;   //to see if self has an entry in the config file
 
  //setup predecessor and successor nodes
  for(list<Node>::iterator it = nodes.begin(); it != nodes.end();it++){
    if(it->getSimpleId() == self.getSimpleId()){
      presence = true;
      if(it == nodes.begin()){
	self.setPredecessor(&*prev(end(nodes)));
	self.setSuccessor(&*next(it));
      }
      else if(it == prev(nodes.end())){
	self.setPredecessor(&*prev(it));
	self.setSuccessor(&*begin(nodes));
      }
      else{
	self.setPredecessor(&*prev(it));
	self.setSuccessor(&*next(it));
      }
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
 * Init UDP sockets to both predecessor and successor.
 * Bind a self socket for incoming predecessor communication and a 
 * client like connection to successor
 * INPUT: sockFd, succSockFd     : respective socket descriptors for self, succ
 * 	  succAddrInfo           : succ socket address info 
 */	  
void initSockets(Node &self, int &sockfd, int &succSockFd,struct addrinfo* &succAddrInfo){

  cout << "Initializing sockets." << endl;

  //for self (server-like for pred)
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

  //for successor
  struct addrinfo hints, *servinfo;

  bzero(&hints, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  
  if(0 != getaddrinfo((self.getSuccessor()->getIp()).c_str(), to_string(self.getSuccessor()->getPort()).c_str(), &hints, &servinfo)){
    perror("getaddrinfo");
    exit(1);
  }

  for(succAddrInfo = servinfo; succAddrInfo!= NULL; succAddrInfo = succAddrInfo->ai_next){
    if(-1 == (succSockFd = socket(succAddrInfo->ai_family, succAddrInfo->ai_socktype, succAddrInfo->ai_protocol))){
      perror("socket");
      continue;
    }
    break;
  }

  if(succAddrInfo == NULL){
    cout << "Failed to create socket to succecessor. Check if the node is up" << endl;
    exit(1);
  }

  freeaddrinfo(servinfo);
  //sockets on both sides up and running
}

/*
 * Close both sockets.
 */
void closeSockets(int &predSockFd, int &succSockFd){

  close(predSockFd);
  close(succSockFd);
  cout << "All sockets closed." << endl;
}

