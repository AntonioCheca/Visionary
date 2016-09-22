#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_

#include <vector>
#include <utility>
#include <map>

using namespace std;

class Type{
  private:
    string type;
    int templ;

  public:
    Type(){
      type = "";
      templ = 0;
    }

    Type(const Type& t){
      type = t.type;
      templ = t.templ;
    }

    Type(string type){
      this->type = type;
      templ = 0;
    }

    Type(string type, int templ){
      this->type = type;
      this->templ = templ;
    }

    ~Type(){}

    Type& operator=(const Type& t){
      type = t.type;
      templ = t.templ;
    }

    string getType()const{
      return type;
    }

    bool isTemplate()const{
      return templ != 0;
    }

    int getTempl()const{
      return templ;
    }

    bool isIncludedOn(const Type& t)const{
      if(t.getType() == getType())
        return true;
      else if(t.getType() == "number" && (getType() == "int" || getType() == "double"))
        return true;
      else
        return false;
    }

    bool operator==(const Type& t)const{
      return t.getType() == getType();
    }

    bool operator<(const Type& t)const{
      return t.getType() < getType();
    }

    string toString()const{
      if(templ != 0)
        return "T" + to_string(templ);
      else
        return type;
    }
};

ostream& operator<<(ostream& os, const Type& t){
  os << t.toString();
  return os;
}

class Variable{
  private:
    string name, init_value;
    bool ref, cnst;
    Type type;

  public:
    Variable(){
      name = init_value = "";
      ref = cnst = false;
    }

    Variable(string type, string name, bool ref=0, bool cnst=false, int templ=0, string init_value = ""){
      this->type = Type(type, templ);
      this->name = name;
      this->ref = ref;
      this->cnst = cnst;
      this->init_value = init_value;
    }

    Variable(const Variable& v){
      type = v.type;
      name = v.name;
      ref = v.ref;
      cnst = v.cnst;
      init_value = v.init_value;
    }

    ~Variable(){}

    Variable& operator=(const Variable& v){
      type = v.type;
      name = v.name;
      ref = v.ref;
      cnst = v.cnst;
      init_value = v.init_value;
    }

    string getName()const{
      return name;
    }

    Type getType()const{
      return type;
    }

    void setType(const Type& t){
      type = t;
    }

    bool getRef()const{
      return ref;
    }

    bool getConst()const{
      return cnst;
    }

    string getInitValue()const{
      return init_value;
    }

    void setInitValue(string init_value){
      this->init_value = init_value;
    }

    int getTempl()const{
      return type.getTempl();
    }

    string toString()const{
      string to_string;
      if(getConst())
        to_string += "const ";
      to_string += getType().toString();
      if(getRef())
        to_string += "&";
      to_string += " " + getName();
      return to_string;
    }
};

ostream& operator<<(ostream& os, const Variable& v){
  os << v.toString();
  return os;
}
class Function{
  private:
    Type type;
    string name;
    bool mod;
    vector<Variable> args;
    string code;

  public:
    Function(){
      name = "";
      mod = false;
      code = "";
    }

    Function(const Function& f){
      type = f.type;
      mod = f.mod;
      args = f.args;
      name = f.name;
      code = f.code;
    }

    Function(string type, string name, vector<Variable> args, bool mod){
      this->type = Type(type);
      this->name = name;
      this->args = args;
      this->mod = mod;
      code = "";
    }

    Function(string type, string name, vector<Variable> args, bool mod, string code){
      this->type = Type(type);
      this->name = name;
      this->args = args;
      this->mod = mod;
      this->code = code;
    }

    Function(Type type, string name, vector<Variable> args, bool mod){
      this->type = type;
      this->name = name;
      this->args = args;
      this->mod = mod;
      code = "";
    }

    Function(Type type, string name, vector<Variable> args, bool mod, string code){
      this->type = type;
      this->name = name;
      this->args = args;
      this->mod = mod;
      this->code = code;
    }

    ~Function(){}

    Function& operator=(const Function& f){
      type = f.type;
      mod = f.mod;
      args = f.args;
      name = f.name;
      code = f.code;
    }

    vector<Variable> getArgs()const{
      return args;
    }

    bool getMod()const{
      return mod;
    }

    Type getType()const{
      return type;
    }

    string getName()const{
      return name;
    }

    string getCode()const{
      return code;
    }

    int getNumberOfTemplates()const{
      int max = 0;
      if(type.getTempl() > max)
        max = type.getTempl();
      for(vector<Variable>::const_iterator it = args.begin(); it != args.end(); ++it){
        if(it->getTempl() > max)
          max = it->getTempl();
      }
      return max;
    }
};

class VariableCollector{
  private:
    map<Type, vector<Variable> > collector;
    vector<Type> types;

  public:
    VariableCollector(){}

    VariableCollector(const VariableCollector& v):collector(v.collector),types(v.types){}

    ~VariableCollector(){}

    VariableCollector& operator=(const VariableCollector& v){
      collector = v.collector;
      types = v.types;
    }

    vector<Variable> operator[](Type t)const{
      map<Type, vector<Variable> >::const_iterator it = collector.find(t);
      if(it != collector.end())
        return it->second;
      else
        return vector<Variable>();
    }

