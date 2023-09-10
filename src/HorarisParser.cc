#include "HorarisParser.hh"
#include <algorithm>
using namespace jaslo;

//Data

Data::Data(size_t size)
{
  _info.reserve(size);
}

HorariObj& Data::operator[](int i)
{
  if(i >= _info.size()){ std::__throw_out_of_range("ERROR: tried to access Data with too big of an index."); }
  else if(i < 0){ std::__throw_out_of_range("ERROR: tried to access Data with a negative index"); }
  return _info[i];
}

const HorariObj& Data::operator[](int i) const
{
  if(i >= _info.size()){ std::__throw_out_of_range("ERROR: tried to access Data with too big of an index."); }
  else if(i < 0){ std::__throw_out_of_range("ERROR: tried to access Data with a negative index"); }
  return _info[i];
}

void Data::pushHorariObj(const HorariObj& o)
{
  _info.emplace_back(o);
}

void Data::print()
{
  for(const HorariObj& o : _info){
    std::cout << o << std::endl;
  }
}

int Data::firstOccurrence(const std::string& subjectName)const
{
  int index = binarySearch(0,_info.size()-1, subjectName);
  while(index > 0 and _info[index-1].code() == subjectName){
    index--;
  }
  return index;
}

std::vector<std::string> Data::names() const
{
  std::vector<std::string> names;
  std::string aux = "this will never be a name for a subject, but who knows, so let's make it as difficult as possible!";
  for(const HorariObj& o : _info)
  {
    if(o.code() != aux){
      names.push_back(o.code());
      aux = o.code();
    }
  }
  return names;
}

void Data::deleteNonRequestedGroups(const std::vector<std::string>& names)
{
  std::vector<int> indicesToKeep;
  for(int i = 0; i < _info.size(); ++i)
  {
    if(contains(names,_info[i].code()))
    {
      indicesToKeep.push_back(i);
    }
  }

  std::vector<HorariObj> newInfo(indicesToKeep.size());
  for(int i = 0; i < newInfo.size(); ++i)
  {
    newInfo[i] = _info[indicesToKeep[i]];
  }

  _info = newInfo;
}

void Data::joinGroups()
{
  std::vector<bool> ignore(_info.size(),false);
  int ignoredGroups = 0;
  int i = 0;

  while(i < _info.size())
  {
    if(ignore[i]){
      ++i;
      continue;
    }
    HorariObj cur = _info[i];
    
    for(int j = i+1; j < _info.size(); ++j)
    {
      if(ignore[j]) continue;
      HorariObj subCur = _info[j];
      if(cur.code() == subCur.code() and cur.group() == subCur.group() and cur.day() == subCur.day()){
        ignore[j] = true;
        ignoredGroups++;
        for(int h : subCur.hours()) _info[i]._hours.push_back(h);
      }
    }

    ++i;
  }

  std::vector<HorariObj> newInfo(_info.size()-ignoredGroups);
  i = 0;
  int j = 0;
  while(i < _info.size())
  {
    if(not ignore[i]){
      newInfo[j] = _info[i];
      ++j;
    }
    ++i;
  }

  _info = newInfo;
}

void Data::deleteRedundantGroups()
{
  std::vector<bool> toDelete(_info.size(),false);
  int deletedGroups = 0;

  for(int i = 0; i < _info.size(); ++i)
  {
    if(toDelete[i]) continue;
    HorariObj cur = _info[i];

    for(int j = i+1; j < _info.size(); ++j)
    {
      if(toDelete[j]) continue;
      HorariObj nextCur = _info[j];
      if(cur.code() != nextCur.code()) break;
      if(cur.equivalent(nextCur))
      {
        toDelete[j] = true;
        ++deletedGroups;
      }
    }
  }

  std::vector<HorariObj> newInfo(_info.size()-deletedGroups);
  int i,j;
  i = 0; j = 0;
  while(i < _info.size())
  {
    if(not toDelete[i]){
      newInfo[j] = _info[i];
      ++j;
    }
    ++i;
  }
  _info = newInfo;
}

