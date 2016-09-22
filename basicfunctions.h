#ifndef _BASIC_FUNCTIONS_H__
#define _BASIC_FUNCTIONS_H__

#include <algorithm>
#include "environment.h"

/** @brief Add essential functions to a Collector. Right now it includes only writing.
  */
  void addEssentialOperators(Collector& c){
    if(find(c.include_coll.begin(), c.include_coll.end(), "essential_operators.h") == c.include_coll.end())
      c.include_coll.push_back("essential_operators.h");
    vector<Variable> v;
    v.push_back(Variable("", "writing", false, true, true));
    Function f("void", "write", v, false);
    c.func_coll.v.push_back(f);
    v.clear();
  }
/** @brief Add basic operator to a Collector. The new operators are arithmetic(+, -, *, /, %), comparators(=, ==, !=, <=, >=, <, >) and logical(&&, ||)
  */
void addBasicOperators(Collector& c){
  Function temp;
  vector<Variable> v;
  string code;

  v.push_back(Variable("number", "sum", 1, 0, 0));
  v.push_back(Variable("number", "sum1", 0, 1, 0));
  v.push_back(Variable("number", "sum2", 0, 1, 0));
  code = "return sum = sum1+sum2;\n";
  temp = Function("number", "basic_sum", v, false, code);
  c.func_coll.v.push_back(temp);
  v.clear();

  v.push_back(Variable("number", "sub", 1, 0, 0));
  v.push_back(Variable("number", "sub1", 0, 1, 0));
  v.push_back(Variable("number", "sub2", 0, 1, 0));
  code = "return sub = sub1-sub2;\n";
  temp = Function("number", "basic_sub", v, false, code);
  c.func_coll.v.push_back(temp);
  v.clear();

  v.push_back(Variable("number", "prod", 1, 0, 0));
  v.push_back(Variable("number", "prod1", 0, 1, 0));
  v.push_back(Variable("number", "prod2", 0, 1, 0));
  code = "return prod = (prod1*prod2);\n";
  temp = Function("number", "basic_prod", v, false, code);
  c.func_coll.v.push_back(temp);
  v.clear();

  v.push_back(Variable("number", "div", 1, 0, 0));
  v.push_back(Variable("number", "div1", 0, 1, 0));
  v.push_back(Variable("number", "div2", 0, 1, 0));
  code = "return div = (div1/div2);\n";
  temp = Function("number", "basic_div", v, false, code);
  c.func_coll.v.push_back(temp);
  v.clear();

  v.push_back(Variable("int", "mod0", 1, 0, 0));
  v.push_back(Variable("int", "mod1", 0, 1, 0));
  v.push_back(Variable("int", "mod2", 0, 1, 0));
  code = "return mod0 = (mod1 % mod2);\n";
  temp = Function("int", "basic_mod", v, false, code);
  c.func_coll.v.push_back(temp);
  v.clear();

  v.push_back(Variable("", "eq", 1, 0, 1));
  v.push_back(Variable("", "eq1", 0, 1, 1));
  code = "return eq = eq1;\n";
  temp = Function(Type("",1), "basic_equal", v, false, code);
  c.func_coll.v.push_back(temp);
  v.clear();

  v.push_back(Variable("bool", "equality", 1, 0, 0));
  v.push_back(Variable("", "equality1", 0, 1, 1));
  v.push_back(Variable("", "equality2", 0, 1, 1));
  code = "return equality = (equality1 == equality2);\n";
  temp = Function("bool", "basic_equality", v, false, code);
  c.func_coll.v.push_back(temp);
  v.clear();

  v.push_back(Variable("bool", "inequality", 1, 0, 0));
  v.push_back(Variable("", "inequality1", 0, 1, 1));
  v.push_back(Variable("", "inequality2", 0, 1, 1));
  code = "return inequality = (inequality1 != inequality2);\n";
  temp = Function("bool", "basic_inequality", v, false, code);
  c.func_coll.v.push_back(temp);
  v.clear();

  v.push_back(Variable("bool", "comp_leq", 1, 0, 0));
  v.push_back(Variable("", "comp_leq1", 0, 1, 1));
  v.push_back(Variable("", "comp_leq2", 0, 1, 1));
  code = "return comp_leq = (comp_leq1 <= comp_leq2);\n";
  temp = Function("bool", "basic_comp_leq", v, false, code);
  c.func_coll.v.push_back(temp);
  v.clear();

  v.push_back(Variable("bool", "comp_lt", 1, 0, 0));
  v.push_back(Variable("", "comp_lt1", 0, 1, 1));
  v.push_back(Variable("", "comp_lt2", 0, 1, 1));
  code = "return comp_lt = (comp_lt1 < comp_lt2);\n";
  temp = Function("bool", "basic_comp_lt", v, false, code);
  c.func_coll.v.push_back(temp);
  v.clear();

  v.push_back(Variable("bool", "comp_geq", 1, 0, 0));
  v.push_back(Variable("", "comp_geq1", 0, 1, 1));
  v.push_back(Variable("", "comp_geq2", 0, 1, 1));
  code = "return comp_geq = (comp_geq1 >= comp_geq2);\n";
  temp = Function("bool", "basic_comp_geq", v, false, code);
  c.func_coll.v.push_back(temp);
  v.clear();

  v.push_back(Variable("bool", "comp_gt", 1, 0, 0));
  v.push_back(Variable("", "comp_gt1", 0, 1, 1));
  v.push_back(Variable("", "comp_gt2", 0, 1, 1));
  code = "return comp_gt = (comp_gt1 > comp_gt2);\n";
  temp = Function("bool", "basic_comp_gt", v, false, code);
  c.func_coll.v.push_back(temp);
  v.clear();

  v.push_back(Variable("bool", "and0", 1, 0, 0));
  v.push_back(Variable("bool", "and1", 0, 1, 0));
  v.push_back(Variable("bool", "and2", 0, 1, 0));
  code = "return and0 = (and1 && and2);\n";
  temp = Function("bool", "basic_and", v, false, code);
  c.func_coll.v.push_back(temp);
  v.clear();

  v.push_back(Variable("bool", "or0", 1, 0, 0));
  v.push_back(Variable("bool", "or1", 0, 1, 0));
  v.push_back(Variable("bool", "or2", 0, 1, 0));
  code = "return or0 = (or1 || or2);\n";
  temp = Function("bool", "basic_or", v, false, code);
  c.func_coll.v.push_back(temp);
  v.clear();

  v.push_back(Variable("bool", "not0", 1, 0, 0));
  v.push_back(Variable("bool", "not1", 0, 1, 0));
  code = "return not0 = !not1;\n";
  temp = Function("bool", "basic_not", v, false, code);
  c.func_coll.v.push_back(temp);
  v.clear();
}

/** @brief This function adds to the Collector some useful functions of libraries
  * This could be separated on other functions
  */
void AddUsefulFunctions(Collector& c){
  //TODO
}

#endif
