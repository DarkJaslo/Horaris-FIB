#include "App.hh"
#include <sstream>
#include <thread>
using namespace jaslo;

//Public

App::App(QWidget* parent) : QWidget(parent)
{
  setFocusPolicy(Qt::StrongFocus); //Can get keyboard events
  data = std::vector<Data>(2);
  allSubjectNamesByMajor = std::vector<std::map<std::string,std::vector<std::string>>>(2);
  majors = std::vector<std::vector<std::string>>(2);
  major = "GEI";
  filter = "";
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

  initStudiesBox();
  initList();
}

void App::receiveListWidget(QListWidget* listWidget)
{
  this->list = listWidget;
}

void App::receiveStudiesBox(MyComboBox* box)
{
  studiesBox = box;
}

//Public slots

void App::setMajor(const QString& m)
{
  major = m.toStdString();

  setFilter(QString::fromStdString(filter));
}
void App::setSemester(const QString& s)
{ 
  if(semesters[0] == s.toStdString()) semester = 0;
  else semester = 1;

  changedSemester();
  setFilter(QString::fromStdString(filter));
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

void App::setFilter(const QString& filt)
{
  if(filt.size() == 0)
  {
    filter = filt.toStdString();
    initList();
  }
  else{
    std::string stdFilter = filt.toStdString();
    for(char& c : stdFilter)
    {
      if(c >= 'a' and c <= 'z')
      {
        c-=('a'-'A');
      }
    }
    filter = stdFilter;
    initList(filter);
  }
}

void App::generate()
{
  computeSchedules();
}




//Private

void App::parseData(int index, const std::string& filename)
{
  Parser parser;
  parser.openFile(filename);
  parser.getCount();
  data[index] = Data(parser.count());
  std::set<std::string> names;
  std::set<std::string> allMajors;
  
  HorariObj obj;

  while(parser.readHorariObj(obj)){
    data[index].pushHorariObj(obj);
    names.insert(obj.code());
  }

  data[index].pushHorariObj(obj);
  names.insert(obj.code());

  parser.closeFile();

  std::cout << "parsed " << index << std::endl; 

  allSubjectNamesByMajor[index].clear();

  //allSubjectNames[index].clear();
  //allSubjectNames[index].reserve(names.size());
  for(const std::string& name : names)
  {
    //Analyze name
    std::string auxMajor = "GEI";

    if(name.length() >= 3 and name.substr(0,3) == "AL-") //Aula lliure subject
    {
      auxMajor = "AL";
    }
    else{
      int i = 0;
      while(i < name.length())
      {
        if(name[i] == '-')
        {
          auxMajor = name.substr(i+1,name.length()-i+1);
          break;
        }
        ++i;
      }      
    }
    if(name == "MENTORIES" or name == "RESERVAT") continue;
    allSubjectNamesByMajor[index][auxMajor].push_back(name);

    allMajors.insert(auxMajor);
  }
  
  for(const std::string& maj : allMajors)
  {
    majors[index].push_back(maj);
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
  for(const std::string& name : allSubjectNamesByMajor[semester][major])
  {
    QListWidgetItem *item = new QListWidgetItem;
    item->setText(QString::fromStdString(name));
    item->setCheckState(Qt::CheckState::Unchecked);

    list->addItem(item);
  }
}

void App::initList(const std::string& filter)
{
  std::vector<std::string> auxNames = allSubjectNamesByMajor[semester][major];
  std::vector<bool> use(auxNames.size(),true);

  for(int i = 0; i < filter.length(); ++i)
  {
    for(int j = 0; j < auxNames.size(); ++j)
    {
      if(use[j] and (auxNames[j].length() < i+1 or auxNames[j][i] != filter[i]))
      {
        use[j] = false;
      }
    }
  }

  list->clear();

  int counter = 0;
  for(const std::string& name : auxNames)
  {
    if(not use[counter++]) continue;
    QListWidgetItem *item = new QListWidgetItem;
    item->setText(QString::fromStdString(name));
    //TODO: properly manage checkstate
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

void App::initStudiesBox()
{
  studiesBox->clear();

  for(const std::string& study : majors[semester])
  {
    studiesBox->addOption(QString::fromStdString(study));
  }

  studiesBox->setCurrentIndex(studiesBox->findText(QString("GEI")));
}