    void addVariable(const Variable& v){
      collector[v.getType()].push_back(v);
      if(find(types.begin(), types.end(), v.getType()) == types.end())
        types.push_back(v.getType());
    }

    void addVariable(string type, string name, string init_value = ""){
      Variable v(type, name, false, false, 0, init_value);
      addVariable(v);
    }

    typedef map<Type, vector<Variable> >::iterator iterator;
    typedef map<Type, vector<Variable> >::const_iterator const_iterator;

    iterator begin(){
      return collector.begin();
    }

    const_iterator begin()const{
      return collector.begin();
    }

    iterator end(){
      return collector.end();
    }

    const_iterator end()const{
      return collector.end();
    }

    vector<Type> getTypes()const{
      return types;
    }

    bool isEmpty()const{
      return collector.empty();
    }

    string toString()const{
      string code;

      for(const_iterator it = begin(); it != end(); ++it){
        code += it->first.getType() + " ";
        for(vector<Variable>::const_iterator it_var = it->second.begin(); it_var != it->second.end(); ++it_var){
          code += it_var->getName();
          string init_value = it_var->getInitValue();
          if(init_value == "")
            code += ", ";
          else
            code += " = " + init_value + ", ";
        }
        code.pop_back();
        code.pop_back();
        code += ";\n";
      }
      return code;
    }

    string initValuesToString()const{
      string code;

      for(const_iterator it = begin(); it != end(); ++it){
        for(vector<Variable>::const_iterator it_var = it->second.begin(); it_var != it->second.end(); ++it_var){
          string init_value = it_var->getInitValue();
          if(init_value != "")
            code +=  it_var->getName() + " = " + init_value + ", ";
        }
        code.pop_back();
        code.pop_back();
        code += ";\n";
      }
      return code;
    }
};

class Struct{
  private:
    string name;
    vector<Variable> vars;
    vector<Function> methods;

  public:
    Struct(){
      name = "";
    }

    Struct(const Struct& s):name(s.name),vars(s.vars),methods(s.methods){}

    ~Struct(){}

    Struct& operator=(const Struct& s){
      name = s.name;
      vars = s.vars;
      methods = s.methods;
    }

    vector<Variable> getVariables()const{
      return vars;
    }

    vector<Function> getMethods()const{
      return methods;
    }

    string getName()const{
      return name;
    }
};

struct FunctionCollector{
  vector<Function> v;
};

struct Collector{
  FunctionCollector func_coll;
  VariableCollector var_coll;
  vector<string> include_coll;

  bool canCallTheFunction(const Function& f)const{
    vector<Variable> v = f.getArgs();
    bool has_vars = true;
    for(vector<Variable>::iterator it = v.begin(); it != v.end() && has_vars ; ++it)
      if(it->getRef())
        if(var_coll[it->getType()].empty())
          has_vars = false;
    return has_vars;
  }
};

Function specifyArguments(const Function& f, const Collector& coll){
  vector<Variable> args = f.getArgs();
  map<int, Type> templates;
  Type type = f.getType();
  int i;
  vector<Type> all_types = coll.var_coll.getTypes();

  if(type.isTemplate()){
    i = rand()%all_types.size();
    templates.insert(pair<int, Type>(type.getTempl(), all_types[i]));
    type = all_types[i];
  }
  for(vector<Variable>::iterator it = args.begin(); it != args.end(); it++){
    type = it->getType();
    if(type.isTemplate()){
      if(templates.find(type.getTempl()) == templates.end()){
        vector<Type> all_types = coll.var_coll.getTypes();
        i = rand()%all_types.size();
        templates.insert(pair<int, Type>(type.getTempl(), all_types[i]));
      }
      it->setType(templates[type.getTempl()]);
    }
  }
  return Function(type, f.getName(), args, f.getMod());
}



string mainCode(const string& code){
  string main_code = "int main(){\n" + code + "\n}";
  return main_code;
}

ostream& operator<<(ostream& os, const Function& f){
  if(f.getCode() == "")
    return os;
  else{
    int number_of_templ = f.getNumberOfTemplates();
    if(number_of_templ > 0){
      os << "template <";
      for(int i = 1; i < number_of_templ; i++)
        os << "class T" << to_string(i) << ", ";
      os << "class T" << to_string(number_of_templ) << ">\n";
    }
    os << f.getType() << " " << f.getName() << "(";
    vector<Variable> args = f.getArgs();
    string args_string;
    for(vector<Variable>::const_iterator it = args.begin(); it != args.end(); ++it)
      args_string += it->toString() + ",";
    args_string.pop_back();
    os << args_string << "){\n" << f.getCode() << "}\n";
    return os;
  }
}

ostream& operator<<(ostream& os, const VariableCollector& v){
  os << v.toString();
  return os;
}

ostream& operator<<(ostream& os, const FunctionCollector& f){
  for(vector<Function>::const_iterator it = f.v.begin(); it != f.v.end(); it++)
    os << (*it);

  return os;
}

bool writeCompleteCode(ostream& os, const Collector& c, const string& code){
  vector<string> v = c.include_coll;
  for(vector<string>::const_iterator it = v.begin(); it != v.end(); ++it)
    os << "#include \"" << *it << "\"\n";
  os << "\n" << c.func_coll;
  os << "\nint main(){\n" << c.var_coll << "\n" << code << "\n}\n";
  return true;
}


#endif
