#ifndef Horari_hh
#define Horari_hh

#include <iostream>
#include <vector>
#include <string>

namespace jaslo{

enum Weekday{
  monday,
  tuesday,
  wednesday,
  thursday,
  friday
};

inline std::ostream& operator<<(std::ostream& os, const Weekday& d){
  switch(d){
    case monday:    os << "dilluns";   break;
    case tuesday:   os << "dimarts";   break;
    case wednesday: os << "dimecres";  break;
    case thursday:  os << "dijous";    break;
    case friday:    os << "divendres"; break;
    default:        os << "error";     break;
  }
  return os;
}

enum ClassType{
  theory,
  lab,
  problems
};

inline std::ostream& operator<<(std::ostream& os, const ClassType& t){
  switch(t){
    case theory:   os << "T";     break;
    case lab:      os << "L";     break;
    case problems: os << "P";     break;
    default:       os << "error"; break;
  }
  return os;
}

//A unit of data in the FIB's API.
class HorariObj{
public:

  HorariObj();
  //Name of the subject. Sometimes is referred as code too.
  std::string             code()  const;
  int                     group() const;
  Weekday                 day()   const;
  const std::vector<int>  hours() const;
  ClassType               type()  const;
  bool                    equivalent(const HorariObj& other) const;
  
  //Mainly used to debug
  friend std::ostream& operator<<(std::ostream& os, const HorariObj& o);

private:

  friend class Parser;
  friend class Data;

  std::string       _code;
  int               _group;
  Weekday           _day;
  std::vector<int>  _hours;
  ClassType         _type;
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

class ClassHour{
public:

  ClassHour();
  ClassHour(int hour, int group, ClassType type, Weekday day, bool taken, const std::string& subjectName);

  std::string subjectName() const;
  int         group() const;
  Weekday     day() const;
  int         hour() const;
  bool        taken() const;
  ClassType   type() const;
  void        print()const;
  void        take();

private:

  friend class Horari;

  int         _hour;
  int         _group;
  ClassType   _type;
  Weekday     _day;
  bool        _taken;
  std::string _subjectName;

};

enum SchedulePreference{
  morning,
  afternoon,
  dont_care
};

//Represents a schedule for a week
class Horari{
public:

  Horari();
  bool  doesItFit(const std::vector<HorariObj>& subj);
  //Assumes subject fits (call doesItFit() to check) and that all HorariObjs in subj share the same subject name
  void  addSubject(const std::vector<HorariObj>& subj);
  int   subjectCount() const;
  //Assumes computeValue() has been called
  int   value() const;
  bool  operator<(const Horari& other)const{ return _value > other._value; }
  void  print() const;
  void  computeValue(SchedulePreference pref);


private:
  //To index the H vector
  static int indexFor(Weekday day, int hour);
  bool containsSubject(const std::string& name) const;

  //ClassHour H[65];
  std::vector<ClassHour>   H;
  int                      _value;
  std::vector<std::string> _subjectNames;

};


}; //namespace end

#endif