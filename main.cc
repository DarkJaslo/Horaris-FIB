#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "HorarisParser.hh"
//#include "SocketWrap.hh" not ready yet

using namespace jaslo;
using namespace std;

typedef vector<HoraClasse> assignatura;

#define MAX_PRINTED_SCHEDULES 1000 //Change to print more if needed. Aims to avoid more than 500kB/file

//TODO: Do the combinations in a class, simplify main() as much as possible

template<typename T>
int findInVector(const vector<T>& vec, const T& thing)
{
  for(int i = 0; i < vec.size(); ++i){
    if(vec[i] == thing) return i;
  }
  return -1;
}

//Pushes a permutation into a group of permutations
void pushPerm(const vector<int>& perm, vector<vector<int>>& allPerms)
{
  vector<int> p;
  for(int i = 0 ; i < perm.size(); ++i){
    p.push_back(perm[i]);
  }
  allPerms.push_back(p);
}

//Recursive solution for perms
void perms(int in, const vector<vector<int>>& grups, vector<vector<bool>>& used, vector<int>& perm, vector<vector<int>>& allPerms)
{
  if(in == perm.size()) pushPerm(perm,allPerms);
  else{
    for(int i = 0; i < grups[in].size(); ++i){
      perm[in] = grups[in][i];
      used[in][i] = true;
      perms(in+1, grups, used, perm,allPerms);
    }
  }
}

//Gets all permutations of compulsory subjects
void perms(const vector<vector<int>>& grups, vector<vector<int>>& allPerms)
{  
  vector<vector<bool>> used(grups.size());

  for(int i = 0; i < grups.size(); ++i){
    used[i] = vector<bool>(grups[i].size(),false);
  }
  vector<int> permutation(grups.size());
  perms(0,grups,used,permutation,allPerms);
}

//extra combinations
int from;

void pushPermExtra(const vector<int>& sol,vector<vector<int>>& extraPermutations)
{
  vector<int> sol2 = sol;
  for(int& i : sol2) i += from;
  extraPermutations.push_back(sol2);
}

