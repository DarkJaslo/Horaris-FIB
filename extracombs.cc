#include <vector>
#include <iostream>
#include <unistd.h>
using namespace std;

//int n = 0;

void print_sol(const vector<int>& sol){
  for(int i = 0; i < sol.size(); ++i){
    cout << sol[i];
    if(i < sol.size()-1) cout << " ";
  }
  cout << endl;
  //++n;
}

void combinations(int in, const vector<int>& opt, vector<int>& sol, vector<bool>& u){
  if(in == sol.size()){
    print_sol(sol);
  }
  else{
    for(int i = 0; i < opt.size(); ++i){
      if(not u[i] and (in == 0 or sol[in-1] < i)){
        u[i] = true;
        sol[in] = i;
        combinations(in+1,opt,sol,u);
        u[i] = false;
      }
    }
  }
}

void combinations(const vector<int>& opt, int groupSize){
  vector<int> sol(groupSize);
  vector<bool> u(opt.size(),false);
  combinations(0,opt,sol,u);
}

void Usage(){
  cout << "Usage: ./combs <num_elems> <starting_elem> <group_size>" << endl;
  exit(1);
}

int main(int argc, char** argv){
  if(argc != 4) Usage();
  int elems = atoi(argv[1]);
  int from = atoi(argv[2]);
  int groupSize = atoi(argv[3]);
  vector<int> opt(elems);
  for(int i = 0; i < opt.size(); ++i){
    opt[i] = from+i;
  }
  combinations(opt,groupSize);
}