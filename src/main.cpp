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

#include "node.h"
#include "hash.h"
#include "helper.h"
#include "background.h"
#include "foreground.h"
#include "help.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <thread>

using namespace std;

/* The main module represnting a peer.
 * INPUT: port number, list of all machines on the Chord network
 * ACTION: setup this node, connect to next and prev nodes, interact with user to share/search for files
 */
int main(int argc, char* argv[]){

  if(argc == 1){
    printHelpPrompt();
    return 0;
  }

  if(strcmp(argv[1], "--help") == 0){
    printHelpMessage();
    return 0;
  }

  string ip;
  int port;

  //extract the input from args
  stringstream ss;
  ss << argv[1];
  ss >> port;
  ss.clear();

  ip = getIP();
  cout << "IP is : " << ip << endl;
  cout << "Reading port as : " << port << endl;
  if(port <= 0 || port > 65535){
    cout << "Specified port is invalid. Try again." << endl;
    return 0;
  }

  Node self(ip,port);

  identifier id = hashNode(ip, port);
  cout << "Identifier hash is " << id << endl;
  self.setID(id);

  int n;  // the chord size 
  int m;  // the number of nodes in the network 
  vector<Node> nodes;  //the m nodes 

  //if extra flags were present
  if(argc > 2){
    if(strcmp(argv[2], "-j") == 0){
      //parse the extra info
      // not available
      if(argc == 4){
	cout << "Invalid input." << endl;
	printHelpPrompt();
	return 0;
      }

      //all good to go
      string dip(argv[3]);
      int dport;

      ss << argv[4];
      ss >> dport;
      ss.clear();

      cout << "Target IP recogonized as: " << dip << endl;
      cout << "Target port recogonized as: " << dport << endl; 
      if(dport <= 0 || dport > 65535){
	cout << "Specified port is invalid. Try again." << endl;
	return 0;
      }

      startupFromExisting(self, dip, dport, n, m, nodes);
      return 0;
    }
    else{
      cout << "Invalid option: " << argv[2] << endl;
      printHelpPrompt();
      return 0;
    }
  }
  else{
    // simple use case
    readConfiguration(n, m, nodes);
  }

  //we have n, m and nodes in hand
  self.setN(n);
  self.setM(m);

  int chordLength = pow(2, n);

  self.setSimpleId(chordLength);
  cout << "Looping the nodes around the chord." << endl;
  for(vector<Node>::iterator it = nodes.begin(); it != nodes.end();it++){
    it->setSimpleId(chordLength); 
  }

  sort(nodes.begin(), nodes.end());   //sort according to simpleId

  for(vector<Node>::iterator it = nodes.begin(); it != nodes.end();it++){
    cout << it->getAddress() <<  " " << it->getID() << " " << it->getSimpleId() << endl;
  }

  int org = nodes.size();
  //reduce to unique elements; according to simpleId
  unique(nodes.begin(), nodes.end());   
  int fin = nodes.size();
  //if duplicates in simpleId exist, the chordLength or the hash func is not good enough
  if(org != fin){
    cout << "ERROR: 2 nodes seem to have the same simpleId. This is illegal." << endl;
    cout << "Increace the chordLength/chordSize and try again." << endl;
    exit(0);
  }

  //nodes are ready
  self.nodes = nodes;

  self.setupFingerTable(nodes, n, chordLength);

  //setup the sockets
  int sockfd;

  initSocketSelfServer(self, sockfd);

  thread background(manageChord, chordLength, self, sockfd);
  thread foreground(manageNodeTerminal, chordLength, self);  

  foreground.join();
  cout << "Shutting down node terminal." << endl;
  background.join();

  self.closeSockets(sockfd);

  return 0;

}
