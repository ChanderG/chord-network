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
#include <sys/select.h>
using namespace std;

/*
 * Handles all connections from behind. Namely all REQuests.
 */
void handleConnectionsFromPred(int &chordLength, Node &self, int &predSockFd, struct addrinfo* &predAddrInfo, int &succSockFd, struct addrinfo* &succAddrInfo){

  Comm mess;
  recvComm(predSockFd, predAddrInfo, mess);
  if(mess.type == REQ_SEARCH){
    //check if the file needs to be added to this node
    if((mess.filehash > self.getPredecessor()->getSimpleId()) && (mess.filehash <= self.getSimpleId())){
      //index the file here itself
      self.addToIndex(string(mess.ipaddr), string(mess.filename));

      cout << "File indexed. " << endl;
      exit(1);

      //send a reply
    }
    else{
      //forward the message
      sendComm(succSockFd, succAddrInfo, mess);
    }

  }
  else if(mess.type == REQ_SHARE){

  }
  else{
    //defn an error
    //handle it
  }
}

/*
 * Handles all connections from ahead. Namely all REPlys.
 */
void handleConnectionsFromSucc(int &chordLength, Node &self, int &predSockFd, struct addrinfo* &predAddrInfo, int &succSockFd, struct addrinfo* &succAddrInfo){
}

/*
 * Main function
 * INPUT: the chord length and the current node
 */
void manageChord(int &chordLength, Node &self, int &predSockFd, struct addrinfo* &predAddrInfo, int &succSockFd, struct addrinfo* &succAddrInfo){
  cout << "Chord maintainance work." << endl;

  //ready to select one of 2 sockets
  fd_set nset;
  FD_ZERO(&nset);

  FD_SET(predSockFd, &nset);
  FD_SET(succSockFd, &nset);

  int maxfd;
  if(predSockFd > succSockFd){
    maxfd = predSockFd;
  }
  else{
    maxfd = succSockFd;
  }
  
  int result;

  while(1){
    result = select(maxfd+1, &nset, NULL, NULL, NULL);
    if(-1 == result){
      cout << "Error in select. " << endl;
      exit(1);
    }
    else{
      if(FD_ISSET(predSockFd, &nset)){
	handleConnectionsFromPred(chordLength, self, predSockFd, predAddrInfo, succSockFd, succAddrInfo);
      }

      if(FD_ISSET(succSockFd, &nset)){
	handleConnectionsFromSucc(chordLength, self, predSockFd, predAddrInfo, succSockFd, succAddrInfo);
      }
    }
  }
}
