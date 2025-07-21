#ifndef HorarisParser_hh
#define HorarisParser_hh

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include "../lib/json.hpp"
#include "Horari.hh"

namespace jaslo{

using json = nlohmann::json;

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

  void printParseError(const std::string& extraInfo);


  std::ifstream _file;
  json          _data;
  std::string   _info;
  int           _curIndex;
  int           _count;
  bool          _openedFile;
};

}; //Namespace end
#endif
