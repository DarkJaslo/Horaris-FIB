#include "Horari.hh"
using namespace jaslo;

const Weekday ALL_DAYS[5] = { monday, tuesday, wednesday, thursday, friday };


//ClassHour

ClassHour::ClassHour(){}
ClassHour::ClassHour(int hour, int group, ClassType type, Weekday day, bool taken, const std::string& subjectName) 
  : _hour(hour), _group(group), _type(type), _day(day), _taken(taken), _subjectName(subjectName){}
std::string ClassHour::subjectName() const { return _subjectName; }
int         ClassHour::group()       const { return _group;       }
Weekday     ClassHour::day()         const { return _day;         }
int         ClassHour::hour()        const { return _hour;        }
bool        ClassHour::taken()       const { return _taken;       }
ClassType   ClassHour::type()        const { return _type;        }
void ClassHour::print() const
{
  std::cout << _subjectName << " " << _group << _type;
}
void        ClassHour::take()              { _taken = true;       }


//HorariObj

HorariObj::HorariObj(){}
std::string             HorariObj::code()  const { return _code;  }
int                     HorariObj::group() const { return _group; }
Weekday                 HorariObj::day()   const { return _day;   }
const std::vector<int>  HorariObj::hours() const { return _hours; }
ClassType               HorariObj::type()  const { return _type;  }
bool                    HorariObj::equivalent(const HorariObj& other) const
{
  return (this->code() == other.code()) and (this->group() != other.group()) and ((this->group()/10) == (other.group()/10)) and (this->day() == other.day()) and (this->hours() == other.hours());
}


//Horari

//Public
Horari::Horari()
{
  _value = 0;
  H = std::vector<ClassHour>(65);
  for(int i = 0; i < H.size(); ++i)
  {
    H[i] = ClassHour(((i-8)%13+8),-1,ClassType::theory,ALL_DAYS[i/13],false,"");
  }
}

bool Horari::doesItFit(const std::vector<HorariObj>& subj)
{
  //Temporally fills some hours in the schedule to handle wrong input, so it needs to save which ones it filled to free them
  std::vector<int> toFreeIndices(0);

  for(int i = 0; i < subj.size(); ++i)
  {
    std::vector<int> subjHours = subj[i].hours();
    for(int j = 0; j < subjHours.size(); ++j)
    {
      int index = indexFor(subj[i].day(),subjHours[j]);
      if(H[index].taken()) return false;
      H[index].take();
      toFreeIndices.push_back(index);
    }
  }

  //Frees all ClassHours occupied in this function
  for(int i = 0; i < toFreeIndices.size(); ++i)
  {
    H[toFreeIndices[i]]._taken = false;
  }
  return true;
}

void Horari::addSubject(const std::vector<HorariObj>& subj)
{
  for(int i = 0; i < subj.size(); ++i)
  {
    std::vector<int> subjHours = subj[i].hours();
    for(int j = 0; j < subjHours.size(); ++j)
    {
      int index = indexFor(subj[i].day(),subjHours[j]);
      H[index] = ClassHour(H[index].hour(),subj[i].group(),subj[i].type(),H[index].day(),true,subj[i].code());
    }
  }

  //Add subject name if it isn't already
  if(not containsSubject(subj[0].code()))
  {
    _subjectNames.push_back(subj[0].code());
  }  
}

int Horari::subjectCount() const { return _subjectNames.size(); }
int Horari::value()        const { return _value;               }

void Horari::print() const
{
  std::cout << "\tMonday" << "\t\tTuesday" << "\t\tWednesday" << "\tThursday" << "\tFriday" << std::endl;

  for(int c = 0; c < 13; ++c) //For all hours in a day
  {
    std::cout << 8+c << "h\t";
    for(int v = 0; v < 5; ++v) //For all days
    {
      int index = indexFor(ALL_DAYS[v],8+c);
      if(H[index].taken()) H[index].print();
      else std::cout << "  -  ";
      std::cout << "\t";
      if(H[index].subjectName().length() <= 3) std::cout << "\t";
    }
    std::cout << std::endl;
  }
}

void Horari::computeValue(SchedulePreference pref)
{
  for(int i = 0; i < 5; ++i) //Iterates through weekdays
  {
    bool dayOff = true;
    int emptyHours = 0;
    bool areThereEmptyHours = false;
    int classHours = 0;

    for(int j = 0; j < 13; ++j) //Iterates through hours
    {
      int index = indexFor(ALL_DAYS[i],j+8);
      if(H[index].taken()) dayOff = false;

      if(not dayOff and not H[index].taken()){
        emptyHours++;
      } 
      else if(not dayOff and H[index].taken()){
        _value -= 201*emptyHours;
        emptyHours = 0;
        if(not areThereEmptyHours) areThereEmptyHours = true;
      }

      int hour = j+8;

      if(H[index].taken()){
        classHours++;

        switch (pref){
        case morning :{
          if(hour >= 14){ _value -= 51;}
          else if(hour == 8 or hour == 9){ _value -= 50;}
          break;
        }
        case afternoon :{
          if(hour < 14){ _value -= 51;}
          else if(hour == 19){ _value -= 25;}
          else if(hour == 20){ _value -= 50;}
          break;
        }
        default:
          break;
        }
      }
    }

    if(dayOff){ _value += 750; }
    if(classHours < 3){ _value -= 150; }
    else if(classHours == 4 and not areThereEmptyHours){ _value += 100; }
    else if(classHours == 6){ _value -= 100; }
    else if(classHours > 6){ _value -= 375*(classHours-6); }
  }
}


//Private
int Horari::indexFor(Weekday day, int hour){ return 13*int(day)+hour-8; }
bool Horari::containsSubject(const std::string& name) const
{
  for(int i = 0; i < _subjectNames.size(); ++i){
    if(_subjectNames[i] == name) return true;
  }
  return false;
}