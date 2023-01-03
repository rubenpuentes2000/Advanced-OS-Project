#include <iostream>
#include <fstream>
#include <string>
//#include "sdisk.h"
#include "filesys.h"
#include <sstream>
#include <vector>
//#include "block.h"



using namespace std;

Filesys::Filesys(string diskname, int numberofblocks, int blocksize):Sdisk(diskname,numberofblocks,blocksize)
{  
  rootsize = getblocksize() / 12;

  fatsize = ((getnumberofblocks()*5)) / getblocksize() + 1;

  string buffer;

  getblock(1,buffer);

  if(buffer[0] == '#')
  {
   	 buildfs();
   	 fssynch();
  }
  else
  {
   	 readfs();
  }
}

int Filesys::fsclose()
{
    return 1;  
}


int Filesys::newfile(string file)
{
  // creates empty file
  // -1 file already exists
  // 0 no room left
  // 1 everything is okay
  for(int i = 0; i < filename.size(); i++)
  {
 	 if(filename[i] == file)
	 {
     		 return -1;
   	 }
  }
  for(int i = 0; i < filename.size(); i++)
  {
   	 if(filename[i] == "XXXXXX")
   	 {
     		 filename[i] = file;
     		 firstblock[i] = 0;
     		 fssynch();
     		 return 1;
   	 }
  }
  return 0;
}

int Filesys::rmfile(string file)
{
  // deletes empty file
  // -1 file does not exists
  // 0 file not empty
  // 1 everything is okay
  for(int i = 0; i < filename.size(); i++)
  {
    if(filename[i] == file)
    {
      if(firstblock[i] == 0)
      {
        filename[i] = "XXXXXX";
        fssynch();
        return 1;
      }
      else
      {
        return 0;
      }
    }
  }
  // return 0;
  // file not found
  return -1;
}

int Filesys::getfirstblock(string file)
{
  // return firstblock numberoffile
  // -1 file does not exists
  for(int i = 0; i < filename.size(); i++)
  {
    if(filename[i] == file)
    {
      return firstblock[i];
    }
  }
  return -1;
}

int Filesys::addblock(string file, string buffer)
{
  // returns block number of allocated block
  int allocate = fat[0];
  if (allocate <= 0 )
  {
    std::cout << "No space on disk\n";
    // 0 indicates failed
    return 0;
  }

  int blockid = getfirstblock(file);

  if (blockid == -1)
  {
    cout << "File doesn't exist\n";
    return 0;
  }
  if (blockid == 0)
  {
    // update root
      for(int i = 0; i < filename.size(); i++)
      {
     		 if (filename[i] == file)
		 {
         		 firstblock[i] = allocate;
         		 break;
       		 }
      }
   } // update fat find the the EOF
  else 
    {
      	 while (fat[blockid] != 0)
    	 {
        	 blockid = fat[blockid];
         }
       // fat[blockid] is equal to 0
      fat[blockid] = allocate;
   } 
  fat[0] = fat[fat[0]];
  fat[allocate] = 0;
  fssynch();
  putblock(allocate, buffer);
  return allocate;
}

int Filesys::delblock(string file, int blocknumber)
{
  bool flag = fbcheck(file, blocknumber);

  if(not flag)
  {
    return 0;
  }

  int b = getfirstblock(file);

  if(b == blocknumber)
  {
   	 for(int i = 0; i < filename.size(); i++)
   	 {
     		 if(filename[i] == file)
      		 {
       			 firstblock[i] = fat[firstblock[i]];
       			 break;
     		 }
   	 }
  } 
  else 
  {
    while(fat[b] != blocknumber)
      {
        b = fat[b];
      }
    fat[b] = fat[blocknumber];
  }
  fat[blocknumber] = fat[0];
  fat[0] = blocknumber; 
  return 1; 
}

int Filesys::readblock(string file, int blocknumber, string &buffer)
{
  if(fbcheck(file, blocknumber))
  {
   getblock(blocknumber,buffer);
   return 1; 
  }
  else 
  {
    return 0;
  }
}

int Filesys::writeblock(string file, int blocknumber, string buffer)
{
  if(fbcheck(file, blocknumber))
  {
    putblock(blocknumber, buffer);
    return 1;
  }
  else
  {
    return 0;
  }
}

int Filesys::nextblock(string file, int blocknumber)
{
  // return nextblock in file after blocknumber
  // return -1 if something is wrong
  if (fbcheck(file, blocknumber))
  {
    return fat[blocknumber]; 
  }
  else 
  {
    return -1;
  }
}

int Filesys::fssynch()
{
  // synch file system

  ostringstream outstream1;

  for(int i = 0; i < firstblock.size(); i++)
  {
    outstream1 << filename[i] << " " << firstblock[i] << " ";
  }

  ostringstream outstream2;

  for(int i = 0; i < fat.size(); i++)
  {
    outstream2 << fat[i] << " ";
  }

  string buffer1 = outstream1.str();
  string buffer2 = outstream2.str();
  // cout << buffer1 << buffer2;
  // to see how they look like

  vector<string> blocks1 = (block(buffer1, getblocksize()));
  vector<string> blocks2 = (block(buffer2, getblocksize()));
  putblock(1, blocks1[0]);

  for(int i = 0; i < blocks2.size(); i++)
  {
	putblock(2+i,blocks2[i]);
    //	putblock(fatsize+2+i,blocks2[i]);
  }
  return 1;
}

int Filesys::buildfs()
{

  for(int i = 0; i < rootsize; i++)
  {
    filename.push_back("XXXXXX");
    firstblock.push_back(0);
  }
  // 2 + fatsize is first data block

  fat.push_back(2+fatsize);
  fat.push_back(-1);

  for(int i = 0; i < fatsize; i++)
  {
    fat.push_back(-1);
  }

  for(int i = 2 + fatsize; i < getnumberofblocks(); i++)
  {
    fat.push_back(i+1);
  }

  // fatsize() is what is in the notes
  fat[fat.size()-1] = 0;

  return 1;
}

int Filesys::readfs()
{
  // root in buffer1, FAT in buffer2
  string buffer1, buffer2;

  getblock(1, buffer1);

  for(int i = 0; i < fatsize; i++)
  {
    string b;
    getblock(i+2,b);
    buffer2 += b;
  }
    istringstream instream1;
    istringstream instream2;

    instream1.str(buffer1);
    instream2.str(buffer2);

    filename.clear();
    firstblock.clear();

    for(int i = 0; i < rootsize; i++)
    {
      string f;
      int b;
      instream1 >> f >> b;
      filename.push_back(f);
      firstblock.push_back(b);
    }

    for(int i = 0; i < getnumberofblocks(); i++)
    {
      int k;
      instream2 >> k;
      fat.push_back(k);
    }
  return 1;
}

bool Filesys::fbcheck(string file, int blocknumber)
{
  int b = getfirstblock(file);

  if(b == -1)
  {
   	 return false;
  }
  
  while(b != 0)
  {
   	 if(b == blocknumber)
	 {
     		 return true;
    	 }
	 else
	 {
      		b = fat[b];
   	 }
  }
  return false;
}


vector<string> Filesys::ls()
{ vector<string> flist;
  for (int i=0; i<filename.size(); i++)
      {
        if (filename[i] != "XXXXXX")
           {
             flist.push_back(filename[i]);
           }
      }
  return flist;
}


/*
  newfile(file1)
  addblock(file1,b)
  addblock(file1,b)
  addblock(file1,b)
*/
