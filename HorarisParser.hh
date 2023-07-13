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

class HorariObj{
public:

  HorariObj();
  std::string             code()  const;
  int                     group() const;
  DiaSetmana              day()   const;
  const std::vector<int>  hours() const;
  std::string             type()  const;
  
  //Mainly used to debug
  friend std::ostream& operator<<(std::ostream& os, const HorariObj& o);

private:

  friend class Parser;

  std::string       _code;
  int               _group;
  DiaSetmana        _day;
  std::vector<int>  _hours;
  std::string       _type;
  std::string       _classroom;
  std::string       _language;

};

inline std::ostream& operator<<(std::ostream& os, const HorariObj& o){
  os << "code:\t"  << o._code  << std::endl
     << "group:\t" << o._group << std::endl
     << "day:\t"   << o._day   << std::endl
     << "hours:\t";
  for(int h : o._hours){
    os << h << ":00h, ";
  }
  os << std::endl;
  os << "tipus:\t" << o._type << std::endl
  <<    "class:\t" << o._classroom << std::endl
  <<    "lang:\t"  << o._language << std::endl;
  return os;
}

class Data{
public:

  Data(){}
  Data(size_t size);
  void                        pushHorariObj(const HorariObj& o);
  void                        print();
  int                         firstOccurrence(const std::string& assignatura) const;
  std::vector<std::string>    names() const;
  /*
    Converts this data to vector<vector<HoraClasse>> format.
  */
  std::vector<
    std::vector<HoraClasse>>  allAssignatures() const;

  inline HorariObj&           operator[](int);
  inline const HorariObj&     operator[](int)const;

private:
  int binarySearch(int l, int r, const std::string& thing) const;
  
  std::vector<HorariObj> _info; //Input is ordered alphabetically
};

class Parser{
public:

  Parser();
  /*
    The number of different HorariObj that will have to be read
  */
  int count()const;
  /*
    Opens a local file with name [filename]
  */
  void openFile(const std::string& filename);
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