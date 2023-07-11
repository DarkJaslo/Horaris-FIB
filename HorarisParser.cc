#include "HorarisParser.hh"
using namespace jaslo;


//HorariObj
HorariObj::HorariObj()
{

}

std::string             HorariObj::code()  const { return _code;  }
int                     HorariObj::group() const { return _group; }
DiaSetmana              HorariObj::day()   const { return _day;   }
const std::vector<int>  HorariObj::hours() const { return _hours; }
std::string             HorariObj::type()  const { return _type;  }






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

int Data::firstOccurrence(const std::string& assignatura)const
{
  int index = binarySearch(0,_info.size()-1, assignatura);
  while(index > 0 and _info[index-1].code() == assignatura){
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

std::vector<std::vector<HoraClasse>> Data::allAssignatures() const
{
  std::vector<std::vector<HoraClasse>> result;
  std::string currentSubj = "this will never be a name for a subject, but who knows, so let's make it as difficult as possible!";
  std::vector<int> currentTheoryIndices;
  bool foundNonTheory = false;

  //std::cout << "antes del i" << std::endl;

  for(int i = 0; i < _info.size(); ++i)
  {
    //std::cout << "vuelta bucle i" << std::endl;
    //i > 0 and foundNonTheory and (
    if(i == 0){
      currentSubj = currentSubj = _info[i].code();
    }
    else if(_info[i].code() != currentSubj or i == _info.size()-1)
    {
      //std::cout << "entra primer if" << std::endl;

      if(i == _info.size()-1){
        if(_info[i].type() == "T" or _info[i].group()%10==0){
          currentTheoryIndices.push_back(i);
        }
      }

      //Look for every non-theory group of this subject and add these hours
      //if it finds nothing, create a new subject with just the theory hours

      bool foundOne = false;
      for(int j = 0; j < result.size(); ++j)
      {
        //std::cout << "vuelta bucle j" << std::endl;
        if(result[j][0].assignatura() == currentSubj){
          foundOne = true;

          for(int k = 0; k < currentTheoryIndices.size(); ++k)
          {
            int index = currentTheoryIndices[k];
            if(abs(_info[index].group()-result[j][0].grup()) > 5) continue;
            for(int t = 0; t < _info[index].hours().size(); ++t)
            {
              HoraClasse hcl(_info[index].hours()[t],result[j][0].grup(),TipusClasse::teoria,_info[index].day(),false,_info[index].code());
              result[j].push_back(hcl);
            }
          }
        }
      }

      if(not foundOne)
      {
        int finalIndex = result.size();
        result.push_back(std::vector<HoraClasse>());
        for(int k = 0; k < currentTheoryIndices.size(); ++k)
        {
          int index = currentTheoryIndices[k];
          for(int t = 0; t < _info[index].hours().size(); ++t)
          {
            HoraClasse hcl(_info[index].hours()[t],_info[index].group(),TipusClasse::teoria,_info[index].day(),false,_info[index].code());
            result[finalIndex].push_back(hcl);
          }
        }
        
      }


      if(i == _info.size()-1){
        break;
      }

      currentSubj = _info[i].code();
      currentTheoryIndices.clear();
    }

    if(_info[i].type() == "T" or _info[i].group()%10 == 0 ){
      //std::cout << "es T, " << _info[i].code() << std::endl;
      currentTheoryIndices.push_back(i);
    }
    else{

      if(not foundNonTheory) foundNonTheory = true;

      //std::cout << "no es T, " <<  _info[i].code()  << std::endl;
      //Generate the HoraClasses
      std::vector<HoraClasse> hcls;
      for(int k = 0; k < _info[i].hours().size(); ++k){
        HoraClasse hcl(_info[i].hours()[k],_info[i].group(),TipusClasse::laboratori,_info[i].day(),false,_info[i].code());
        hcls.push_back(hcl);
      }

      //std::cout << "paso primer for" << std::endl;
      



      //Put into a vector
      bool found = false;
      for(int j = 0; j < result.size(); ++j){
        //std::cout << "vuelta bucle j interno" << std::endl;
        if(result[j][0].assignatura() == _info[i].code() and result[j][0].grup() == _info[i].group()){
          for(const HoraClasse& hcl : hcls){
            result[j].push_back(hcl);
          }
          found = true;
          break;
        }
      }
      if(not found){
        int index = result.size();
        result.push_back(std::vector<HoraClasse>());
        for(const HoraClasse& hcl : hcls){
          result[index].push_back(hcl);
        }
      }
    }
  }

  return result;
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
    const DiaSetmana totsDies[5] = {DiaSetmana::dilluns,DiaSetmana::dimarts,DiaSetmana::dimecres,DiaSetmana::dijous,DiaSetmana::divendres};
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
    o._type = aux.substr(1,aux.length()-3);
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
  std::cerr << "A parsing error has occurred. Extra info: " << extraInfo << std::endl;
  exit(1);
}