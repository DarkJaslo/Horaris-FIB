#include "App.hh"
#include <sstream>
#include <thread>
using namespace jaslo;

//Public

App::App(QWidget* parent) : QWidget(parent)
{
  setFocusPolicy(Qt::StrongFocus); //Can get keyboard events
  data = std::vector<Data>(2);
  allSubjectNames = std::vector<std::vector<std::string>>(2);
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

  semesters[0] = semest;
  semesters[1] = aux;

  emit loadSemester(QString::fromStdString(semest));
  emit loadSemester(QString::fromStdString(aux));
}

void App::getData()
{
  std::thread th1(&App::getDataTask,this,0);
  std::thread th2(&App::getDataTask,this,1);

  th1.join();
  th2.join();

  initList();
}

void App::getStudies()
{

}

void App::receiveListWidget(QListWidget* listWidget)
{
  this->list = listWidget;
}

//Public slots

void App::setSemester(const QString& s)
{ 
  if(semesters[0] == s.toStdString()) semester = 0;
  else semester = 1;

  initList();
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

void App::parseData(int index, const std::string& filename)
{
  parser.openFile(filename);
  parser.getCount();
  data[index] = Data(parser.count());
  std::set<std::string> names;
  
  HorariObj obj;

  while(parser.readHorariObj(obj)){
    data[index].pushHorariObj(obj);
    names.insert(obj.code());
  }

  data[index].pushHorariObj(obj);
  names.insert(obj.code());

  parser.closeFile();

  allSubjectNames[index].clear();
  allSubjectNames[index].reserve(names.size());
  for(const std::string& name : names)
  {
    allSubjectNames[index].emplace_back(name);
  }
  
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

  data[dataIndex].generateSchedules(sizeHorari,mixGroups,preference,maxPrintedSchedules,mustAppear,other,toExclude);

  outputFile.open(outputFilename, std::ios::out | std::ios::trunc);
  data[dataIndex].printSchedules(outputFile);
  outputFile.close();
  
  //Signal
  writtenSchedules();
}

void App::initList()
{
  list->clear();
  for(const std::string& name : allSubjectNames[semester])
  {
    QListWidgetItem *item = new QListWidgetItem;
    item->setText(QString::fromStdString(name));
    item->setCheckState(Qt::CheckState::Unchecked);

    list->addItem(item);
  }
}

void App::sortList()
{
  list->sortItems(Qt::SortOrder::AscendingOrder);
}

void App::getDataTask(int semest)
{
  std::cout << "exec with semest " << semest << std::endl;
  std::string path;
  path = path1;
  path.append(semesters[semest]);
  path.append(path2);

  std::string filename = "FIB_DATA";
  if(semest == 0) filename.append("_0.txt");
  else filename.append("_1.txt");

  if(not HTTPSGetter::get(url,path,filename)) exit(EXIT_FAILURE);

  parseData(semest,filename);
}