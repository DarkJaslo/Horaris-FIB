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

class Data{
public:

  Data(){}
  Data(size_t size);
  void                        pushHorariObj(const HorariObj& o);
  void                        print();
  int                         firstOccurrence(const std::string& subjectName) const;
  std::vector<std::string>    names() const;
  //Deletes all groups of subjects whose names aren't in names
  void                        deleteNonRequestedGroups(const std::vector<std::string>& names);
  //The API isn't consistent with the HorariObjs, so this needs to be called once after deleteNonRequestedGroups() and before deleteReduntantGroups()
  void                        joinGroups();
  //For all equivalent subgroups with the same hours (eg. G11, G12, G13), leaves only one of them
  void                        deleteRedundantGroups();
  //Deletes all subgroups specified in groups
  void                        deleteExcludedGroups(const std::vector<std::pair<std::string,int>>& groups);
  //Organizes the data. Parameter mix specifies if lab groups different from their intended theory group can be tried. Set mix to false unless you know what you're doing.
  void                        makeGroups(bool mix);
  //Does the heavy work. subjNum is num of subjects wanted in every schedule, always are the names of must-appear subjects, and include are the names of the rest.
  void                        makePermutations(int subjNum, const std::vector<std::string>& always, const std::vector<std::string>& include);
  //Tries the permutations, saves valid schedules and prints them up to maxPrintedSchedules in order
  void                        makeAndPrintSchedules(SchedulePreference preference, int maxPrintedSchedules);

  inline HorariObj&           operator[](int);
  inline const HorariObj&     operator[](int)const;

private:
  static bool contains(const std::vector<std::string>& names, const std::string& name);
  int         binarySearch(int l, int r, const std::string& thing) const;

  //Used to make permutations of size current.size() of options.size() elements 
  void        onePerGroupPermutations(int minUsed, int cur, const std::vector<int>& options, std::vector<bool>& used, std::vector<int>& current, std::vector<std::vector<int>>& allPermutations);

  /*Used to make permutations of size current.size() of current.size() groups of N elements each, choosing one element of each group.
  For instance, G1{e1,e2}, G2{e3,e4,e5}, G3{e6,e7,e8} would make P1{e1,e3,e6}, P2{e1,e3,e7}, P3{e1,e3,e8}, P4{e1,e4,e6}...*/
  void        manyPerGroupPermutations(int cur, const std::vector<int>& groupsPerSubject, std::vector<int>& current, std::vector<std::vector<int>>& allPermutations);

  /*Joins permutation source1 with permutations in source2 in dest.
  For instance, permutation P1{0,1,2}, if joined with permutations P2{3,4}, P3{3,5}, would make P12{0,1,2,3,4} and P13{0,1,2,3,5}*/
  void        copyVector(const std::vector<int>& source1, const std::vector<std::vector<int>>& source2, std::vector<std::vector<int>>& dest, int loc);
  

  std::vector<HorariObj> _info; //Input is ordered alphabetically

  /* A vector that stores subject groups for a schedule
  First of pair is the name of the subject
  Second is a vector of vectors of indices, each index being a "pointer" to a HorariObj in _info  */
  std::vector<std::pair<std::string,std::vector<std::vector<int>>>> _groups;
  
  /*Indices to _groups' first vector*/
  std::vector<std::vector<int>> _subjectPermutations;

  /*First is the index "I" for _groups
  Second is the index for _groups[I].second */
  std::vector<std::vector<std::pair<int,int>>> _perms;
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