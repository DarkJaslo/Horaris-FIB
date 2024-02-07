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
  sizeHorari = 5;
  preference = SchedulePreference::morning;
  mixGroups = false;
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

void App::receiveSelectionLists(QListWidget* listInclude, QListWidget* listAlways)
{
  this->listInclude = listInclude;
  this->listAlways = listAlways;
}

//Public slots

void App::setMajor(const QString& m)
{
  major = m.toStdString();

  clearSelection();
  setFilter(QString::fromStdString(filter));
}
void App::setSemester(const QString& s)
{ 
  if(semesters[0] == s.toStdString()) semester = 0;
  else semester = 1;

  changedSemester();
  clearSelection();
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

void App::moveFromMainList(QListWidgetItem* item)
{
  //item->setCheckState(Qt::CheckState::Unchecked);
  QListWidgetItem* newItem = new QListWidgetItem();
  newItem->setText(item->text());
  newItem->setCheckState(Qt::CheckState::Unchecked);

  otherSubjects.insert(item->text().toStdString());
  delete list->takeItem(list->row(item));
  //list->removeItemWidget(item);

  listInclude->addItem(newItem);
  listInclude->sortItems(Qt::SortOrder::AscendingOrder);
}

void App::moveFromIncludeList(QListWidgetItem* item)
{
  QListWidgetItem* newItem = new QListWidgetItem();
  newItem->setText(item->text());
  newItem->setCheckState(Qt::CheckState::Unchecked);

  if(item->checkState() == Qt::CheckState::Checked)
  {
    mustAppearSubjects.insert(item->text().toStdString());
    listAlways->addItem(newItem);
    listAlways->sortItems(Qt::SortOrder::AscendingOrder);
  }
  else{ //Never happens
    list->addItem(newItem);
    list->sortItems(Qt::SortOrder::AscendingOrder);
  }

  otherSubjects.erase(item->text().toStdString());
  delete listInclude->takeItem(listInclude->row(item));
}

void App::moveFromAlwaysList(QListWidgetItem* item)
{
  QListWidgetItem* newItem = new QListWidgetItem();
  newItem->setText(item->text());
  newItem->setCheckState(Qt::CheckState::Unchecked);

  mustAppearSubjects.erase(item->text().toStdString());
  delete listAlways->takeItem(listAlways->row(item));
  list->addItem(newItem);
  list->sortItems(Qt::SortOrder::AscendingOrder);
  setFilter(QString::fromStdString(filter));  
}

void App::generate()
{
  //TODO: a veces crashea, no veo el patrón
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

  std::cout << "pre generateSchedules" << std::endl;

  std::cout << "Must appear:\n"; 
  for(const std::string& s : mustAppear)
  {
    std::cout << s << " ";
  }

  std::cout << "Can appear:\n";
  for(const std::string& s : other)
  {
    std::cout << s << " ";
  }
  std::cout << "\n";

  data[semester].generateSchedules(sizeHorari,mixGroups,preference,maxPrintedSchedules,mustAppear,other,toExclude);

  std::cout << "outputfile open" << std::endl;

  outputFile.open(outputFilename, std::ios::out | std::ios::trunc);

  std::cout << "print schedules" << std::endl;

  data[semester].printSchedules(outputFile);

  std::cout << "outputfile close" << std::endl;
  outputFile.close();

  std::cout << "outputfile closed" << std::endl;
  
  //Signal
  writtenSchedules();
}

void App::initList()
{
  list->clear();
  for(const std::string& name : allSubjectNamesByMajor[semester][major])
  {
    if(otherSubjects.find(name) != otherSubjects.end() or mustAppearSubjects.find(name) != mustAppearSubjects.end())
      continue;
    
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

  for(int i = 0; i < auxNames.size(); ++i)
  {
    if(otherSubjects.find(auxNames[i]) != otherSubjects.end())
      use[i] = false;
    else if(mustAppearSubjects.find(auxNames[i]) != mustAppearSubjects.end())
      use[i] = false;
  }

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

void App::clearSelection()
{
  otherSubjects.clear();
  mustAppearSubjects.clear();
  listAlways->clear();
  listInclude->clear();
}