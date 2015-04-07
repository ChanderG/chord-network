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

#include "background.h"

#include "communication.h"
#include "helper.h"

#include <iostream>
#include <cstring>
#include <sys/select.h>
#include <cmath>
using namespace std;

/* Handle an incomming share request 
 * DEPRECATED in favour of handleReqShare2
 */
void handleReqShare(Comm &mess, Node &self, int &succSockFd, struct addrinfo* &succAddrInfo, int &predSockFd, struct addrinfo* &predAddrInfo ){
  Comm messrep;
  //made a full loop => not available anywhere
  if(mess.src == self.getSimpleId()){
    cout << "Error in chord network." << endl;
    //pass the message back
    bzero(messrep.ipaddr,MAXIPLEN); 

    messrep.type = REP_SHARE;
    messrep.src = mess.src;
    strcpy(messrep.ipaddr, "Error. File not indexed." );
    sendComm(predSockFd, predAddrInfo, messrep);
  }  
  //check if the file has been indexed in this node
  else if((mess.filehash > self.getPredecessor()->getSimpleId()) && (mess.filehash <= self.getSimpleId())){
    self.addToIndex(string(mess.filename), string(mess.ipaddr));
    cout << "File " << mess.filename << " indexed. " << endl;
    //send a reply
    bzero(messrep.comment,256); 

    messrep.type = REP_SHARE;
    messrep.src = mess.src;
    strcpy(messrep.comment, "File successfully indexed." );
    sendComm(predSockFd, predAddrInfo, messrep);
  }
  else{
    //not in this node
    //forward the message
    sendComm(succSockFd, succAddrInfo, mess);
  }
}

/* Handle an incomming share request adapted to chord network
 */
void handleReqShare2(Comm &mess, Node &self){
  Comm messrep;
  NodeClientSocket ncs;
  //made a full loop => not available anywhere
  if(mess.src == self.getSimpleId()){
    cout << "Error in chord network." << endl;
    //pass the message back
    
    bzero(messrep.ipaddr,MAXIPLEN); 

    messrep.type = REP_SHARE;
    messrep.src = mess.src;
    strcpy(messrep.ipaddr, "Error. File not indexed." );
    ncs = self.getNodeSocketFor(mess.src);
    sendCommStruct(ncs, messrep);
  }  
  //check if the file has been indexed in this node
  else if((mess.filehash > self.getPredecessor()->getSimpleId()) && (mess.filehash <= self.getSimpleId())){
    self.addToIndex(string(mess.filename), string(mess.ipaddr));
    cout << "File " << mess.filename << " indexed. " << endl;
    //send a reply
    bzero(messrep.comment,256); 

    messrep.type = REP_SHARE;
    messrep.src = mess.src;
    strcpy(messrep.comment, "File successfully indexed." );
    ncs = self.getNodeSocketFor(mess.src);
    sendCommStruct(ncs, messrep);
  }
  else{
    //not in this node
    //forward the message
    ncs = self.getNodeSocketFor(mess.filehash);
    sendCommStruct( ncs, mess);
  }
}


/* Handle an incomming search request 
 * DEPRECATED in favour of handleReqSearch2
 */
void handleReqSearch(Comm &mess, Node &self, int &succSockFd, struct addrinfo* &succAddrInfo, int &predSockFd, struct addrinfo* &predAddrInfo ){
  //made a full loop => not available anywhere
  Comm messrep;
  if(mess.src == self.getSimpleId()){
    cout << "File not in chord network." << endl;
    //pass the message back
    bzero(messrep.ipaddr,MAXIPLEN); 

    messrep.type = REP_SEARCH;
    messrep.src = mess.src;
    strcpy(messrep.ipaddr, "NOT FOUND" );
    sendComm(predSockFd, predAddrInfo, messrep);
  }  
  //check if the file needs to be added to this node
  else if((mess.filehash > self.getPredecessor()->getSimpleId()) && (mess.filehash <= self.getSimpleId())){
    string ip = self.getFromIndex(string(mess.filename));
    cout << "IP with required file " << mess.filename << " is " << ip << endl;
    //send a reply
    bzero(messrep.ipaddr,MAXIPLEN); 

    messrep.type = REP_SEARCH;
    messrep.src = mess.src;
    strcpy(messrep.ipaddr, ip.c_str());
    sendComm(predSockFd, predAddrInfo, messrep);
  }
  else{
    //not in this node
    //forward the message
    sendComm(succSockFd, succAddrInfo, mess);
  }
}

/* Handle an incomming search request adapted to chord network
 */
