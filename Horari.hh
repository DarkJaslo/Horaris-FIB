#ifndef Horari_hh
#define Horari_hh

#include <vector>
#include <string>
#include <iostream>
using namespace std;

enum DiaSetmana{
  dilluns,
  dimarts,
  dimecres,
  dijous,
  divendres
};
inline std::ostream& operator<<(std::ostream& os, const DiaSetmana& d){
  switch(d){
    case dilluns:   os << "dilluns";   break;
    case dimarts:   os << "dimarts";   break;
    case dimecres:  os << "dimecres";  break;
    case dijous:    os << "dijous";    break;
    case divendres: os << "divendres"; break;
    default:        os << "error";     break;
  }
  return os;
}

const DiaSetmana dies[5] = {dilluns,dimarts,dimecres,dijous,divendres};

enum TipusClasse{
  teoria,
  laboratori
};
inline std::ostream& operator<<(std::ostream& os, const TipusClasse& t){
  switch(t){
    case teoria:       os << "T";  break;
    case laboratori:   os << "L";  break;
    default:        os << "error"; break;
  }
  return os;
}

class HoraClasse{
public:
  HoraClasse(){}
  HoraClasse(int hora, bool ocupada, string assignatura, int _grup, TipusClasse tipus, DiaSetmana dia) : _hora(hora), _ocupada(ocupada), ass(assignatura), _grup(_grup), _tipus(tipus), _dia(dia){}
  string assignatura()const{ return ass; }
  int grup()const{ return _grup; }
  DiaSetmana dia()const{ return _dia; }
  int hora()const{return _hora; }
  bool ocupada()const{return _ocupada;}
  void ocupa(){
    _ocupada = true;
  }
  friend std::ostream& operator<<(std::ostream& os, const HoraClasse& h);
  void print(){
    cout << assignatura() << " " << grup() << _tipus;
  }
private:
  int _hora;
  bool _ocupada;
  string ass;
  int _grup;
  TipusClasse _tipus;
  DiaSetmana _dia;
};

inline std::ostream& operator<<(std::ostream& os, const HoraClasse& h){
  os << h.ass << ": " << endl <<
  h._grup << h._tipus << " " << h._dia << ": " << h._hora << "h" << endl;
  return os;
}



class Horari{
public:
  Horari(){
    h = vector<vector<HoraClasse>>(5,vector<HoraClasse>(13));
    for(int i = 0; i < h.size(); ++i){
      for(int j = 0; j < h[i].size(); ++j){
        h[i][j] = HoraClasse(j+8,false,"",-1,teoria,dies[i]);
      }
    }
  }
  bool encaixa(const HoraClasse& hora){
    return not h[hora.dia()][hora.hora()].ocupada();
  }
  void afegeix(const vector<HoraClasse>& hores){
    //cout << "afegeix" << endl;
    for(int i = 0; i < hores.size(); ++i){
      h[hores[i].dia()][hores[i].hora()-8] = hores[i];
      h[hores[i].dia()][hores[i].hora()-8].ocupa();
    }
    int index = findInVector(assigs,hores[0].assignatura());
    if(index == -1) assigs.push_back(hores[0].assignatura());
  }
  int nassig(){return assigs.size();}

  void print(){
    //cout << "print horari" << endl;
    cout << "\tDilluns" << "\tDimarts" << "\tDimecr" << "\tDijous" << "\tDivend" << endl;

    for(int j = 0; j < h[0].size(); ++j){
      cout << 8+j << "h\t";
      for(int i = 0; i < h.size(); ++i){
        if(h[i][j].ocupada()) h[i][j].print();
        else cout << "  -  ";
        cout << "\t";
      }
      cout << endl;
    }
  }
private:
  template<typename T>
  int findInVector(const vector<T>& vec, const T& thing){
    for(int i = 0; i < vec.size(); ++i){
      if(vec[i] == thing) return i;
    }
    return -1;
  }
  std::vector<std::vector<HoraClasse>> h;
  vector<string> assigs;
};

#endif