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

#include "foreground.h"

#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

/*
 * Handle sharing. Needs to take inputs
 * DEPRECATED : use handleShare2 for taking chord route instead of direct 
 */
void handleShare(int &chordLength, Node &self, int &succSockFd, struct addrinfo* &succAddrInfo){
  string fileName;
  cout << "Enter file name: "; 
  cin >> fileName;

  if(fileName.length() >= MAXFILENAME){
    cout << "Filename too long. " << endl;
    return;
  }

  //hash the string
  identifier fh = hashfunc(fileName.c_str(), fileName.length());
  //mod it to this chord network
  int filehash = fh % chordLength;

  cout << "Hashing file: " << fileName << " to " << filehash << endl;

  //check if the file needs to be added to this very node
  if((filehash > self.getPredecessor()->getSimpleId()) && (filehash <= self.getSimpleId())){
    //index the file here itself
    self.addToIndex(fileName, self.getIp());
    cout << "Indexed" << endl;
    return;
  }

  //else compose a message and send it on
  Comm mess;
  mess.type = REQ_SHARE;
  mess.src = self.getSimpleId();

  bzero(mess.filename, MAXFILENAME);
  strcpy(mess.filename, fileName.c_str());

  bzero(mess.ipaddr, MAXIPLEN);
  strcpy(mess.ipaddr, self.getIp().c_str());
  
  mess.filehash = filehash;

  sendComm(succSockFd, succAddrInfo, mess);
  cout << "Sent to successor" << endl;

  return;
}

/*
 * Handle sharing in the new chord network. Needs to take inputs
 * Passes along message using chord protocol
 */
void handleShare2(int &chordLength, Node &self){
  string fileName;
  cout << "Enter file name: "; 
  cin >> fileName;

  if(fileName.length() >= MAXFILENAME){
    cout << "Filename too long. " << endl;
    return;
  }

  //hash the string
  identifier fh = hashfunc(fileName.c_str(), fileName.length());
  //mod it to this chord network
  int filehash = fh % chordLength;

  cout << "Hashing file: " << fileName << " to " << filehash << endl;

  //check if the file needs to be added to this very node
  if((filehash > self.getPredecessor()->getSimpleId()) && (filehash <= self.getSimpleId())){
    //index the file here itself
    self.addToIndex(fileName, self.getIp());
    cout << "Indexed" << endl;
    return;
  }

  //else compose a message and send it on
  Comm mess;
  mess.type = REQ_SHARE;
  mess.src = self.getSimpleId();

  bzero(mess.filename, MAXFILENAME);
  strcpy(mess.filename, fileName.c_str());

  bzero(mess.ipaddr, MAXIPLEN);
  strcpy(mess.ipaddr, self.getIp().c_str());
  
  mess.filehash = filehash;

  NodeClientSocket ncs = self.getNodeSocketFor(filehash);

  cout << "Finger table in fg:" << endl;
  self.printFingertable();

  sendCommStruct(ncs, mess);
  cout << "Sent to next node" << endl;

  return;
}

/*
 * Handle searching. Needs to take inputs
 * DEPRECATED : use handleSearch2 for taking chord route instead of direct 
 */
void handleSearch(int &chordLength, Node &self, int &succSockFd, struct addrinfo* &succAddrInfo){
  string fileName;
  cout << "Enter file name: "; 
  cin >> fileName;

  if(fileName.length() >= MAXFILENAME){
    cout << "Filename too long. " << endl;
    return;
  }

  //hash the string
  identifier fh = hashfunc(fileName.c_str(), fileName.length());
  //mod it to this chord network
  int filehash = fh % chordLength;

  cout << "Hashing file: " << fileName << " to " << filehash << endl;

  //check if the file has been hashed in this very node
  if((filehash > self.getPredecessor()->getSimpleId()) && (filehash <= self.getSimpleId())){
    string ip = self.getFromIndex(fileName);

    cout << "File is in IP: " << ip << endl;
  }
  
  //else pass it on to the successor
  Comm mess;
  mess.type = REQ_SEARCH;
  mess.src = self.getSimpleId();

  bzero(mess.filename, MAXFILENAME);
  strcpy(mess.filename, fileName.c_str());
  cout << mess.filename << endl;

  mess.filehash = filehash;

  sendComm(succSockFd, succAddrInfo, mess); 
}

/*
 * Handle searching in the chord network. Needs to take inputs
 * Passes along message using chord protocol
 */
void handleSearch2(int &chordLength, Node &self){
  string fileName;
  cout << "Enter file name: "; 
  cin >> fileName;

  if(fileName.length() >= MAXFILENAME){
    cout << "Filename too long. " << endl;
    return;
  }

  //hash the string
  identifier fh = hashfunc(fileName.c_str(), fileName.length());
  //mod it to this chord network
  int filehash = fh % chordLength;

  cout << "Hashing file: " << fileName << " to " << filehash << endl;

  //check if the file has been hashed in this very node
  if((filehash > self.getPredecessor()->getSimpleId()) && (filehash <= self.getSimpleId())){
    string ip = self.getFromIndex(fileName);

    cout << "File is in IP: " << ip << endl;
  }
  
  //else pass it on to the successor
  Comm mess;
  mess.type = REQ_SEARCH;
  mess.src = self.getSimpleId();

  cout << "Preparing search request:  REQ_SEARCH " << mess.src << endl;

  bzero(mess.filename, MAXFILENAME);
  strcpy(mess.filename, fileName.c_str());
  cout << mess.filename << endl;

  mess.filehash = filehash;

  NodeClientSocket ncs = self.getNodeSocketFor(filehash);
  sendCommStruct(ncs, mess);
}

/*
 * Main user terminal
 * INPUT: the chord length and the current node
 */
void manageNodeTerminal(int &chordLength, Node &self){
  cout << "Node user interface work." << endl;

  int choice;
  int result;
  fd_set read_fds;

  while(1){
    //safety value; to prevent endless loop
    choice = 0;  

    cout << "***Main Menu***" << endl;
    cout << "1. Search for file" << endl;
    cout << "2. Share a file" << endl;
    cout << "3. Quit" << endl;
    cout << "Enter choice: " << endl;

    
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds);
    result = select(STDIN_FILENO + 1, &read_fds, NULL, NULL, NULL);
    if(result == -1){
      cout << "Error" << endl;
      continue;
    }

    if(FD_ISSET(STDIN_FILENO, &read_fds)){
      //cin >> choice;
      scanf("%d", &choice);
      switch(choice){
	case 1: {
		  //handleSearch(chordLength, self, succSockFd, succAddrInfo);
		  handleSearch2(chordLength, self);
		  break;
		}		
	case 2: {
		  //handleShare(chordLength, self, succSockFd, succAddrInfo);
		  handleShare2(chordLength, self);
		  break;
		}		
	case 3: {
		  return;
		}		
	default: cout << " Invalid option." << endl;
      }
    }
  }

  return;
}
