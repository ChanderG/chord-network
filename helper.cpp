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

#include "helper.h"


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <cstdlib>

using namespace std;

//to get IP address of the current machine
//OUTPUT: the IP address as a string
std::string getIP(void){
  std::string ip = "127.0.0.1";
  return ip;
}

/*
 * Reads the "conf/config.chord" file and parses out the chord network information.
 * INPUT: reference variables to store 
 * 	m     : the number of peers/nodes
 * 	n     : the size of the chord
 * 	nodes : the list of nodes extracted from the file 
 * ACTION: parses out the info and stores them in these variables 	
 */
void readConfiguration(int &n, int &m, list<Node> &nodes){
  //try to read config file
  ifstream configfile;
  configfile.open("conf/config.chord", ios::in);
  if(!configfile.good()){
    cout << "Config file does not exist!!" << endl;  
    cout << "Rename config.chord.test to config.chord for a sample chord file." << endl;
    exit(1);
  }

  cout << "Config file found. Loading configuration." << endl;

  //parse the file
  string line;
  stringstream ss;
  string ip;
  int port;

  string commentPrefix = ";";
  string param_chordSize = "chordSize";
  string param_numPeers = "numPeers";

  while(getline(configfile, line)){

    // commented out lines 
    if(line.compare(0, commentPrefix.length(), commentPrefix) == 0){
      continue;
    }

    if(line.compare(0, param_chordSize.length(), param_chordSize) == 0){
      line.erase(0, param_chordSize.length());   
      ss << line;
      ss >> n;
      ss.clear();
      cout << "Extracted n: " << n << endl;
    }

    if(line.compare(0, param_numPeers.length(), param_numPeers) == 0){
      line.erase(0, param_numPeers.length());   
      ss << line;
      ss >> m;
      ss.clear();
      cout << "Extracted m: " << m << endl;

      //also read the next m lines for the nodes
      for(int i = 0;i < m; i++){
	if(!getline(configfile, line)){
	  //incomplete config file
	  cout << "Bad config file!!" << endl;
	  exit(1);
        }

        //create the node and add it to the list 
        ss << line;
	ss >> ip;
	ss >> port;
	ss.clear();

	cout << "Reads " << ip << " : " << port << endl;

	Node newnode(ip, port);
        nodes.push_back(newnode);

	break;
      }
    }
  }

  configfile.close();
}
