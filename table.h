#ifndef TABLE_H
#define TABLE_H
#include <string>
#include <vector>
#include <sstream>
#include "filesys.h"


class Table : public Filesys
{
public:
Table(string diskname,int numberofblocks,int blocksize, string flatfile, string indexfile);
// string input_file = data.txt 
int buildtable(string input_file); // build flatfile and indexfile 
int search(string value); // search indexfile for block # return block! 
private :
string flatfile;
string indexfile;
int indexSearch(string value);
};

#endif



