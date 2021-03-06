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
 * Node class and related stuff
 */
#ifndef NODE_H
#define NODE_H

#include <string>
#include <map>
#include <vector>

/*
 * Simple type for identifier
 */
typedef unsigned long long identifier;

/*
 * Structure to hold socket info of all nodes easily.
 * sockfd   : socket descriptor of client to send messages to that node
 * addrInfo : the corr addr for above
 * Needs to be associated with a node.
 */ 
struct NodeClientSocket {
  int sockfd;
  struct addrinfo* addrInfo;
};

/* Main Node class  
 * Lightweight. Need to normalize the values outside
 */
class Node{
  identifier id;  //unique identifier depending on m,n
  int simpleId;   //modulo of id pertaining to the current chord network
  std::string ip;
  int port;

  Node *predecessor;
  Node *successor;

  std::map<std::string,std::string> index;
  std::map<int, Node> fingertable;
  std::map<int, NodeClientSocket> nodesockets;

  int m; //the number of peers
  int n; //the size of the chord

  public:

  int succSockFd;
  struct addrinfo* succAddrInfo;
  int predSockFd;
  struct addrinfo* predAddrInfo;

  std::vector<Node> nodes;  // all the nodes

  /*
   * Main constructor
   * INPUT: Normalized ip and port
   */
  Node(std::string ip, int port);

  /*
   * Simple contructor
   */
  Node();

  /*
   * To construct a node .
   */
  void setup(std::string ip, int port);

  /*
   * Getval for ip
   */
  std::string getIp();

  /*
   * Getval for port
   */
  int getPort();


  /*
   * get the address of the node in ip:port form
   */
  std::string getAddress();

  /*
   * Set value of id
   * INPUT: externally hashed value
   */
  void setID(identifier id);

  /*
   * Simple getval function
   */
  identifier getID();

  /*
   * Set simpleID based on chordLength.
   * INPUT: chordLength: the number of slots in the circular chord
   */
  void setSimpleId(int chordLength);

  /*
   * Simple getval function
   */
  int getSimpleId() const;

  /*
   * set predecessor
   */
  void setPredecessor(Node* pred);

  /*
   * simple getval 
   */
  Node* getPredecessor();

  /*
   * set successor
   */
  void setSuccessor(Node* succ);

  /*
   * simple getval 
   */
  Node* getSuccessor();

  /*
   * Add an <filename, ip> entry to this index
   */
  void addToIndex(std::string filename, std::string ip);

  /*
   * Retrieve ip from index
   */
  std::string getFromIndex(std::string filename); 

  /*
   * Overloading < operator for use in sorting
   */
  friend bool operator<(const Node& first, const Node& second);

  /*
   * Overloading = operator for use in removing duplicates
   */
  friend bool operator==(const Node& first, const Node& second);

  /*
   * Setup finger table given set of all nodes.
   */
  void setupFingerTable(std::vector<Node> nodes, int chordSize, int chordLength);

  /*
   * Setup the map of nodes to socketinfo
   * Open sockets for all nodes in fingertable
   */ 
  void setupNodesockets();

  /*
   * Close all sockets. One server and all clients.
   * INPUT: server socket
   */
  void closeSockets(int &sockfd);

  /*
   * Return the client socket info for the node responsible for the input filehash.
   * Get the greatest entry in the fingertable smaller than the query.
   */ 
  NodeClientSocket getNodeSocketFor(int filehash);

  /*
   * Simple setval function.
   */ 
  void setN(int n);

  /*
   * Simple setval function.
   */ 
  void setM(int m);

  /*
   * Simple getval function.
   */ 
  int getN();

  /*
   * Simple getval function.
   */ 
  int getM();

  /*
   * Simple inc function: as new nodes coming in is a very important req
   */
  void incM();

  /*
   * When a new node is allowed in, to include it in the setup.
   * Typically called after a new node is pushed into the nodes list
   */ 
  void reinit();

  /*
   * Display current finger table.
   */ 
  void printFingertable();

  /*
   * Display current nodesocket mapping.
   */ 
  void printNodesockets();

  /*
   * Update index, given a new predecessor has joined.
   */
  std::map<std::string, std::string> distributeIndexOnJoin();
};


#endif
