
#include "shell.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

Shell::Shell(string diskName, int numberOfBlocks, int blockSize): Filesys(diskName, numberOfBlocks, blockSize) 
{
  
}

int Shell::dir()
{ 
  vector<string> flist=ls();
  for (int i=0; i<flist.size(); i++)
      {
        cout << flist[i] << endl;
      }
  return 0;
}

int Shell::add(string file, string buffer)
{
  int code = newfile(file);

  if(code == 1)
  {
    vector<string> blocks = block(buffer, getblocksize());
    for(int i = 0; i < blocks.size(); i++)
    {
      code = addblock(file, blocks[i]);
    }
    return 1;
  }
  return 0;
}

int Shell::del(string file)
{
  // try deleting the blocks in order one after another 
  
  while(getfirstblock(file) != 0)
  {
    delblock(file, getfirstblock(file));
  }

  rmfile(file);

  return 1;

}

int Shell::type(string file)
{
  string buffer;

  int block = getfirstblock(file);
  while(block > 0)
  {
    string b;
    readblock(file, block, b);
    buffer += b;
    block = nextblock(file, block);
  }
  cout << buffer;  
  return 0;
}

int Shell::copy(string file1, string file2)
{
  string buffer;

  int block = getfirstblock(file1);
  while(block > 0)
  {
    string b;
    readblock(file1, block, b);
   // addblock(file2, buffer);
    buffer += b;
    block = nextblock(file1, block);
  }
  
  add(file2,buffer);
  return 0;
}



