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

#include <iostream>
#include <cstring>
#include <sys/select.h>
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
void handleReqJoin(Comm &mess, Node &self, struct sockaddr_in &saddr, vector<Node> &nodes){
  //WIP    
}

/*
 * Main function
 * INPUT: the chord length and the current node
 */
void manageChord(int &chordLength, Node &self, int &sockfd, int &succSockFd, struct addrinfo* &succAddrInfo, int &predSockFd, struct addrinfo* &predAddrInfo, vector<Node> &nodes){
  cout << "Chord maintainance work." << endl;

  Comm mess;
  struct sockaddr_in saddr;
  while(1){
    //recvComm(sockfd, mess);
    recvCommFrom(sockfd, mess, saddr);
    
    switch(mess.type){
      case REQ_SHARE: {
			//handleReqShare(mess, self,succSockFd, succAddrInfo, predSockFd, predAddrInfo);
			handleReqShare2(mess, self);
			break;
		      }
      case REQ_SEARCH: {
			//handleReqSearch(mess,self, succSockFd, succAddrInfo, predSockFd, predAddrInfo);
			handleReqSearch2(mess,self);
			break;
		      }
      case REP_SHARE: {
			//handleRepShare(mess,self, succSockFd, succAddrInfo, predSockFd, predAddrInfo);
			handleRepShare2(mess,self);
			break;
		      }
      case REP_SEARCH: {
			//handleRepSearch(mess,self, succSockFd, succAddrInfo, predSockFd, predAddrInfo);
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
		       handleReqJoin(mess, self, saddr, nodes);
		       break;
		     }
      case CTRL_JOIN: {
			//a peer wants to inform us about a new node
			// contains IP and port, add to your node list
			// retrigger fingertable calculation
			break;
		      }
      default: break;		      
    }
  }

}
