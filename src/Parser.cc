#include "Parser.hh"
#include "Horari.hh"
#include <string>
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
  _data = json::parse(_file);
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
  _count = _data["count"];
}

bool Parser::readHorariObj(HorariObj& o)
{
  o = HorariObj();
  json& cur = _data["results"][_curIndex];
  _curIndex++;
  if(!cur.contains("codi_assig")){
    printParseError("wrong codi_assig keyword");
  }
  o._code = cur["codi_assig"];

  if(!cur.contains("grup")){
    printParseError("wrong grup keyword");
  }
  try {
    o._group = std::stoi((std::string)cur["grup"]);
  } catch (std::invalid_argument&) {
    o._group = 0;
  }

  if(!cur.contains("dia_setmana")){
    printParseError("wrong dia_setmana keyword");
  }
  o._day = (Weekday)((int)cur["dia_setmana"] - 1);

  if(!cur.contains("inici")){
    printParseError("wrong inici keyword");
  }
  if(!cur.contains("durada")){
    printParseError("wrong durada keyword");
  }
  std::string hour = cur["inici"];
  int h = 0;
  int index = 0;
  while(hour[index] != ':')
  {
    h*=10;
    h += hour[index]-'0';
    index++;
  }
  o._hours.push_back(h);
  int durada = cur["durada"];
  durada--; //The first hour is already included
  int counter = 1;
  while(counter <= durada)
  {
    o._hours.push_back(h+counter);
    counter++;
  }

  if(!cur.contains("tipus")){
    printParseError("wrong tipus keyword");
  }
  std::string aux = cur["tipus"];
  if(aux == "T") o._type = ClassType::theory;
  else if(aux == "L") o._type = ClassType::lab;
  else o._type = ClassType::problems;


  return _curIndex < _count;
}


//Private parser

void Parser::printParseError(const std::string& extraInfo = "")
{
  std::cerr << "The parser has found a problem / unexpected input in FIB_DATA.txt. Extra info: " << extraInfo << std::endl;
  exit(1);
}
