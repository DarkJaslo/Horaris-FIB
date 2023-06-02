#include <iostream>
#include <vector>
using namespace std;

void printPerm(const vector<int>& perm){
  for(int i = 0 ; i < perm.size(); ++i){
    cout << perm[i];
    if(i < perm.size()-1) cout << " ";
  }
  cout << endl;
}

void perms(int in, const vector<vector<int>>& grups, vector<vector<bool>>& used, vector<int>& perm){
  if(in == perm.size()) printPerm(perm);
  else{
    for(int i = 0; i < grups[in].size(); ++i){
      perm[in] = grups[in][i];
      used[in][i] = true;
      perms(in+1, grups, used, perm);
    }
  }
}

void perms(const vector<vector<int>>& grups){
  
  vector<vector<bool>> used(grups.size());

  for(int i = 0; i < grups.size(); ++i){
    used[i] = vector<bool>(grups[i].size(),false);
  }

  vector<int> permutation(grups.size());
  perms(0,grups,used,permutation);
}

int main(){

  vector<vector<int>> grups = {{0,1,2},{10,11},{20,21,22}};
  perms(grups);
}