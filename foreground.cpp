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

#include "foreground.h"

#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

/*
 * Handle sharing. Needs to take inputs
 */
void handleShare(int &chordLength, Node &self){
  string fileName;
  cout << "Enter file name: "; 
  cin >> fileName;

  if(fileName.length() >= MAXFILENAME){
    cout << "Filename too long. " << endl;
    return;
  }

  //hash the string
  int filehash = hashfunc(fileName.c_str(), fileName.length());
  //mod it to this chord network
  filehash %= chordLength;

  cout << "Hashing file: " << fileName << " to " << filehash << endl;

  //check if the file needs to be added to this very node

  //else compose a message and send it on
  Comm mess;
  mess.type = REQ_SHARE;
  mess.src = self.getSimpleId();

  bzero(mess.filename, MAXFILENAME);
  strcpy(mess.filename, fileName.c_str());

  bzero(mess.ipaddr, MAXIPLEN);
  strcpy(mess.ipaddr, self.getIp().c_str());
  
  mess.filehash = filehash;

  return;
}


/*
 * Main user terminal
 * INPUT: the chord length and the current node
 */
void manageNodeTerminal(int &chordLength, Node &self){
  cout << "Node user interface work." << endl;

  int choice;

  while(1){
    //safety value; to prevent endless loop
    choice = 0;  

    cout << "***Main Menu***" << endl;
    cout << "1. Search for file" << endl;
    cout << "2. Share a file" << endl;
    cout << "3. Quit" << endl;
    cout << "Enter choice: ";

    cin >> choice;
    switch(choice){
      case 1: {
		break;
              }		
      case 2: {
		handleShare(chordLength, self);
		break;
              }		
      case 3: {
		return;
              }		
      default: cout << " Invalid option." << endl;
    }
  }

  return;
}
