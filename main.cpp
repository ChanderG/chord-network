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

#include"node.h"
#include"hash.h"
#include"helper.h"

#include <iostream>
#include <string>
#include <sstream>
using namespace std;

/* The main module represnting a peer.
 * INPUT: port number, list of all machines on the Chord network
 * ACTION: setup this node, connect to next and prev nodes, interact with user to share/search for files
 */
int main(int argc, char* argv[]){

  if(argc < 3){
    cout << "Incorrect ususage!!" << endl;
    cout << "./main <port> <file.txt>" << endl;
    cout << "<port> : refers to the port to use for connecting to the chord network" << endl;
    cout << "<file.txt> : needs to have all peers in this network in form of" << endl;
    cout << "\t<ip> <port> " << endl;
    cout << "\tone on each line." << endl;
    return 0;
  }

  string ip;
  int port;

  //extract the input from args
  stringstream ss;
  ss << argv[1];
  ss >> port;
  ss.clear();

  ip = getIP();
  cout << "IP is : " << ip << endl;
  cout << "Reading port as : " << port << endl;
  if(port <= 0 || port > 65535){
    cout << "Specified port is invalid. Try again." << endl;
    return 0;
  }
  
  return 0;
}
