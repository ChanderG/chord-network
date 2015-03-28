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


/* Handle an incomming search request 
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


/* Handle an incomming share reply 
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


/* Handle an incomming search reply 
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


/*
 * Main function
 * INPUT: the chord length and the current node
 */
void manageChord(int &chordLength, Node &self, int &sockfd, int &succSockFd, struct addrinfo* &succAddrInfo, int &predSockFd, struct addrinfo* &predAddrInfo){
  cout << "Chord maintainance work." << endl;

  Comm mess;
  while(1){
    recvComm(sockfd, mess);
    
    switch(mess.type){
      case REQ_SHARE: {
			handleReqShare(mess, self,succSockFd, succAddrInfo, predSockFd, predAddrInfo);
			break;
		      }
      case REQ_SEARCH: {
			handleReqSearch(mess,self, succSockFd, succAddrInfo, predSockFd, predAddrInfo);
			break;
		      }
      case REP_SHARE: {
			handleRepShare(mess,self, succSockFd, succAddrInfo, predSockFd, predAddrInfo);
			break;
		      }
      case REP_SEARCH: {
			handleRepSearch(mess,self, succSockFd, succAddrInfo, predSockFd, predAddrInfo);
			break;
		      }
      default: break;		      
    }
  }

}