void handleReqSearch2(Comm &mess, Node &self){
  //made a full loop => not available anywhere
  Comm messrep;
  NodeClientSocket ncs;
  if(mess.src == self.getSimpleId()){
    cout << "File not in chord network." << endl;
    //pass the message back
    bzero(messrep.ipaddr,MAXIPLEN); 

    messrep.type = REP_SEARCH;
    messrep.src = mess.src;
    strcpy(messrep.ipaddr, "NOT FOUND" );
    ncs = self.getNodeSocketFor(mess.src);
    sendCommStruct(ncs, messrep);
  }  
  //check if the file needs to be added to this node
  else if((mess.filehash > self.getPredecessor()->getSimpleId()) && (mess.filehash <= self.getSimpleId())){
    string ip = self.getFromIndex(string(mess.filename));
    cout << "IP with required file " << mess.filename << " is " << ip << endl;
    //send a reply
    bzero(messrep.ipaddr,MAXIPLEN); 

    messrep.type = REP_SEARCH;
    messrep.src = mess.src;

    cout << "Preparing message: REP_SHARE to " << mess.src << endl; 

    strcpy(messrep.ipaddr, ip.c_str());
    ncs = self.getNodeSocketFor(mess.src);
    sendCommStruct(ncs, messrep);
  }
  else{
    //not in this node
    //forward the message
    ncs = self.getNodeSocketFor(mess.filehash);
    sendCommStruct( ncs, mess);
  }
}


/* Handle an incomming share reply 
 * DEPRECATED in favour of handleRepShare2
 */
void handleRepShare(Comm &mess, Node &self, int &succSockFd, struct addrinfo* &succAddrInfo, int &predSockFd, struct addrinfo* &predAddrInfo ){
  // this message is meant for us
  if(mess.src == self.getSimpleId()){
    cout << mess.comment << endl;
  }  
  else{
    //simply pass it along
    //the pred addr
    sendComm(predSockFd, predAddrInfo, mess);
  }
}

/* Handle an incomming share reply adapted to chord network
 */
void handleRepShare2(Comm &mess, Node &self){
  // this message is meant for us
  if(mess.src == self.getSimpleId()){
    cout << mess.comment << endl;
  }  
  else{
    //simply pass it along
    //the pred addr
    NodeClientSocket ncs;
    ncs = self.getNodeSocketFor(mess.src);
    sendCommStruct(ncs, mess);
  }
}


/* Handle an incomming search reply 
 * DEPRECATED in favour of handleRepSearch2
 */
void handleRepSearch(Comm &mess, Node &self, int &succSockFd, struct addrinfo* &succAddrInfo, int &predSockFd, struct addrinfo* &predAddrInfo ){
  // this message is meant for us
  if(mess.src == self.getSimpleId()){
    cout << "File found in : " << mess.ipaddr << endl;
  }  
  else{
    //simply pass it along
    //the pred addr
    sendComm(predSockFd, predAddrInfo, mess);
  }
}

/* Handle an incomming search reply adapted to chord network
 */
void handleRepSearch2(Comm &mess, Node &self){
  // this message is meant for us
  if(mess.src == self.getSimpleId()){
    cout << "File found in : " << mess.ipaddr << endl;
  }  
  else{
    //simply pass it along
    //the pred addr
    NodeClientSocket ncs;
    ncs = self.getNodeSocketFor(mess.src);
    sendCommStruct(ncs, mess);
  }
}

/*
 * Handle request from a new node for joining
 * We need to vet the candidate completely then send it the required information.
 */ 
