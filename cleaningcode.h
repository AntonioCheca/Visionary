#ifndef _CLEANING_CODE_H__
#define _CLEANING_CODE_H__

#include "environment.h"
#include <vector>

using namespace std;

bool itAppears(const string& orig, const string& s){
  bool found = false, equals;
  for(int i = 0; (i <= (int) (orig.size())- (int) s.size()) && !found; i++){
    equals = true;
    for(int j = 0; (j < s.size()) && equals; j++)
      equals = orig[i+j] == s[j];
    found = equals;
  }
  return found;
}

//Esto no va aquí
// Hay que poner los while
int NumberOfLinesPrinted(string code){
  string s = "\\n";
  int total = 0;
  bool equals;
  for(int i = 0; (i <= (int) (code.size())- (int) s.size()); i++){
    equals = true;
    for(int j = 0; (j < s.size()) && equals; j++)
      equals = code[i+j] == s[j];
    if(equals)
      total++;
  }
  return total;
}

void eraseUnusedVars(const string& code, Collector& coll){
  VariableCollector v;
  for(VariableCollector::iterator it = coll.var_coll.begin(); it != coll.var_coll.end(); ++it){
    for(vector<Variable>::iterator it_var = it->second.begin(); it_var != it->second.end(); ++it_var){
      if(itAppears(code, it_var->getName()))
        v.addVariable(*it_var);
    }
  }
  coll.var_coll = v;
}

void eraseUnusedFuncs(const string& code, Collector& coll){
  FunctionCollector f;
  for(vector<Function>::iterator it = coll.func_coll.v.begin(); it != coll.func_coll.v.end(); ++it){
    if(itAppears(code, it->getName()))
      f.v.push_back(*it);
  }
  coll.func_coll = f;
  addEssentialOperators(coll);
}

void eraseUnusedStuff(const string& code, Collector& coll){

  eraseUnusedVars(code, coll);
  eraseUnusedFuncs(code, coll);
}

#endif
