#include <iostream>
#include <vector>
#include "Horari.hh"
using namespace std;

typedef vector<HoraClasse> assignatura;

DiaSetmana stringToDia(const string& str){
  if(str == "dilluns") return dilluns;
  else if(str == "dimarts") return dimarts;
  else if(str == "dimecres") return dimecres;
  else if(str == "dijous") return dijous;
  return divendres;
}

template<typename T>
int findInVector(const vector<T>& vec, const T& thing){
  for(int i = 0; i < vec.size(); ++i){
    if(vec[i] == thing) return i;
  }
  return -1;
}

void printHorari(const vector<assignatura>& assigs){
  Horari h;

  vector<string> assignaturesAfegides;

  for(const assignatura& ass: assigs){
    if(h.nassig() >= 5) break;
    string assig1 = ass[0].assignatura();
    int index = findInVector(assignaturesAfegides,assig1);
    if(index != -1) continue;

    bool totEncaixa = true;
    for(const HoraClasse& hcl : ass){
      if(not h.encaixa(hcl)){ totEncaixa = false; break; }
    }

    if(not totEncaixa) continue;

    h.afegeix(ass);

    assignaturesAfegides.push_back(assig1);
  }

  h.print();
}

void printCombinacions(const vector<string>& nomsAssignatures, const vector<assignatura>& assigs){
  vector<pair<string,int>> utilitzades;
  vector<bool> usats(nomsAssignatures.size(),false);
  int numUsats = 0;

  vector<vector<pair<string,int>>> totes(nomsAssignatures.size());

  for(const assignatura& ass : assigs){
    string nom = ass[0].assignatura();
    int index = findInVector(nomsAssignatures,nom);
    if(index == -1) continue;
    int grup = ass[0].grup();
    totes[index].push_back(make_pair(nom,grup));
  }
}

int main(){
  vector<assignatura> assigs;

  string nom;
  cin >> nom;

  while(nom != "end"){
    //cout << "w1" << endl;
    assignatura ass;
    
    string tipus;
    int grup;
    string dia;
    int hora;

    cin >> grup;

    cin >> tipus;
    while(tipus != "endtipus"){
      //cout << "w2" << endl;
      cin >> dia;
      while(dia != "enddia"){
        //cout << "w3" << endl;
        cin >> hora;
        while(hora != -1){
          //cout << "w4" << endl;
          TipusClasse tip;
          if(tipus == "teoria") tip = teoria;
          else tip = laboratori;
          HoraClasse hcl(hora,false,nom,grup,tip,stringToDia(dia));
          ass.push_back(hcl);
          cin >> hora;
        }
        cin >> dia;
      }
      cin >> tipus;
    }
    cin >> nom;
    assigs.push_back(ass);
  }

  /*for(const assignatura& assaux : assigs){
    for(const HoraClasse& hcl : assaux){
      cout << hcl;
    }
  }*/

  printHorari(assigs);  
}