void Data::deleteExcludedGroups(const std::vector<std::pair<std::string,int>>& groups)
{
  std::vector<bool> indicesToDelete(_info.size(),false);
  int toDelete = 0;

  for(int i = 0; i < groups.size(); ++i)
  {
    std::string subject = groups[i].first;
    int group = groups[i].second;
    int index = firstOccurrence(subject);
    if(index != -1)
    {
      for(int j = index; j < _info.size(); ++j)
      {
        if(_info[j].group() == group)
        {
          indicesToDelete[j] = true;
          toDelete++;
          break;
        }
      }
    }
    
  }

  std::vector<HorariObj> newInfo(_info.size()-toDelete);
  int j = 0;
  for(int i = 0; i < _info.size(); ++i){
    if(not indicesToDelete[i]){
      newInfo[j] = _info[i];
      ++j;
    }
  }

  _info = newInfo;
}

void Data::makeGroups(bool mix)
{
  std::vector<std::string> names;
  std::string current = "Not a subject";
  for(int i = 0; i < _info.size(); ++i)
  {
    if(_info[i].code() != current)
    {
      current = _info[i].code();
      if(not contains(names, current)) names.push_back(current);
    }
  }

  for(int i = 0; i < names.size(); ++i)
  {
    std::string current = names[i];

    _groups.push_back(std::pair<std::string,std::vector<std::vector<int>>>());
    _groups[i].first = current;

    //Add indices of HorariObjs of each possible group. Add more if mix == true

    bool hasTheory = false;
    int index = firstOccurrence(current);
    std::vector<int> theoryGroups;
    std::vector<int> extraTheoryGroups;

    for(int j = index; j < _info.size(); ++j)
    {
      if(_info[j].code() != current) break;
      else if(_info[j].group()%10 == 0){
        hasTheory = true;

        bool found = false;
        for(int aux : theoryGroups) if(aux == _info[j].group()) found = true;
        if(found) extraTheoryGroups.push_back(j);
        else theoryGroups.push_back(_info[j].group());
      }
    }

    if(hasTheory){

      std::vector<bool> used(theoryGroups.size(),false);
      

      //For each theory group, add subgroups
      for(int k = index; k < _info.size(); ++k)
      {
        if(_info[k].code() != current) break;

        //Find one theory group
        if(_info[k].group()%10 == 0)
        {
          bool isUsed = false;

          //Save theory groups and extra theory groups
          for(int aux = 0; aux < theoryGroups.size(); ++aux)
          {
            if(theoryGroups[aux] == _info[k].group()){
              if(used[aux]){
                isUsed = true;
                break;
              }
              else{
                used[aux] = true;
                break;
              }
            }
          }
          if(isUsed) continue;

          int group = _info[k].group();
          std::vector<int> subgroups; //How many subgroups are there?

          //Count and store subgroups
          for(int l = index; l < _info.size(); ++l)
          {
            if(_info[l].code() != current) break;
            if(_info[l].group()%10==0) continue;
            if(mix or (abs(_info[l].group()-group) < 5))
            {
              bool found = false;
              for(int m = 0; m < subgroups.size(); ++m)
              {
                if(subgroups[m] == _info[l].group()){
                  found = true;
                  break;
                }
              }
              if(not found) subgroups.push_back(_info[l].group());
            }
          }

          //Add permutations one subgroup at a time
          for(int m = 0; m < subgroups.size(); ++m)
          {
            std::vector<int> indices;
            indices.push_back(k); //Push back theory group
            for(int extraGroup : extraTheoryGroups)
            {
              if(_info[extraGroup].group() == _info[k].group()){
                indices.push_back(extraGroup);
              }
            }
            for(int l = index; l < _info.size(); ++l)
            {
              if(_info[l].code() != current) break;
              if(_info[l].group() == subgroups[m]){
                indices.push_back(l);
              }
            }
            _groups[i].second.push_back(indices);
          }

          //Subject only has a theory group or something (AC2 does this I think)
          if(subgroups.size() == 0) 
          {
            std::vector<int> indices;
            indices.push_back(k); //Push back theory group
            for(int l = index; l < _info.size(); ++l)
            {
              if(_info[l].code() != current) break;
              if(_info[l].group() == group and l != k) indices.push_back(l);
            }
            _groups[i].second.push_back(indices);
          }
        }
      }
    }
    else{ //No theory, so no mixing
      std::vector<int> subgroups;
      for(int k = index; k < _info.size(); ++k)
      {
        if(_info[k].code() != current) break;
        int group = _info[k].group();
        bool found = false;
        for(int l = 0; l < subgroups.size(); ++l)
        {
          if(subgroups[l] == group){
            found = true;
            break;
          }
        }
        if(not found){
          subgroups.push_back(group);
        }
      }

      for(int k = 0; k < subgroups.size(); ++k)
      {
        std::vector<int> indices;
        for(int l = index; l < _info.size(); ++l)
        {
          if(_info[l].group() == subgroups[k]) indices.push_back(l);
        }
        _groups[i].second.push_back(indices);
      }
    }
  }
}

