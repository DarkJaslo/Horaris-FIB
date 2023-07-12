#ifndef HorarisParser_hh
#define HorarisParser_hh

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include "Horari.hh"

/*
  My attempt at writing a very static and dumb parser for a FIB schedule generator.
  It can read, parse and store the Class list the FIB API provides.
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
    Gets the information from the FIB API. Currently unused because HTTPS isn't implemented yet.
  */
  void getInfo();
  /*
    Opens a local file with name [filename]
  */
  void openFile(const std::string& filename);
  //Requires opening a file
  void getCount();
  /*
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