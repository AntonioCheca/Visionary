#ifndef _BASIC_VARS_H__
#define _BASIC_VARS_H__

#include <vector>
#include "environment.h"

using namespace std;

void addRandomInitValues(Collector& c){
  for(VariableCollector::iterator it = c.var_coll.begin(); it != c.var_coll.end(); ++it){
    for(vector<Variable>::iterator it_var = it->second.begin(); it_var != it->second.end(); ++it_var){
      if(it_var->getInitValue().empty())
        it_var->setInitValue(NewRandom(it_var->getType()));
    }
  }

}


/** @brief Add 4 random variables to the Collector for each basic type
  */
void addVariables(Collector& c){
  c.var_coll.addVariable(Variable("std::string", "s1", false, false));
  c.var_coll.addVariable(Variable("std::string", "s2", false, false));
  c.var_coll.addVariable(Variable("std::string", "s3", false, false));
  c.var_coll.addVariable(Variable("std::string", "s4", false, false));
  c.var_coll.addVariable(Variable("bool", "b1", false, false));
  c.var_coll.addVariable(Variable("bool", "b2", false, false));
  c.var_coll.addVariable(Variable("bool", "b3", false, false));
  c.var_coll.addVariable(Variable("bool", "b4", false, false));
  c.var_coll.addVariable(Variable("int", "i1", false, false));
  c.var_coll.addVariable(Variable("int", "i2", false, false));
  c.var_coll.addVariable(Variable("int", "i3", false, false));
  c.var_coll.addVariable(Variable("int", "i4", false, false));
  c.var_coll.addVariable(Variable("double", "d1", false, false));
  c.var_coll.addVariable(Variable("double", "d2", false, false));
  c.var_coll.addVariable(Variable("double", "d3", false, false));
  c.var_coll.addVariable(Variable("double", "d4", false, false));
  c.var_coll.addVariable(Variable("char", "c1", false, false));
  c.var_coll.addVariable(Variable("char", "c2", false, false));
  c.var_coll.addVariable(Variable("char", "c3", false, false));
  c.var_coll.addVariable(Variable("char", "c4", false, false));
  addRandomInitValues(c);
}
#endif