void handleReqJoin(Comm &mess, Node &self, struct sockaddr_in &saddr, int &sockfd){
  identifier id = hashNode(mess.ipaddr, mess.src);
  int chordLength = pow(2, self.getN());
  int simpleid = id % chordLength;

  ChordMeta repl;

  for(vector<Node>::iterator it = self.nodes.begin(); it != self.nodes.end(); it++){
    if(it->getSimpleId() == simpleid){
      //we have a problem
      //send a reject message right back
      //and exit here
      repl.type = JOIN_REJECT;
      bzero(repl.comment, 256);
      strcpy(repl.comment, "Node with same hash value exists in network.");
      sendChordMeta(sockfd, saddr, repl);
    }
  }

  //create a new node to represent this newcomer
  Node newnode(mess.ipaddr, mess.src);
  newnode.setID(id);
  newnode.setSimpleId(chordLength);

  cout << "New node accepted with values: " << endl;
  cout << mess.ipaddr << " : " << mess.src << " " << newnode.getSimpleId() << endl;

  //success
  //increment m and add this node to our list
  //send a success token first
  //followed by n, m and all m machines
  // and then send this info to all peers who can simply add to their m and nodes
  // whithout any extra checking
  
  //sending success token
  repl.type = JOIN_ACCEPT;
  bzero(repl.comment, 256);
  strcpy(repl.comment, "Wait for further instructions.");
  sendChordMeta(sockfd, saddr, repl);

  //now send n 
  repl.type = JOIN_CHORDSIZE;
  repl.payload = self.getN();
  sendChordMeta(sockfd, saddr, repl);

  //now increment m
  self.incM();
  // and send it
  repl.type = JOIN_NONODES;
  repl.payload = self.getM();
  sendChordMeta(sockfd, saddr, repl);

  //add it to your nodes set
  self.nodes.push_back(newnode);
  //send the entire nodes to the newcomer
  repl.type = JOIN_NODEINFO;
  for(int i = 0;i < self.getM();i++){
    bzero(repl.ipaddr, MAXIPLEN);
    strcpy(repl.ipaddr, self.nodes[i].getIp().c_str());
    repl.port = self.nodes[i].getPort();
    sendChordMeta(sockfd, saddr, repl);
  }
  //it creates its node vector and retriggers
  
  //send the new node to all others, so that they do the same
  //they can simply inc m and retrigger changes
  // except for self, SHOULD not send again
  Comm alert;
  alert.type = CTRL_JOIN;
  alert.src = mess.src;
  bzero(alert.ipaddr, MAXIPLEN);
  strcpy(alert.ipaddr, mess.ipaddr);
  for(vector<Node>::iterator it = self.nodes.begin();it != self.nodes.end();it++){
    //need to send to all nodes
    if(it->getSimpleId() != self.getSimpleId()){
      NodeClientSocket ncs;
      initSocketClientToNode(*it, ncs.sockfd, ncs.addrInfo);
      sendCommStruct(ncs, alert);
      close(ncs.sockfd);
    }
  }

  //retrigger your sorting etc and reinit fingertable 
  self.reinit();

  //then the existing file index transfer has to be done
  //TODO
}

/*
 * Handle control packet from a peer informing of an accepted new node.
 */ 
void handleCtrlJoin(Comm &mess, Node &self){
  identifier id = hashNode(mess.ipaddr, mess.src);
  int chordLength = pow(2, self.getN());

  //create a new node to represent this newcomer
  Node newnode(mess.ipaddr, mess.src);
  newnode.setID(id);
  newnode.setSimpleId(chordLength);

  cout << "New node accepted with values: " << endl;
  cout << mess.ipaddr << " : " << mess.src << " " << newnode.getSimpleId() << endl;

  //now increment m
  self.incM();

  //add it to your nodes set
  self.nodes.push_back(newnode);
  //retrigger your sorting etc and reinit fingertable 
  self.reinit();
}

/*
 * Handle request for files from newly joined node.
 * Has to be our predecessor
 */
void handleFileDistJoinReq(Comm &mess, Node &self, struct sockaddr_in &saddr, int &sockfd){
  ChordMeta repl;

  //check of it is really our predecessor
  if(mess.src != self.getPredecessor()->getSimpleId()){
    cout << "Wrong node contacted for file distribution." << endl;

    repl.type = JOIN_DIST_REJECT;
    sendChordMeta(sockfd, saddr, repl);
  }

  repl.type = JOIN_DIST_SUCCESS;
  sendChordMeta(sockfd, saddr, repl);

}

/*
 * Main function
 * INPUT: the chord length and the current node
 */
void manageChord(int &chordLength, Node &self, int &sockfd){
  cout << "Chord maintainance work." << endl;

  Comm mess;
  struct sockaddr_in saddr;
  while(1){
    recvCommFrom(sockfd, mess, saddr);
    //recvComm(sockfd, mess);
    
    switch(mess.type){
      case REQ_SHARE: {
			handleReqShare2(mess, self);
			break;
		      }
      case REQ_SEARCH: {
			handleReqSearch2(mess,self);
			break;
		      }
      case REP_SHARE: {
			handleRepShare2(mess,self);
			break;
		      }
      case REP_SEARCH: {
			handleRepSearch2(mess,self);
			break;
		      }
      case REQ_JOIN: {
		       //a new node sends to one of the members
		       //who responds will all required info to setup
		       //assuming that everything is OK aka the hash value etc
		       //and then tells all old members this info
		       //adds the node to it's own list
		       //and triggers recalculation of fingettable
		       handleReqJoin(mess, self, saddr, sockfd);
		       break;
		     }
      case CTRL_JOIN: {
			//a peer wants to inform us about a new node
			// contains IP and port, add to your node list
			// retrigger fingertable calculation
			handleCtrlJoin(mess, self);
			break;
		      }
      case REQ_DIST_JOIN: {
		       //its predecessor requests for the file index
		       handleFileDistJoinReq(mess, self, saddr, sockfd);
		     }
      default: break;		      
    }
  }

}
