#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fcntl.h>
#include <cstdlib>
#include <ctime>
#include <regex>
#include "HorarisParser.hh"
#include "Horari.hh"

/*
  Schedule generator for FIB-UPC. Downloads data from https://api.fib.upc.edu.
*/

using namespace jaslo;
using namespace std;

#define MAX_PRINTED_SCHEDULES 1000 //Change to print more if needed. Aims to avoid more than 500kB/file

bool isNumber(const std::string& str) {
  // Define the regex pattern for a number
  std::regex numRegex("^[0-9]*$");
  // Match the input string against the regex pattern
  return std::regex_match(str, numRegex);
}

int main(int argc, char** argv)
{
  if(argc > 1)
  {
    std::cerr << "Usage: ./main.exe" << std::endl;
    std::cerr << "This program has no parameters. The typical command you'd want to run would be < my_input_file.txt > my_output_file.txt" << std::endl;
    exit(1);
  }

  std::time_t currentTime = std::time(nullptr);

  // Convert the current time to a struct tm
  std::tm* currentTm = std::localtime(&currentTime);

  // Extract the date components from the struct tm
  int year = currentTm->tm_year + 1900;
  int month = currentTm->tm_mon + 1;
  int day = currentTm->tm_mday;
  int hour = currentTm->tm_hour;
  int minute = currentTm->tm_min;
  int second = currentTm->tm_sec;

  //Read the "SEMESTER"
  std::string semester;
  std::cin >> semester;
  std::cin >> semester;

  char buffer[256];
  sprintf(buffer,"python3 get.py %s > FIB_DATA.txt",semester.c_str());

  //Executes python program that gets the data
  if(std::system(buffer) != 0)
  {
    std::cerr << "Error: get.py script couldn't be executed or ran into an error" << std::endl;
    exit(1);
  }

  Parser parser;
  parser.openFile("FIB_DATA.txt");
  parser.getCount();
  Data data(parser.count());
  
  HorariObj obj;

  while(parser.readHorariObj(obj)){
    data.pushHorariObj(obj);
  }
  data.pushHorariObj(obj);

  int sizeHorari;
  { //Read the "SUBJECTS_IN_SCHEDULE"
    std::string numberOfSubjects;
    cin >> numberOfSubjects;
    cin >> sizeHorari;
  }

  bool mixGroups = false;
  { //Read the "MIX_GROUPS"
    std::string mixGroupsString;
    cin >> mixGroupsString;
    std::string doMixGroups;
    cin >> doMixGroups;
    if(doMixGroups == "true") mixGroups = true;
    else if(doMixGroups != "false"){
      std::__throw_invalid_argument("Wrong input. Expected 'true' or 'false' after 'MIX_GROUPS:'");
    }
  }

  SchedulePreference preference = SchedulePreference::dont_care;

  { //Read the "PREFER"
    std::string preferString;
    cin >> preferString;
    cin >> preferString;
    if(preferString == "morning") preference = morning;
    else if(preferString == "afternoon") preference = afternoon;
  }

  vector<string> nomsObligatories;
  vector<string> nomsAssignatures;
  vector<pair<string,int>> groupsToExclude;

  //Fill names
  string auxAssig;
  cin >> auxAssig;  //Read the "ALWAYS:"
  cin >> auxAssig;
  while(auxAssig != "END_INPUT"){
    nomsObligatories.push_back(auxAssig);
    cin >> auxAssig;
  }

  cin >> auxAssig;  //Read the "INCLUDE:"
  cin >> auxAssig;
  while(auxAssig != "END_INPUT"){
    nomsAssignatures.push_back(auxAssig);
    cin >> auxAssig;
  }

  cin >> auxAssig;  //Read the "EXCLUDE:"
  cin >> auxAssig;
  std::string actAssig = "not really a subject";
  while(auxAssig != "END_INPUT")
  {
    if(isNumber(auxAssig)){

      if(actAssig != "not really a subject"){
        pair<string,int> item = make_pair(actAssig,stoi(auxAssig));
        groupsToExclude.push_back(item);
      }
    }
    else{
      actAssig = auxAssig;
    }
    cin >> auxAssig;
  }

  vector<string> names(nomsObligatories.size()+nomsAssignatures.size());
  for(const string& s : nomsObligatories) names.emplace_back(s);
  for(const string& s : nomsAssignatures) names.emplace_back(s);

  data.deleteNonRequestedGroups(names);
  data.joinGroups();
  data.deleteExcludedGroups(groupsToExclude);
  data.deleteRedundantGroups();
  data.makeGroups(mixGroups);
  data.makePermutations(sizeHorari,nomsObligatories,nomsAssignatures);

  cout << "Printing schedules for " << semester << "   ";
  std::cout << "Current Date: " << year << "-" << month << "-" << day << ", ";
  
  std::cout << hour << ":";
  if(minute < 10) std::cout << '0';
  std::cout << minute << ":";
  if(second < 10) std::cout << '0';
  std::cout << second << std::endl << std::endl;
  std::cout << "Always appear:";

  for(const std::string& s : nomsObligatories) std::cout << " " << s;
  std::cout << std::endl;

  std::cout << "Can appear:";
  for(const std::string& s : nomsAssignatures) std::cout << " " << s;
  std::cout << std::endl << std::endl;

  std::cout << "Mixing is ";
  if(mixGroups) std::cout << "enabled";
  else std::cout << "disabled";
  std::cout << std::endl << std::endl;

  std::cout << "Warning: remember to check if the schedule you want actually exists in https://www.fib.upc.edu/ca/estudis/graus/grau-en-enginyeria-informatica/horaris." << std::endl;
  std::cout << "When there are equivalent lab groups, ONLY ONE (not necessarily the smallest number) is shown. Check if there are more should you need them." << std::endl << std::endl;

  data.makeAndPrintSchedules(preference, MAX_PRINTED_SCHEDULES);
}