#ifndef HorarisParser_hh
#define HorarisParser_hh

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include "Horari.hh"

/*
  My attempt at writing a static parser for a FIB schedule generator.
  It can read, parse and store the Class list the FIB API provides (https://api.fib.upc.edu).
  
  That is the only thing it can do, so don't try to use it for anything else. It only "looks generic" because that way it's more reusable.
*/

namespace jaslo{

class Parser{
public:

  Parser();
  ~Parser();

  /*
    The number of different HorariObj that will have to be read
  */
  int count()const;

  /*
    Opens a local file with name [filename]
  */
  void openFile(const std::string& filename);

  /*
    Closes the opened file if it was open
  */
  void closeFile();

  /*
    Requires having opened a file.
    Reads everything until the "count": line in the file, and stores its value.
  */
  void getCount();

  /*
    Requires having opened a file and having called getCount() once.
    Reads an HorariObj. Returns false if it read the last one, and true otherwise
  */
  bool readHorariObj(HorariObj& o);

private:

  bool isCorrect(int expectedSize, int substrSize, const std::string& str, const std::string& substr)const;
  void printParseError(const std::string& extraInfo);


  std::ifstream _file;
  std::string   _info;
  int           _curIndex;
  int           _count;
  bool          _openedFile;
};

}; //Namespace end
#endif