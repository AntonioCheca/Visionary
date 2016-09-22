#ifndef _STATEMENTS_H_
#define _STATEMENTS_H_

#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include "environment.h"

using namespace std;

const int FUNCTION_AMOUNT = 800;
const int IF_AMOUNT = 100;
const int WHILE_AMOUNT = 100;
const int SUM = FUNCTION_AMOUNT + IF_AMOUNT + WHILE_AMOUNT;

const int MAX_WHILE_LINES = 5;
const int MAX_IF_LINES = 5;
const int MAX_STRING = 10;

const int MAX_INTS = 10;
const int MAX_ITERATIONS = 1000;

string RandomLine(const Collector& c);

string EscapeCharacters(string s){
  for(int i = 0; i < (int) s.size(); i++){
    if(s[i] == '\'' || s[i] == '\"' || s[i] == '\\'){
      s.push_back(' ');
      for(int j = (int) s.size()-1; j > i; j--)
        s[j] = s[j-1];
      s[i] = '\\';
      i++;
    }
  }
  return s;
}

string QuoteString(string s, char quote){
  s.push_back(' ');
  for(int j = (int) s.size()-1; j > 0; j--)
    s[j] = s[j-1];
  s[0] = quote;
  s.push_back(quote);
  return s;
}

string QuotedCharsToString(string s){
  s = "std::string(" + s + ")";
  return s;
}

string NewRandom(Type type){
  if(type == Type("bool")){
    int k = rand()%2;
    if(k == 1)
      return "true";
    else
      return "false";
  }
  else if(type == Type("int"))
    return to_string(rand()%MAX_INTS);
  else if(type == Type("char")){
    string s;
    s.push_back((char) ' '+rand()%('~'-' '+1)); // The printable characters are between ' ' and '~' in ASCII
    /*if(s[s.size()-1] == '\\' || s[s.size()-1] == '\'' || s[s.size()-1] == '\"'){   // If the random char is the backslash, the ' or ", needs to be backslashed
      char old = s[s.size()-1];
      s[s.size()-1] = '\\';
      s.push_back(old);
    }*/
    s = EscapeCharacters(s);
    s = QuoteString(s, '\'');
    return s;
  }
  else if(type == Type("double"))
    return NewRandom(Type("int")) + "." + NewRandom(Type("int"));
  else if(type == Type("std::string")){
    int k = rand()%MAX_STRING;
    string s;
    for(int i = 0; i < k; i++){
      string new_char = NewRandom(Type("char"));
      for(int j = 1; j < new_char.size()-1; j++)
        s.push_back(new_char[j]);
    }
    //s = EscapeCharacters(s);
    s = QuoteString(s, '\"');
    return QuotedCharsToString(s);
  }
  else if(type == Type("number")){
    int k = rand()%2;
    if(k == 1)
      return NewRandom(Type("int"));
    else
      return NewRandom(Type("double"));
  }
  else
    return "";
}

string Random(const Collector& c, Type type){
  vector<Variable> v = c.var_coll[type];
  int k = v.size();
  int n = rand()%(k+1);
  if(n != k)
    return v[n].getName();
  else
    return NewRandom(type);
}

string RandomOfCollection(const Collector& c, Type type){
  vector<Variable> v = c.var_coll[type];
  if(!v.empty()){
    int k = v.size();
    int n = rand()%k;
    return v[n].getName();
  }
  else
    return "";
}

string Random(const Collector& c, Type type, bool ref){
  if(!ref)
    return Random(c, type);
  else
    return RandomOfCollection(c,type);
}

string RandomFunctionLine(const Collector& c){
  int k = rand()%(c.func_coll.v.size());
  Function f = c.func_coll.v[k];
  f = specifyArguments(f,c);
  while(!c.canCallTheFunction(f)){
    k = rand()%(c.func_coll.v.size());
    f = c.func_coll.v[k];
    f = specifyArguments(f,c);
  }
  string s;
  vector<Variable> v = f.getArgs();
  s = f.getName() + "(";
  for(vector<Variable>::const_iterator it = v.begin(); it != v.end(); ++it)
    s += Random(c, it->getType(), it->getRef()) + ",";

  if(v.size() == 0)
    s.push_back(')');
  else
    s[s.size()-1] = ')';
  s.push_back(';');
  return s;
}

string RandomWhileBlock(const Collector& c){
  static int total_whiles = 0;
  total_whiles++;
  string condition = RandomOfCollection(c,Type("bool"));
  if(condition.size() == 0)
    condition = "false";
  string id_while = "wh_" + to_string(total_whiles);
  condition += " && " + id_while + " < " + to_string(MAX_ITERATIONS);
  string while_block = "int " + id_while + " = 0;\nwhile(" + condition + "){";
  while_block += id_while + "++;\n";
  int k = rand()%(MAX_WHILE_LINES-1)+1;
  for(int i = 0; i < k; i++)
    while_block += RandomLine(c);
  while_block += "}";
  return while_block;
}

string RandomIfBlock(const Collector& c){
  string condition = RandomOfCollection(c,Type("bool"));
  if(condition.size() == 0)
    condition = "true";
  string if_block = "if(" + condition + "){";
  int k = max(1, rand()%MAX_IF_LINES);
  for(int i = 0; i < k; i++)
    if_block += RandomLine(c);
  if_block += "}";
  return if_block;
}

string RandomLine(const Collector& c){
  int k = rand()%SUM;
  if(k < FUNCTION_AMOUNT)
    return RandomFunctionLine(c) + "\n";
  else if(k < FUNCTION_AMOUNT + IF_AMOUNT)
    return RandomIfBlock(c) + "\n";
  else
    return RandomWhileBlock(c) + "\n";
}

string LastWrite(const Collector& c){
  if(c.var_coll.isEmpty())
    return "write("");";

  vector<Type> v_type = c.var_coll.getTypes();
  Type rand_type;
  vector<Variable> vars_of_rand_type;
  Variable rand_var;

  rand_type = v_type[rand()%v_type.size()];
  vars_of_rand_type = c.var_coll[rand_type];
  rand_var = vars_of_rand_type[rand()%vars_of_rand_type.size()];
  return "write(" + rand_var.getName() + ");";
}




#endif