void Data::makePermutations(int subjNum, const std::vector<std::string>& always, const std::vector<std::string>& include)
{
  //Permutations of "ALWAYS:" subjects
  std::vector<std::vector<int>> compulsoryPermutations;
  {
    
    std::vector<int> permutation(always.size());
    std::vector<int> options;

    for(int i = 0; i < always.size(); ++i)
    {
      for(int j = 0; j < _groups.size(); ++j)
      {
        if(_groups[j].first == always[i]){
          options.push_back(j);
          break;
        }
      }
    }

    std::vector<bool> used(options.size(),false);
    onePerGroupPermutations(0,0,options,used,permutation,compulsoryPermutations);
  }

  //Permutations of "INCLUDE:" subjects
  std::vector<std::vector<int>> includePermutations;
  {
    std::vector<int> permutation(subjNum-always.size());
    std::vector<int> options;
    for(int i = 0; i < include.size(); ++i)
    {
      for(int j = 0; j < _groups.size(); ++j)
      {
        if(_groups[j].first == include[i]){
          options.push_back(j);
          break;
        }
      }
    }

    std::vector<bool> used(options.size(), false);
    onePerGroupPermutations(0,0,options,used,permutation,includePermutations);
  }

  //Join permutations in _subjectPermutations (class member)
  if(compulsoryPermutations.size() == 0)
  {
    _subjectPermutations = includePermutations; 
  }
  else if(includePermutations.size() == 0)
  {
    _subjectPermutations = compulsoryPermutations;
  }
  else
  {
    _subjectPermutations = std::vector<std::vector<int>>(compulsoryPermutations.size()*includePermutations.size(), std::vector<int>(compulsoryPermutations[0].size()+includePermutations[0].size()));

    int cur = 0;
    for(int i = 0; i < _subjectPermutations.size(); i += includePermutations.size())
    {
      copyVector(compulsoryPermutations[cur],includePermutations,_subjectPermutations,i);
      ++cur;
    }
  }

  //Make permutations 

  for(int i = 0; i < _subjectPermutations.size(); ++i)
  {
    //Select one permutation
    std::vector<int> groupsPerSubject(_subjectPermutations[0].size());
    for(int j = 0; j < _subjectPermutations[i].size(); ++j)
    {
      groupsPerSubject[j] = _groups[_subjectPermutations[i][j]].second.size();
    }
    std::vector<std::vector<int>> permutations;
    std::vector<int> current(subjNum);

    //Make all permutations of groups in the subjects of that one permutation
    manyPerGroupPermutations(0,groupsPerSubject,current,permutations);

    //Add them to _perms
    for(int j = 0; j < permutations.size(); ++j)
    {

      std::vector<std::pair<int,int>> subjectsAndGroups(subjNum);
      for(int k = 0; k < subjNum; ++k)
      {
        subjectsAndGroups[k] = std::make_pair(_subjectPermutations[i][k],permutations[j][k]);
      }
      _perms.push_back(subjectsAndGroups);
    }
  }
}

