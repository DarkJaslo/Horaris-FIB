#ifndef App_hh
#define App_hh

#include <qwidget.h>
#include <QListWidget>
#include <string>
#include <vector>
#include <set>
#include "HTTPSGetter.hh"
#include "Parser.hh"
#include "Data.hh"
#include "MyComboBox.hh"

class App : public QWidget
{
 Q_OBJECT

public:

  App(QWidget *parent = 0);
  //Gets both semesters for this year
  void getSemesters();
  //Gets the data into FIB_DATA.txt
  void getData();
  void receiveListWidget(QListWidget* list);
  void receiveStudiesBox(MyComboBox* box);
  void receiveSelectionLists(QListWidget* listInclude, QListWidget* listAlways);
  void receiveMixedLabel(QCheckBox* checkBox);
  void receiveFilterLineEdit(QLineEdit* filterLineEdit);
  void receiveGenerateButton(QPushButton* generateButton);
  void receiveTable(QTableWidget* table);
  void receiveNumLabel(QLabel* label);
  void changeCurrentSchedule(int change);
  void displaySchedule();

public slots:
  void setMajor(const QString& m);
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
  void setFilter(const QString& filter);
  void moveFromMainList(QListWidgetItem* item);
  void moveFromIncludeList(QListWidgetItem* item);
  void moveFromAlwaysList(QListWidgetItem* item);
  void generate();
  void changeLanguage(const QString& language);
  void decreaseCurrentSchedule();
  void increaseCurrentSchedule();

signals:
  void writtenSchedules();
  void closeApp();
  void loadSemester(const QString& s);
  void changedSemester();
  void loadMajor(const QString& m);
  void majorTagChanged(const QString& text);
  void semesterTagChanged(const QString& text);
  void noSubjectsTagChanged(const QString& text);
  void preferenceTagChanged(const QString& text);
  void includeTagChanged(const QString& text);
  void forceTagChanged(const QString& text);
  void languageTagChanged(const QString& text);

private:

//Parses data
void parseData(int index, const std::string& filename);

void computeSchedules();

void initList();
void initList(const std::string& filter);

void sortList();

void getDataTask(int semest);

void initStudiesBox();

void clearSelection();


int semester;
std::string       semesters[2];
std::string       major;
std::vector<std::vector<std::string>> majors;
std::string       filter;

const std::string url = "https://api.fib.upc.edu";
const std::string path1 = "/v2/quadrimestres/";
const std::string path2 = "/classes/?format=api&client_id=liSPe2KsaYUovErkk1WyqgMdYxOD1Wqd3VCXwhoy";

const std::string pathSemesters = "/v2/quadrimestres/actual-horaris/?format=api&client_id=liSPe2KsaYUovErkk1WyqgMdYxOD1Wqd3VCXwhoy";
const std::string pathStudies = "";

std::vector<jaslo::Data>  data;
int     sizeHorari;
bool    mixGroups;
jaslo::SchedulePreference preference;
const static int   maxPrintedSchedules = 1000;
std::fstream outputFile;
const std::string outputFilename = "OUTPUT_SCHEDULES.txt"; 

std::set<std::string> mustAppearSubjects;
std::set<std::string> otherSubjects;
std::set<std::pair<std::string,int>> groupsToExclude;
QListWidget* list;
QListWidget* listInclude;
QListWidget* listAlways;
MyComboBox* studiesBox;
QCheckBox* mixCheckBox;
QLineEdit* filterLineEdit;
QPushButton* generateButton;

std::vector<std::vector<std::string>> allSubjectNames;

std::vector<std::map<std::string,std::vector<std::string>>> allSubjectNamesByMajor;

std::string lang = "English"; //Options are English, Español, Catala

QTableWidget* table;
QLabel* numLabel;
int currentSchedule;
};

#endif