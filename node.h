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

/*
 * Simple type for identifier
 */
typedef unsigned long long identifier;

/* Main Node class  
 * Lightweight. Need to normalize the values outside
 */
class Node{
  identifier id;  //unique identifier depending on m,n
  std::string ip;
  int port;

  public:
  /*
   * Main constructor
   * INPUT: Normalized ip and port
   */
  Node(std::string ip, int port);

  /*
   * Set value of id
   * INPUT: externally hashed value
   */
  void setID(identifier id);
};

#endif
