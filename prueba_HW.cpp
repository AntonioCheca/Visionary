#include <iostream>
#include <fstream>
#include <cstdlib>

#include "critics.h"
#include "environment.h"
#include "statements.h"
#include "basicfunctions.h"
#include "basicvars.h"
#include "cleaningcode.h"
using namespace std;

int main(){
  srand(time(NULL));
  Collector c1, c2;
  string s;
  int iter = 0;
  int max_score = 0;
  string better_code;

  addBasicOperators(c1);
  addEssentialOperators(c1);
  //addVariables(c);
  c1.var_coll.addVariable("std::string", "h", "\"Hello\"");
  c1.var_coll.addVariable("std::string", "s", "\" \"");
  c1.var_coll.addVariable("std::string", "w", "\"World!\"");
  while(max_score < 100 && iter < 1000){
    iter++;
    c2 = c1;
    string code;
    for(int i = 0; i < 3; i++)
      code += RandomLine(c2);
    eraseUnusedStuff(code, c2);
    if(!itAppears(code,"write"))
      code += LastWrite(c2);

    ofstream f("test_programa.cpp");
    if(writeCompleteCode(f, c2, code)){
      f.close();
      vector<string> other_files;
      other_files.push_back("essential_operators.h");
      if(compile("test_programa", other_files))
        execute("test_programa", "test_programa_salida", s);

      int iter_score = score("test_programa_salida", "Hello.txt");
      if(iter_score > max_score){
        max_score = iter_score;
        better_code = code;
      }
      cout << "Esta es la salida de ejecución número " << iter << " (SCORE: " << iter_score << ", BEST_SCORE: " <<  max_score << "):\n";
      cout << s << "\n";
      s.clear();
    }
  }
  if(max_score >= 100)
    cout << "Se ha llegado al código exacto que produce la salida que se ha pedido. El código se coloca a continuación:\n" << better_code << "\n";
  else
    cout << "El mejor código ha conseguido una puntuación de " << max_score << ", que ha sido el siguiente:\n" << better_code << "\n";

    /* (M[NU]) Añadir funciones de la STL como variables a los Collector.
    (M[NU]) Una función que recorra un programa y lo idente bien.
    (M) Poner los FunctionCollector como una clase bien hecha, no un struct cutre
    (M) Lo mismo con los Collector, ponerlos bien
    (H) Se podría cambiar que se hagan al menos 1000 iteraciones de los bucles a que se haga siempre una actualización de la variable de la que depende el bucle
    (U(C)) Ha habido veces que ha puesto dos veces el código en test_programa y no sé por qué! No me ha vuelto a pasar, pero cuidado , creo que es por un error en los popen al compilar, que los flujos hacen cosas extrañas
    (E) Quitar los using namespace en los .h
    (H) Poner funciones para poder comparar una tanda de programas en una sola compilación y ejecución para disminuir el cuello de botella de la compilación
    (H) Empezar a hacer las iteraciones más inteligentes y que dependan una de otra. Una idea es separar en RandomLines por sus valores independientes e intentar hacer algo estilo genético
    (M) Mirar que cuando se hace cout de un programa se pongan el número de Lines al principio del archivo para poder paralelizar luego el score
    (M) Para los cout en while se puede hacer una estimación del máximo número de iteraciones por los \n que halla, pero esto no siempre será cierto. En caso de que no coincidan el número de líneas totales con el que debería ser, se deberá anular esa iteración y colocarla en archivos diferentes
    (E) Una vez hecho lo anterior, probar con Hello World! a ver si hay mejora considerable en velocidad o nope
    ([NU]) Documentar al menos lo importante, antes del 31 de Mayo.
  */
}
