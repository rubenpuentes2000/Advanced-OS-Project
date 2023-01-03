#include <iostream>
#include <fstream>
#include <string>
#include "sdisk.h"
//#include "block.h"

using namespace std;


Sdisk::Sdisk(string diskname, int numberofblocks, int blocksize)
{
  this->diskname = diskname;
  this->blocksize = blocksize;
  this->numberofblocks= numberofblocks;
  //     .
  //     .
  //     .
  // this->blocksize = ....
  
  // check if diskname exists
  ifstream indisk;
  indisk.open(diskname.c_str());

  if(indisk.fail())
  {
    // write blocksize x numberofblocks to diskname
    indisk.close();
    ofstream outdisk;
    outdisk.open(diskname.c_str());
    for(int i = 0; i < numberofblocks * blocksize; i++)
    {
      outdisk.put('#');
    }
  }
  else 
  {
    indisk.open("filename");
    int counter = 0;
    char c;
    indisk.get(c);
   	 while(indisk.good())
	 {
   		 counter++;
   		 indisk.get(c);
   	 }
  	if((counter = numberofblocks * blocksize))
	{
     		cout << "This size is correct\n";
    		return;
  	}
  }
}


int Sdisk::putblock(int blocknumber, string buffer)
{
  fstream iofile;
  iofile.open(diskname.c_str(), ios::in|ios::out);

  if(blocknumber < 0 | blocknumber >= numberofblocks)
  {
    cout << "Block doesn't exist\n";
    return 0;
  }

  iofile.seekp(blocksize * blocknumber);
  

  // blocksize is equal buffer.size()
  for(int i = 0; i < blocksize; i++)
  {
    iofile.put(buffer[i]);
  }
  return 0;
  }

int Sdisk::getblock(int blocknumber, string &buffer)
{
  fstream iofile;
  iofile.open(diskname.c_str(), ios::in|ios::out);

  if(blocknumber < 0 | blocknumber >= numberofblocks)
  {
    cout << "Block doesn't exist\n";
    return 0;
  }
  iofile.seekg(blocknumber * blocksize);
  buffer.clear();
  char c;
  

  for(int i = 0; i < blocksize; i++)
  {
    iofile.get(c);
    buffer.push_back(c);
  }
  return 0;
}

int Sdisk::getnumberofblocks()
{
  return numberofblocks;
}

int Sdisk::getblocksize()
{
  return blocksize;
}



