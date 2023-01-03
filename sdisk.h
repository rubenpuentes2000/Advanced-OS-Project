#ifndef  SDISK_H
#define SDISH_H
#include <string>

using namespace std;

class Sdisk{
 public:
  Sdisk(string diskname, int numberofblocks, int blocksize);
  int getblock(int blocknumber, string &buffer);
  int putblock(int blocknumber, string buffer);
  int getnumberofblocks(); // accessor function
  int getblocksize(); // accessor function
   
 private:
  string diskname;
  int numberofblocks;
  int blocksize;

};

#endif