void Data::makeAndPrintSchedules(SchedulePreference preference, int maxPrintedSchedules)
{
  std::vector<Horari> hs;

  for(int i = 0; i < _perms.size(); ++i)
  {
    Horari h;
    bool ok = true;
    for(int j = 0; j < _perms[0].size(); ++j)
    {
      std::vector<HorariObj> subjectGroup;

      for(int k = 0; k < _groups[_perms[i][j].first].second[_perms[i][j].second].size(); ++k)
      {
        subjectGroup.push_back(_info[_groups[_perms[i][j].first].second[_perms[i][j].second][k]]);
      }

      if(h.doesItFit(subjectGroup))
      {
        h.addSubject(subjectGroup);
        //h.print();
      }
      else{
        ok = false;
        break;
      }
    }
    if(not ok) continue;
    hs.push_back(h);
  }

  for(Horari& h : hs)
  {
    h.computeValue(preference);
  }

  sort(hs.begin(),hs.end());

  int printedSchedules = 0;

  for(const Horari& h : hs){
    h.print();
    std::cout << std::endl;
    if(++printedSchedules == maxPrintedSchedules) break;
  }
  std::cout << "Printed: " << printedSchedules << std::endl; 
  std::cout << "Total schedules: " << hs.size() << std::endl;
}


//Private Data

int Data::binarySearch(int l, int r, const std::string& thing) const
{
  if(l > r) return -1;
  int m = (l+r)/2;
  if(_info[m].code() > thing)
    return binarySearch(l,m-1,thing);
  else if(_info[m].code() < thing)
    return binarySearch(m+1,r,thing);
  return m;
}

bool Data::contains(const std::vector<std::string>& names, const std::string& name)
{
  for(const std::string& s : names)
  {
    if(s == name) return true;
  }
  return false;
}

void Data::onePerGroupPermutations(int minUsed, int cur, const std::vector<int>& options, std::vector<bool>& used, std::vector<int>& current, std::vector<std::vector<int>>& allPermutations)
{
  if(cur == current.size()) allPermutations.push_back(current);
  else
  {
    for(int i = cur; i < options.size(); ++i){
      if(used[i] or i < minUsed) continue;
      minUsed = i;
      current[cur] = options[i];
      used[i] = true;
      onePerGroupPermutations(minUsed, cur+1, options, used, current ,allPermutations);
      used[i] = false;
    }
  }
}

void Data::manyPerGroupPermutations(int cur, const std::vector<int>& groupsPerSubject, std::vector<int>& current, std::vector<std::vector<int>>& allPermutations)
{
  if(cur == current.size()){
    allPermutations.push_back(current);
  }
  else
  {
    for(int j = 0; j < groupsPerSubject[cur]; ++j)
    {
      current[cur] = j;
      manyPerGroupPermutations(cur+1,groupsPerSubject,current,allPermutations);
    }
  }
}

void Data::copyVector(const std::vector<int>& source1, const std::vector<std::vector<int>>& source2, std::vector<std::vector<int>>& dest, int loc)
{
  for(int i = 0; i < source2.size(); ++i){
    int j;
    for(j = 0; j < source1.size(); ++j){
      dest[loc+i][j] = source1[j];
    }
    int offset = j;
    while(j-offset < source2[i].size()){
      dest[loc+i][j] = source2[i][j-offset];
      ++j;
    }
  }
}


//Parser

Parser::Parser()
{
  _curIndex   = 0;
  _count      = 0;
  _openedFile = false;
}

int Parser::count()const{  return _count; }

void Parser::openFile(const std::string& filename)
{
  _file.open(filename.c_str());
  _openedFile = true;
}

void Parser::getCount()
{
  if(not _openedFile){
    std::cerr << "Trying to get the number of groups without opening a file" << std::endl;
    exit(1);
  }
  while(not _file.eof())
  {
    std::string line;
    getline(_file,line);
    std::istringstream iss(line);
    std::string aux;
    iss >> aux;
    
    if(aux.length() < 7) continue;
    if(aux[0] == '"')
    {
      if(aux.substr(1,5) == "count")
      {
        iss >> _count;
        getline(_file,line); //Leaves everything ready
        return;
      }
    }
  }
}

