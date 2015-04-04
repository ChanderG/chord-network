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
 * Functions involved in the starting up of the network.
 */ 
#include "node.h"

#include "helper.h"

#include <string>
#include <vector>

using namespace std;
/*
 * Reads the "conf/config.chord" file and parses out the chord network information.
 * INPUT: reference variables to store 
 * 	m     : the number of peers/nodes
 * 	n     : the size of the chord
 * 	nodes : the list of nodes extracted from the file 
 * ACTION: parses out the info and stores them in these variables 	
 */
void readConfiguration(int &n, int &m, vector<Node> &nodes);

/*
 * Contact a suspected member of the ring, to get complete info about the chord
 * Info that we would otherwise have got from a config file.
 * The contact would have to take care that the new guy fits with the network.
 * INPUT: self : self node
 *        ip   : ipaddr of suspect  
 *        port : port of the same 
 *        m    : number of peers including self  
 *        n    : size of the chord
 *        nodes: the list if all nodes including self
 */
void startupFromExisting(Node &self, string ip, int port, int &n, int &m, vector<Node> &nodes);
