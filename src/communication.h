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
#include "node.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h> 

/*
 * Identifier of class of message.
 */
enum comm_type { REQ_SEARCH, REQ_SHARE, REP_SEARCH, REP_SHARE, REQ_JOIN, CTRL_JOIN };

/*
 * Identifier of meta/control packet.
 */
enum chordmeta_type  { JOIN_ACCEPT, JOIN_REJECT, JOIN_CHORDSIZE, JOIN_NONODES, JOIN_NODEINFO };

/*
 * Some constants for eshtablishing uniformity in communictaions.
 */
#define MAXFILENAME 256
#define MAXIPLEN 16

/*
 * Main communication data type - to store info
 * Has Meta info for up and down traversal in the chord network
 * Fields are used in different ways for different types.
 *
 * type : REQ_SEARCH
 * src : node which requests file
 * filename : name of file requested
 * filehash : hash of file modded for this chord network
 *
 * type : REP_SEARCH
 * src : node which wants the file
 * ipaddr : ip of machine with the required file
 *
 * type : REQ_SHARE
 * src : node that is sharing / has the file
 * ipaddr : ip addr of node with the file
 * filename : name of file being shared
 * filehash : hash of file modded for this chord network
 *
 * type : REP_SHARE
 * src : orig node; where the reply must be sent to
 * comment : any extra comments; say file indexing success
 *
 * type : REQ_JOIN
 * src : port of machine that wants to join
 * ipaddr : ip address of machine that wants to join
 *
 * type : CTRL_JOIN
 * src : port of machine that wants to join
 * ipaddr : ip address of machine that wants to join
 */
struct Comm{
  comm_type type;
  int src;
  char ipaddr[MAXIPLEN];
  char filename[MAXFILENAME];
  int filehash;
  char comment[256];
};

/*
 * Meta communication type.
 * Used when new nodes want to join.
 * The type field represents the type of control packet.
 * For maintinaing simplicity, a new node first makes contact using the REQ_JOIN
 * then subequent dealings are done with this ChordMeta type packets
 *
 * type: JOIN_REJECT
 */
struct ChordMeta{
  chordmeta_type type;
  int payload;
  char ipaddr[MAXIPLEN];
  int port;
  char comment[256];
};


/*
 * Wrapper to send a message via socket.
 * INPUT: the socket fd, the socket addrinfo and the message structure 
 */
void sendComm(int &sockfd, struct addrinfo* &p, Comm &msg);

/*
 * Wrapper to recv a message via socket.
 * INPUT: the socket fd and the message structure 
 */
void recvComm(int &sockfd, Comm &msg);

/*
 * Wrapper over sendComm function for ease
 * INPUT:  NodeClientSocket struct
 *         Comm message        
 */        
void sendCommStruct(struct NodeClientSocket &ncs, Comm &msg);

/*
 * Wrapper to recieve message along with the sender info
 */
void recvCommFrom(int &sockfd, Comm &msg, struct sockaddr_in &sendera);

/*
 * Wrapper to send a chord meta via socket.
 * INPUT: the socket fd, the socket addrinfo and the message structure 
 */
void sendChordMeta(int &sockfd, struct sockaddr_in &sa, ChordMeta &msg);

/*
 * Wrapper to recv a chord meta via socket.
 * INPUT: the socket fd and the message structure 
 */
void recvChordMeta(int &sockfd, ChordMeta &msg);