bool Parser::readHorariObj(HorariObj& o)
{
  o = HorariObj();
  std::string        line;
  std::istringstream iss;
  std::string        aux;
  std::string        constant;
  getline(_file,line); //  Reads {

  getline(_file,line); //  Reads "codi_assig"
  iss = std::istringstream(line);
  iss >> aux;
  constant = "codi_assig";
  if(isCorrect(constant.length()+3,constant.length(),aux,constant))
  {
    iss >> aux;
    o._code = aux.substr(1,aux.length()-3);
  }
  else{
    printParseError("wrong codi_assig keyword");
  }

  getline(_file,line); //  Reads "grup"
  iss = std::istringstream(line);
  iss >> aux;
  constant = "grup";
  if(isCorrect(constant.length()+3,constant.length(),aux,constant))
  {
    iss >> aux;
    if(aux.substr(1,aux.length()-3).length() <= 2) 
      o._group = stoi(aux.substr(1,aux.length()-3));
    else o._group = 0;
  }
  else{
    printParseError("wrong grup keyword");
  }

  getline(_file,line); //  Reads "dia_setmana"
  iss = std::istringstream(line);
  iss >> aux;
  constant = "dia_setmana";
  if(isCorrect(constant.length()+3,constant.length(),aux,constant))
  {
    int auxint;
    iss >> auxint;
    const Weekday totsDies[5] = {Weekday::monday,Weekday::tuesday,Weekday::wednesday,Weekday::thursday,Weekday::friday};
    o._day = totsDies[auxint-1];
  }
  else{
    printParseError("wrong dia_setmana keyword");
  }

  getline(_file,line); //  Reads "inici"
  iss = std::istringstream(line);
  iss >> aux;
  constant = "inici";
  if(isCorrect(constant.length()+3,constant.length(),aux,constant))
  {
    iss >> aux;
    std::string hour = aux.substr(1,aux.length()-3);
    int h = 0;
    int index = 0;
    while(hour[index] != ':')
    {
      h*=10;
      h += hour[index]-'0';
      index++;
    }
    o._hours.push_back(h);

    getline(_file,line); //  Reads "durada"
    iss = std::istringstream(line);
    iss >> aux;
    constant = "durada";
    if(isCorrect(constant.length()+3,constant.length(),aux,constant))
    {
      int durada;
      iss >> durada;
      durada--; //The first hour is already included
      int counter = 1;
      while(counter <= durada)
      {
        o._hours.push_back(h+counter);
        counter++;
      }
    }
    else{
      printParseError("wrong durada keyword");
    }
  }
  else{
    printParseError("wrong inici keyword");
  }

  
  getline(_file,line); //  Reads "tipus"
  iss = std::istringstream(line);
  iss >> aux;
  constant = "tipus";
  if(isCorrect(constant.length()+3,constant.length(),aux,constant))
  {
    iss >> aux;
    aux = aux.substr(1,aux.length()-3);
    if(aux == "T") o._type = ClassType::theory;
    else if(aux == "L") o._type = ClassType::lab;
    else o._type = ClassType::problems;
  }
  else{
    printParseError("wrong tipus keyword");
  }


  getline(_file,line); //  Reads "aules"
  getline(_file,line); //  Reads "idioma"
  getline(_file,line); //  Reads "},"
  iss = std::istringstream(line);
  iss >> aux;
  return aux == "},";
}


//Private parser

bool Parser::isCorrect(int expectedSize, int substrSize, const std::string& str, const std::string& substr)const
{
  if(str.length() < expectedSize) return false;
  return str.substr(1,substrSize) == substr;
}

void Parser::printParseError(const std::string& extraInfo = "")
{
  std::cerr << "The parser has found a problem / unexpected input in FIB_DATA.txt. Extra info: " << extraInfo << std::endl;
  exit(1);
}