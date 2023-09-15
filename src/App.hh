#ifndef App_hh
#define App_hh

#include <qwidget.h>
#include <string>
#include <vector>
#include <set>
#include "HTTPSGetter.hh"
#include "Parser.hh"
#include "Data.hh"

namespace jaslo{

class App : public QWidget
{
 Q_OBJECT

public:

  App(QWidget *parent = 0);

public slots:
  //Downloads data in FIB_DATA.txt
  void init();
  void setSemester(const QString& s);
  void setScheduleSize(int size);
  void setMixGroups(bool mix);
  void setSchedulePreference(const QString& pref);
  void addSubject(const QString& subj);
  void removeSubject(const QString& subj);
  void addSubjectAlways(const QString& subj);
  void removeSubjectAlways(const QString& subj);
  void excludeGroup(const QString& subjGr);
  void unExcludeGroup(const QString& subjGr);

signals:
  //Triggers when schedules are generated and printed
  void writtenSchedules();

private:

//_path = _path1 + _semester + _path2
void fillPath();

//Gets the data into FIB_DATA.txt
void getData();

//Parses data
void parseData();

void computeSchedules();


std::string       semester;
const std::string url = "https://api.fib.upc.edu";
const std::string path1 = "/v2/quadrimestres/";
const std::string path2 = "/classes/?format=api&client_id=liSPe2KsaYUovErkk1WyqgMdYxOD1Wqd3VCXwhoy";
std::string       path;

Parser  parser;
Data    data;
int     sizeHorari;
bool    mixGroups;
SchedulePreference preference;
const static int   maxPrintedSchedules = 1000;
std::set<std::string> mustAppearSubjects;
std::set<std::string> otherSubjects;
std::set<std::pair<std::string,int>> groupsToExclude;

};
}; //namespace end

#endif