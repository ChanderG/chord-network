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

/* Member definitions of Node class. Other node related functions
 */
#include "node.h"

#include <string>

/* Main constructor
 * INPUT: Normalized ip and port
 */
Node :: Node(std::string ip, int port){
  this->ip = ip;
  this->port = port;
  this->id = 0;
  this->simpleId = 0;
}

/*
 * Set value of id
 * INPUT: externally hashed value
 */
void Node::setID(identifier id){
  this->id = id;
}

/*
 * Simple getval function
 */
identifier Node::getID(){
  return id;
}

/*
 * Set simpleID based on chordLength.
 * INPUT: chordLength: the number of slots in the circular chord
 */
void Node::setSimpleId(int chordLength){
  simpleId = id%chordLength;
}

/*
 * Simple getval function
 */
int Node::getSimpleId(){
  return simpleId;
}

/* 
 * Compare function for 2 nodes based on simpleId. For sorting.
 */
bool compare_simpleId(Node &first, Node &second){
  return (first.getSimpleId() < second.getSimpleId());
}

/* 
 * Equality function for 2 nodes based on simpleId. For uniqueness testing.
 */
bool equal_simpleId(Node &first, Node &second){
  return (first.getSimpleId() == second.getSimpleId());
}
