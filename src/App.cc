#include "App.hh"
#include <sstream>
using namespace jaslo;

//Public

App::App(QWidget* parent) : QWidget(parent)
{
  setFocusPolicy(Qt::StrongFocus); //Can get keyboard events
  init();
}

void App::getSemesters()
{
  if(not HTTPSGetter::get(url,pathSemesters,"SEMESTER.txt")) exit(EXIT_FAILURE);

  std::fstream auxFile;
  auxFile.open("SEMESTER.txt", std::ios::in);

  bool gotSemester = false;

  std::string semest;

  while(not gotSemester and auxFile.is_open() and not auxFile.eof())
  {
    std::string line;
    getline(auxFile,line);
    std::istringstream iss(line);

    std::string aux;
    iss >> aux;
    if(aux == "\"id\":")
    {
      iss >> aux;
      semest = aux.substr(1,6);
      gotSemester = true;
    }
  }

  std::string aux = semest;
  if(aux[5] == '1')
  {
    aux[5] = '2';
  }
  else if(aux[5] == '2')
  {
    semest[5] = '1';
  }

  //loadSemester no funciona
  emit loadSemester(QString::fromStdString(semest));
  emit loadSemester(QString::fromStdString(aux));
}

void App::getData()
{
  fillPath();
  if(not HTTPSGetter::get(url,path,"FIB_DATA.txt")) exit(EXIT_FAILURE);
}

//Public slots

void App::init(){}
void App::setSemester(const QString& s)
{ 
  semester = s.toStdString();
}
void App::setScheduleSize(int size)
{ 
  sizeHorari = size; 
}
void App::setMixGroups(bool mix)       
{ 
  mixGroups = mix;           
}
void App::setSchedulePreference(const QString& pref)
{
  std::string aux = pref.toStdString();
  if(aux == "Morning") preference = SchedulePreference::morning;
  else if(aux == "Afternoon") preference = SchedulePreference::afternoon;
  else preference = SchedulePreference::dont_care;
}
void App::addSubject(const QString& subj)
{
  otherSubjects.insert(subj.toStdString());
}
void App::removeSubject(const QString& subj)
{
  otherSubjects.erase(subj.toStdString());
}
void App::addSubjectAlways(const QString& subj)
{
  mustAppearSubjects.insert(subj.toStdString());
}
void App::removeSubjectAlways(const QString& subj)
{
  mustAppearSubjects.erase(subj.toStdString());
}
void App::excludeGroup(const QString& subjGr)
{
  //Unknown format at the moment
}
void App::unExcludeGroup(const QString& subjGr)
{
  //Unknown format at the moment
}
void App::generate()
{
  computeSchedules();
}




//Private

void App::fillPath()
{
  //Path = path1 + semester + path2
  path = path1;
  path.append(semester);
  path.append(path2);
}

void App::parseData()
{
  parser.openFile("FIB_DATA.txt");
  parser.getCount();
  data = Data(parser.count());
  
  HorariObj obj;

  while(parser.readHorariObj(obj)){
    data.pushHorariObj(obj);
  }
  data.pushHorariObj(obj);
}

void App::computeSchedules()
{
  //Transform sets into vectors

  std::vector<std::string> mustAppear(mustAppearSubjects.size());
  int i = 0;
  for(const std::string& s : mustAppearSubjects)
  {
    mustAppear[i++] = s;
  }
  std::vector<std::string> other(otherSubjects.size());
  i = 0;
  for(const std::string& s : otherSubjects)
  {
    other[i++] = s;
  }
  std::vector<std::pair<std::string,int>> toExclude(groupsToExclude.size());
  i = 0;
  for(const std::pair<std::string,int>& p : groupsToExclude)
  {
    toExclude[i++] = p;
  }

  data.generateSchedules(sizeHorari,mixGroups,preference,maxPrintedSchedules,mustAppear,other,toExclude);

  outputFile.open(outputFilename, std::ios::out | std::ios::trunc);
  data.printSchedules(outputFile);
  outputFile.close();
  
  //Signal
  writtenSchedules();
}