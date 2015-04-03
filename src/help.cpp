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
 * Manage help messages for user in command line.
 */
#include "help.h"

#include <iostream>
using namespace std;

/*
 * To guide user in case of incorrect usuage.
 */
void printHelpPrompt(){
  cout << "Usuage: ./main <PORT> [OPTION] ..." << endl;
  cout << "Try './main --help' for more information." << endl;
}

/*
 * Full help message.
 */ 
void printHelpMessage(){
  cout << "Usuage: ./main <PORT> [OPTION] ..." << endl;
  cout << "There are 2 main usuage modes:" << endl;
  cout << endl;
  cout << "New network: " << endl;
  cout << "  ./main <PORT>" << endl;
  cout << "  Creates a new chord network." << endl;
  cout << "  PORT: refers to port on this machine to use to connect." << endl;
  cout << "  A valid config.chord file needs to present in conf/ directory." << endl;
  cout << "  Have a look at the provided sample file." << endl;
    
  cout << endl;
  cout << "Join existing network: " << endl;
  cout << "  ./main <PORT> -j <IP> <PORT>" << endl;
  cout << "  Join an existing chord network." << endl;
  cout << "  PORT: refers to port on this machine to use to connect." << endl;
  cout << "  IP PORT refers to any node in the existing network." << endl;

  cout << endl;
  cout << "Help: " << endl;
  cout << "  --help " << endl;
  cout << "  Display this document and exit." << endl;
}
