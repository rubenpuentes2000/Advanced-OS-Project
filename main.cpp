//#include "sdisk.h"
#include "filesys.h"
#include "shell.h"
#include "block.h"
#include "table.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "filesys.h"
#include "shell.h"

using namespace std;

int main()
{
 //
 //This main program inputs commands to the shell.
 //It inputs commands as : command op1 op2
 //You should modify it to work for your implementation.
 //
 
 Table table("disk1",256,128,"ffile","ifile");
 table.buildtable("data.txt");
 Shell shell("disk1",256,128);
 string s;
 string command="go";
 string op1,op2;

 while (command != "quit")
     {
       command.clear();
       op1.clear();
       op2.clear();
       cout << "$";
       getline(cin,s);
       int firstblank=s.find(' ');
       if (firstblank < s.length()) s[firstblank]='#';
       int secondblank=s.find(' ');
       command=s.substr(0,firstblank);
       if (firstblank < s.length())
         op1=s.substr(firstblank+1,secondblank-firstblank-1);
       if (secondblank < s.length())
         op2=s.substr(secondblank+1);
       if (command=="dir")
          {
            // use the ls function
	    	shell.dir();
           }
       if (command=="add")
          {
            // The variable op1 is the new file and op2 is the file data
	    	shell.add(op1, op2);
           }
       if (command=="del")
          {
            // The variable op1 is the file
	    	shell.del(op1);
           }
       if (command=="type")
          {
            // The variable op1 is the file
	    	shell.type(op1);
           }
       if (command=="copy")
          {
            // The variable op1 is the source file and the variable op2 is the destination file.
	    	shell.copy(op1, op2);
           }
       if (command=="search")
          {
            // This is the command for Project 4
            // The variable op1 is the date
		 table.search(op1);
           }

      }

 return 0;
}

