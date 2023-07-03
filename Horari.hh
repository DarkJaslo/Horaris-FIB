#ifndef Horari_hh
#define Horari_hh

#include <vector>
#include <string>
#include <iostream>

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
  HoraClasse(int hora, int grup, TipusClasse tipus, DiaSetmana dia, bool ocupada, std::string assignatura) 
    : _hora(hora), _grup(grup), _tipus(tipus), _dia(dia), _ocupada(ocupada), ass(assignatura){}
  std::string assignatura()const{ return ass; }
  int grup()const{ return _grup; }
  DiaSetmana dia()const{ return _dia; }
  int hora()const{ return _hora; }
  bool ocupada()const{ return _ocupada; }
  void ocupa(){ _ocupada = true; }
  TipusClasse tipus() const{ return _tipus; }
  friend std::ostream& operator<<(std::ostream& os, const HoraClasse& h);
  void print() const{
    std::cout << assignatura() << " " << grup() << tipus();
  }
private:
  int _hora;
  int _grup;
  TipusClasse _tipus;
  DiaSetmana _dia;
  bool _ocupada;
  std::string ass;
};

inline std::ostream& operator<<(std::ostream& os, const HoraClasse& h){
  os << h.ass << ": " << std::endl <<
  h._grup << h._tipus << " " << h._dia << ": " << h._hora << "h" << std::endl;
  return os;
}


class Horari{
public:
  Horari(){
    _valor = 0;
    h = std::vector<std::vector<HoraClasse>>(5,std::vector<HoraClasse>(13));
    for(int i = 0; i < h.size(); ++i){
      for(int j = 0; j < h[i].size(); ++j)
        h[i][j] = HoraClasse(j+8,-1,teoria,dies[i],false,"");
    }
  }
  bool encaixa(const HoraClasse& hora){
    return not h[hora.dia()][hora.hora()-8].ocupada();
  }
  bool encaixaAssig(const std::vector<HoraClasse>& assig){
    int index = findInVector(assigs,assig[0].assignatura());
    if(index != -1){
      int index2 = findInVector(grups,assig[0].grup());
      if(index2 == -1) return false;
    }
    for(const HoraClasse& hcl : assig){
      if(not encaixa(hcl)){ return false; }
    }
    return true;
  }
  void afegeix(const std::vector<HoraClasse>& hores){
    int index = findInVector(assigs,hores[0].assignatura());
    if(index == -1){
      assigs.push_back(hores[0].assignatura());
      grups.push_back(hores[0].grup());
    }
    for(int i = 0; i < hores.size(); ++i){
      h[hores[i].dia()][hores[i].hora()-8] = hores[i];
      h[hores[i].dia()][hores[i].hora()-8].ocupa();
    }
  }
  int nassig(){return assigs.size();}
  int valor()const { return _valor; }
  bool operator<(const Horari& other)const{ return _valor > other._valor; }
  void print()const{
    std::cout << "\tDilluns" << "\tDimarts" << "\tDimecr" << "\tDijous" << "\tDivend" << std::endl;

    for(int j = 0; j < h[0].size(); ++j){
      std::cout << 8+j << "h\t";
      for(int i = 0; i < h.size(); ++i){
        if(h[i][j].ocupada()) h[i][j].print();
        else std::cout << "  -  ";
        std::cout << "\t";
      }
      std::cout << std::endl;
    }
  }
  void calculaValor(){
    _valor = 0;
    for(int i = 0; i < h.size(); ++i){
      bool diaLliure = true;
      int horesBuides = 0;
      int horesTotal = 0;
      bool hiHaHoresBuides = false;
      for(int j = 0; j < h[i].size(); ++j){
        if(h[i][j].ocupada()){
          diaLliure = false;
          horesTotal++;
          if(j == 0){ //Les 8
            _valor -= 50;
          }
          else if(j == 1){ //Les 9
            _valor -= 50;
          }
          else if(j >= 6){ //Les 14h o més
            _valor -= 20;
          }
        }
        if(not diaLliure and not h[i][j].ocupada()){
          horesBuides++;
        }
        else if(not diaLliure and h[i][j].ocupada()){
          _valor -= 200*horesBuides;
          if(horesBuides > 0){
            if(horesTotal == 4) _valor+=100*horesBuides;
          }
          horesBuides = 0;
          if(not hiHaHoresBuides) hiHaHoresBuides = true;
        }
      }
      if(diaLliure){_valor += 750;}
      if(horesTotal < 3){_valor -= 150;}
      else if(horesTotal == 4 and not hiHaHoresBuides){_valor += 100;}
      else if(horesTotal == 6){_valor -= 100;}
      else if(horesTotal > 6){ _valor -= 375*(horesTotal-6);}
    }
  }
private:
  template<typename T>
  int findInVector(const std::vector<T>& vec, const T& thing){
    for(int i = 0; i < vec.size(); ++i){
      if(vec[i] == thing) return i;
    }
    return -1;
  }
  std::vector<std::vector<HoraClasse>> h;
  std::vector<std::string> assigs;
  std::vector<int> grups;
  int _valor;
};

#endif