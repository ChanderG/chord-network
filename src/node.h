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

/*
 * Simple type for identifier
 */
typedef unsigned long long identifier;

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

  public:
  /*
   * Main constructor
   * INPUT: Normalized ip and port
   */
  Node(std::string ip, int port);

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
};

#endif
