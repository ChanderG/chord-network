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

/*
 * For all communication structures and helper functions.
 */

#include "communication.h"

#include <cstdlib>
#include <cstdio>

/*
 * Wrapper to send a message via socket.
 * INPUT: the socket fd, the socket addrinfo and the message structure 
 */
void sendComm(int &sockfd, struct addrinfo* &p, Comm &msg){
  if(-1 == sendto(sockfd, &msg, sizeof(msg), 0, p->ai_addr, p->ai_addrlen)){
    perror("Problems in connecting to peer: ");
    exit(0);
  }
}

/*
 * Wrapper to recv a message via socket.
 * INPUT: the socket fd, the socket addrinfo and the message structure 
 */
void recvComm(int &sockfd, Comm &msg){
  if(-1 == recvfrom(sockfd, &msg, sizeof(msg), 0, NULL, NULL)){
    perror("recvfrom");  
    exit(1);
  }
}

/*
 * Wrapper over sendComm function for ease
 * INPUT:  NodeClientSocket struct
 *         Comm message        
 */        
void sendCommStruct(struct NodeClientSocket &ncs, Comm &msg){
  sendComm(ncs.sockfd, ncs.addrInfo, msg);
}

/*
 * Wrapper to recieve message along with the sender info
 */
void recvCommFrom(int &sockfd, Comm &msg, struct sockaddr_in &sendera){
  int sizeof_sendera = sizeof(struct sockaddr);
  if(-1 == recvfrom(sockfd, &msg, sizeof(msg), 0, (struct sockaddr *)&sendera, (socklen_t*)&sizeof_sendera)){
    perror("recvfrom");  
    exit(1);
  }
}

/*
 * Wrapper to send a chord meta via socket.
 * INPUT: the socket fd, the socket addrinfo and the message structure 
 */
void sendChordMeta(int &sockfd, struct sockaddr_in &sa, ChordMeta &msg){
  int sizeof_sa = sizeof(sa);
  if(-1 == sendto(sockfd, &msg, sizeof(msg), 0, (struct sockaddr*)&sa, (socklen_t)sizeof_sa )){
    perror("Problems in connecting to new node: ");
    exit(0);
  }
}

/*
 * Wrapper to recv a chord meta via socket.
 * INPUT: the socket fd and the message structure 
 */
void recvChordMeta(int &sockfd, ChordMeta &msg){
  if(-1 == recvfrom(sockfd, &msg, sizeof(msg), 0, NULL, NULL)){
    perror("recvfrom");  
    exit(1);
  }
}
