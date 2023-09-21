#include "Data.hh"
#include <algorithm>
#include <fstream>
using namespace jaslo;


Data::Data(size_t size)
{
  _info.clear();
  _groups.clear();
  _subjectPermutations.clear();
  _perms.clear();
  _schedules.clear();
  
  _info.reserve(size);
}

Data::Data(const Data& other)
{
  _info = other._info;
  _groups = other._groups;
  _subjectPermutations = other._subjectPermutations;
  _perms = other._perms;
  _schedules = other._schedules;
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

void Data::generateSchedules(int subjectNumber, bool mixGroups, SchedulePreference preference, int maxPrintedSchedules,
const std::vector<std::string>& alwaysAppearSubjects, 
const std::vector<std::string>& otherSubjects, 
const std::vector<std::pair<std::string,int>>& subgroupsToExclude)
{
  _groups.clear();
  _subjectPermutations.clear();
  _perms.clear();
  _schedules.clear();

  std::cout << "generateSchedules start" << std::endl;

	std::vector<std::string> allRequestedSubjects(alwaysAppearSubjects.size()+otherSubjects.size());

	{
		int i = 0;
		for(int j = 0; j < alwaysAppearSubjects.size(); ++j)
		{
			allRequestedSubjects[i++] = alwaysAppearSubjects[j];
		}
		for(int j = 0; j < otherSubjects.size(); ++j)
		{
			allRequestedSubjects[i++] = otherSubjects[j];
		}
	}

	deleteNonRequestedGroups(allRequestedSubjects);
	joinGroups();
	deleteExcludedGroups(subgroupsToExclude);
	deleteRedundantGroups();
	makeGroups(mixGroups);
	makePermutations(subjectNumber,alwaysAppearSubjects,otherSubjects);
	makeSchedules(preference, maxPrintedSchedules);
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

void Data::makeSchedules(SchedulePreference preference, int maxPrintedSchedules)
{
  _schedules.clear();

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
    _schedules.push_back(h);
  }

  for(Horari& h : _schedules)
  {
    h.computeValue(preference);
  }

  sort(_schedules.begin(),_schedules.end());

  std::vector<Horari> newSchedules(maxPrintedSchedules);

  for(int i = 0; i < maxPrintedSchedules; ++i)
  {
    newSchedules[i] = _schedules[i];
  }

  _schedules = newSchedules;

  /*int printedSchedules = 0;

  for(const Horari& h : _schedules){
    h.print();
    std::cout << std::endl;
    if(++printedSchedules == maxPrintedSchedules) break;
  }
  std::cout << "Printed: " << printedSchedules << std::endl; 
  std::cout << "Total schedules: " << _schedules.size() << std::endl;*/
}

void Data::printSchedules(std::fstream& file)
{
  for(const Horari& h : _schedules) h.print(file);
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