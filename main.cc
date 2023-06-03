#include <iostream>
#include <vector>
#include <algorithm>
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

//compulsory combs
void pushPerm(const vector<int>& perm, vector<vector<int>>& allPerms){
  vector<int> p;
  for(int i = 0 ; i < perm.size(); ++i){
    p.push_back(perm[i]);
  }
  allPerms.push_back(p);
}

void perms(int in, const vector<vector<int>>& grups, vector<vector<bool>>& used, vector<int>& perm, vector<vector<int>>& allPerms){
  if(in == perm.size()) pushPerm(perm,allPerms);
  else{
    for(int i = 0; i < grups[in].size(); ++i){
      perm[in] = grups[in][i];
      used[in][i] = true;
      perms(in+1, grups, used, perm,allPerms);
    }
  }
}

void perms(const vector<vector<int>>& grups, vector<vector<int>>& allPerms){
  
  vector<vector<bool>> used(grups.size());

  for(int i = 0; i < grups.size(); ++i){
    used[i] = vector<bool>(grups[i].size(),false);
  }
  vector<int> permutation(grups.size());
  perms(0,grups,used,permutation,allPerms);
}

//extracombs
int from;

void print_sol(const vector<int>& sol,vector<vector<int>>& extraPermutations){
  vector<int> sol2 = sol;
  for(int& i : sol2) i += from;
  extraPermutations.push_back(sol2);
}

void combinations(int in, const vector<int>& opt, vector<int>& sol, vector<bool>& u,vector<vector<int>>& extraPermutations){
  if(in == sol.size()){
    print_sol(sol,extraPermutations);
  }
  else{
    for(int i = 0; i < opt.size(); ++i){
      if(not u[i] and (in == 0 or sol[in-1] < i)){
        u[i] = true;
        sol[in] = i;
        combinations(in+1,opt,sol,u,extraPermutations);
        u[i] = false;
      }
    }
  }
}

void combinations(const vector<int>& opt, int groupSize, vector<vector<int>>& extraPermutations){
  vector<int> sol(groupSize);
  vector<bool> u(opt.size(),false);
  combinations(0,opt,sol,u,extraPermutations);
}

void extraCombinations(int elems, int fromWhere, int groupSize,vector<vector<int>>& extraPermutations){
  from = fromWhere;
  vector<int> opt(elems);
  for(int i = 0; i < opt.size(); ++i){
    opt[i] = i;
  }
  combinations(opt,groupSize, extraPermutations);
}

