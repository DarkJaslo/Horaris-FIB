#ifndef App_hh
#define App_hh

#include <qwidget.h>
#include <string>
#include <vector>
#include <set>
#include "HTTPSGetter.hh"
#include "Parser.hh"
#include "Data.hh"

class App : public QWidget
{
 Q_OBJECT

public:

  App(QWidget *parent = 0);
  //Gets both semesters for this year
  void getSemesters();
  //Gets the data into FIB_DATA.txt
  void getData();

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
  void generate();

signals:
  void writtenSchedules();
  void closeApp();
  void loadSemester(const QString& s);

private:

//_path = _path1 + _semester + _path2
void fillPath();

//Parses data
void parseData();

void computeSchedules();


std::string       semester;
const std::string url = "https://api.fib.upc.edu";
const std::string path1 = "/v2/quadrimestres/";
const std::string path2 = "/classes/?format=api&client_id=liSPe2KsaYUovErkk1WyqgMdYxOD1Wqd3VCXwhoy";
std::string       path;

const std::string pathSemesters = "/v2/quadrimestres/actual/?format=api&client_id=liSPe2KsaYUovErkk1WyqgMdYxOD1Wqd3VCXwhoy";

jaslo::Parser  parser;
jaslo::Data    data;
int     sizeHorari;
bool    mixGroups;
jaslo::SchedulePreference preference;
const static int   maxPrintedSchedules = 1000;
std::fstream outputFile;
const std::string outputFilename = "OUTPUT_SCHEDULES.txt"; 

std::set<std::string> mustAppearSubjects;
std::set<std::string> otherSubjects;
std::set<std::pair<std::string,int>> groupsToExclude;

};

#endif