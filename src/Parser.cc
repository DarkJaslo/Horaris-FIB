#include "Parser.hh"
using namespace jaslo;

//Parser

Parser::Parser()
{
  _curIndex   = 0;
  _count      = 0;
  _openedFile = false;
}

Parser::~Parser()
{
  closeFile();
}

int Parser::count()const{  return _count; }

void Parser::openFile(const std::string& filename)
{
  closeFile();
  _file.open(filename.c_str());
  _openedFile = true;
}

void Parser::closeFile()
{
  if(_openedFile and _file.is_open())
  {
    _file.close();
  }
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