//all combs
void copyVector(const vector<int>& source1, const vector<vector<int>>& source2, vector<vector<int>>& dest, int loc){
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

void getAssignatura(assignatura& ass, const pair<string,int>& info, const vector<assignatura>& assigs){
  for(const assignatura& a : assigs){
    if(a[0].assignatura() == info.first and a[0].grup() == info.second){
      ass = a;
      return;
    }
  }
}

//schedule making
void fesHoraris(vector<Horari>& horarisValids, const vector<vector<int>>& perms, const vector<pair<string,int>>& map, const vector<assignatura>& assigs){
  for(int i = 0; i < perms.size(); ++i){
    Horari h;
    bool ok = true;
    for(int j = 0; j < perms[i].size(); ++j){
      assignatura ass;
      getAssignatura(ass,map[perms[i][j]],assigs);
      if(h.encaixaAssig(ass)){
        h.afegeix(ass);
      }
      else{
        ok = false;
        break;
      }
    }
    if(ok){
      horarisValids.push_back(h);
    }
  }
}

int main(){
  vector<assignatura> assigs;

  string nom;
  cin >> nom;

  //reads everything
  while(nom != "end"){
    assignatura ass;
    
    string tipus;
    int grup;
    string dia;
    int hora;

    cin >> grup;

    cin >> tipus;
    while(tipus != "endtipus"){
      cin >> dia;
      cin >> hora;
      while(hora != -1){
        TipusClasse tip;
        if(tipus == "teoria") tip = teoria;
        else tip = laboratori;
        HoraClasse hcl(hora,false,nom,grup,tip,stringToDia(dia));
        ass.push_back(hcl);
        cin >> hora;
      }
      cin >> tipus;
    }
    cin >> nom;
    assigs.push_back(ass);
  }


  vector<string> nomsObligatories;
  vector<string> nomsAssignatures;

  //Fill names
  string auxAssig;
  while(cin >> auxAssig){
    nomsObligatories.push_back(auxAssig);
  }

  for(const assignatura& ass: assigs){
    int index = findInVector(nomsObligatories,ass[0].assignatura());
    if(index != -1) continue;
    index = findInVector(nomsAssignatures,ass[0].assignatura());
    if(index != -1) continue;
    nomsAssignatures.push_back(ass[0].assignatura());
  }

  //Separate subjects in groups
  vector<vector<pair<string,int>>> obligatories(nomsObligatories.size());
  vector<vector<pair<string,int>>> resta(nomsAssignatures.size());

  for(const assignatura& ass : assigs){
    string nom = ass[0].assignatura();
    int grup = ass[0].grup();
    int index = findInVector(nomsObligatories,nom);
    if(index == -1){
      index = findInVector(nomsAssignatures,nom);
      if(index == -1) continue;
      resta[index].push_back(make_pair(nom,grup));
    }
    else{
      obligatories[index].push_back(make_pair(nom,grup));
    }
    
  }

  //Mapping to manage permutations with integers instead of pair<string,int>
  vector<pair<string,int>> mapping;

  //Make groups with integers
  int aux = 0;
  vector<vector<int>> grupsObligatories(1);

  for(int i = 0; i < obligatories.size(); ++i){
    for(int j = 0; j < obligatories[i].size(); ++j){
      mapping.push_back(obligatories[i][j]);
      if(obligatories[i][j].first == nomsObligatories[aux]){
        grupsObligatories[aux].push_back(mapping.size()-1);
      }
      else{
        aux++;
        grupsObligatories.push_back(vector<int>());
        grupsObligatories[aux].push_back(mapping.size()-1);
      }
    }
  }

  int oblig = mapping.size();

  for(int i = 0; i < resta.size(); ++i){
    for(int j = 0; j < resta[i].size(); ++j){
      mapping.push_back(resta[i][j]);
    }
  }

  //Make permutations of compulsory subjects
  vector<vector<int>> permutations;
  perms(grupsObligatories,permutations);

  //Make permutations of extra subjects
  vector<vector<int>> extraPermutations;
  extraCombinations(mapping.size()-oblig,oblig,5-nomsObligatories.size(),extraPermutations);

  //Combine them in allPermutations
  vector<vector<int>> allPermutations(permutations.size()*extraPermutations.size(),vector<int>(permutations[0].size()+extraPermutations[0].size()));
  int cur = 0;
  for(int i = 0; i < allPermutations.size(); i +=extraPermutations.size()){
    copyVector(permutations[cur],extraPermutations,allPermutations,i);
    ++cur;
  }

  //Print all permutations
  /*for(int i = 0; i < allPermutations.size(); ++i){
    for(int j = 0; j < allPermutations[i].size(); ++j){
      cout << allPermutations[i][j] << " ";
    }
    cout << endl;
  }*/

  //Makes valid schedules
  vector<Horari> horarisValids;
  fesHoraris(horarisValids, allPermutations,mapping,assigs);

  //Sorts them
  for(Horari& h : horarisValids){
    h.calculaValor();
  }
  sort(horarisValids.begin(),horarisValids.end());

  //Prints them
  for(const Horari& h : horarisValids){
    h.print();
    cout << endl;
  }

  //Print subjects
  cout << "no obligatòries" << endl;

  for(const string& no : nomsAssignatures){
    cout << no << endl;
  }

  cout << endl << "obligatòries: " << endl;

  for(const string& no : nomsObligatories){
    cout << no << endl;
  }
}