void combinations(int in, const vector<int>& opt, vector<int>& sol, vector<bool>& u,vector<vector<int>>& extraPermutations)
{
  if(in == sol.size()){
    pushPermExtra(sol,extraPermutations);
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

/*void combinations(const vector<int>& opt, int groupSize, vector<vector<int>>& extraPermutations){
  
}*/

void extraCombinations(int elems, int fromWhere, int groupSize,vector<vector<int>>& extraPermutations)
{
  from = fromWhere;
  vector<int> opt(elems);
  for(int i = 0; i < opt.size(); ++i){
    opt[i] = i;
  }
  vector<int> sol(groupSize);
  vector<bool> u(opt.size(),false);
  combinations(0,opt,sol,u,extraPermutations);
  //combinations(opt,groupSize, extraPermutations);
}

//all combinations
void copiaVector(const vector<int>& source1, const vector<vector<int>>& source2, vector<vector<int>>& dest, int loc)
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

void getAssignatura(assignatura& ass, const pair<string,int>& info, const vector<assignatura>& assigs)
{
  for(const assignatura& a : assigs){
    if(a[0].assignatura() == info.first and a[0].grup() == info.second){
      ass = a;
      return;
    }
  }
}

//schedule making
void fesHoraris(vector<Horari>& horarisValids, const vector<vector<int>>& perms, const vector<pair<string,int>>& map, const vector<assignatura>& assigs)
{
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

int main(int argc, char** argv)
{
  if(argc != 2)
  {
    std::cerr << "Usage: ./main.exe [FIB's_data_filename]" << std::endl;
    exit(1);
  }
  
  std::string filename = argv[1];

  /*SocketWrap sw;
  sw.getAPIinFile("inputweb.txt");
  exit(0);*/

  Parser parser;
  parser.openFile(filename);
  parser.getCount();
  Data data(parser.count());
  
  HorariObj obj;

  while(parser.readHorariObj(obj)){
    data.pushHorariObj(obj);
  }
  data.pushHorariObj(obj);
  //data.print();

  int sizeHorari;
  cin >> sizeHorari;

  vector<assignatura> assigs = data.allAssignatures();

  vector<string> nomsObligatories;
  vector<string> nomsAssignatures;

  //Fill names
  string auxAssig;
  cin >> auxAssig;  //Read the "ALWAYS:"
  cin >> auxAssig;
  while(auxAssig != "END_INPUT"){
    nomsObligatories.push_back(auxAssig);
    cin >> auxAssig;
  }

  cin >> auxAssig;  //Read the "INCLUDE:"
  while(auxAssig != "END_INPUT"){
    nomsAssignatures.push_back(auxAssig);
    cin >> auxAssig;
  }

  //Group all groups that share the same subject code/name (S1 Gr10, S1 Gr11, etc.)
  vector<vector<pair<string,int>>> obligatories(nomsObligatories.size());
  vector<vector<pair<string,int>>> resta(nomsAssignatures.size());

  for(const assignatura& ass : assigs)
  {
    string nom = ass[0].assignatura();
    int grup = ass[0].grup();
    int index = findInVector(nomsObligatories,nom);
    if(index == -1){ //not found
      index = findInVector(nomsAssignatures,nom);
      if(index == -1) continue; //not found
      resta[index].push_back(make_pair(nom,grup));
    }
    else{
      obligatories[index].push_back(make_pair(nom,grup));
    }
  }

  //Map to manage permutations with integers instead of pair<string,int>
  vector<pair<string,int>> map;

  //Make groups with integers
  int aux = 0;
  vector<vector<int>> grupsObligatories(1);

  for(int i = 0; i < obligatories.size(); ++i){
    for(int j = 0; j < obligatories[i].size(); ++j)
    {
      map.push_back(obligatories[i][j]);
      if(obligatories[i][j].first == nomsObligatories[aux]){
        grupsObligatories[aux].push_back(map.size()-1);
      }
      else{
        aux++;
        grupsObligatories.push_back(vector<int>());
        grupsObligatories[aux].push_back(map.size()-1);
      }
    }
  }

  int oblig = map.size();

  for(int i = 0; i < resta.size(); ++i){
    for(int j = 0; j < resta[i].size(); ++j){
      map.push_back(resta[i][j]);
    }
  }

  //Make permutations of compulsory subjects
  vector<vector<int>> permutations;
  perms(grupsObligatories,permutations);

  //Make permutations of extra subjects
  vector<vector<int>> extraPermutations;
  extraCombinations(map.size()-oblig,oblig,sizeHorari-nomsObligatories.size(),extraPermutations);

  //Combine them in allPermutations
  vector<vector<int>> allPermutations;
  if(permutations.size() == 0){
    allPermutations = extraPermutations;
  }
  else if(extraPermutations.size() == 0){
    allPermutations = permutations;
  }
  else{
    allPermutations = vector<vector<int>>(permutations.size()*extraPermutations.size(),vector<int>(permutations[0].size()+extraPermutations[0].size()));

    int cur = 0;
    for(int i = 0; i < allPermutations.size(); i +=extraPermutations.size()){
      copiaVector(permutations[cur],extraPermutations,allPermutations,i);
      ++cur;
    }
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
  fesHoraris(horarisValids, allPermutations,map,assigs);

  //Sorts them
  for(Horari& h : horarisValids){
    h.calculaValor();
  }
  sort(horarisValids.begin(),horarisValids.end());

  //Prints them
  int horarisImpresos = 0;
  for(const Horari& h : horarisValids){
    h.print();
    cout << endl;
    if(++horarisImpresos == MAX_PRINTED_SCHEDULES) break;
  }
  cout << "Impresos: " << horarisImpresos << endl; 
  cout << "Total horaris: " << horarisValids.size() << endl;
}

