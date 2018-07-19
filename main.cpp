/*  assembler.cpp
 *
 *  Created by Stavros Avramidis on 7/1/18.
*/

#include <iostream>

#include "assembler.hpp"

int main() {
  using namespace asmbl;

  Assembler a(true);
  clock_t t;

  a.setInputFile("bench.asm");

  t = clock();    //Bench mark time

  bool result = a.parseFile();

  t = clock() - t;

  cout << "File parsed in: " << t*1.0/CLOCKS_PER_SEC << " (s) and returned " << boolalpha << result << endl;

  if (!result)
    cout << a.getErrors();
  else
    a.translate();


  return 0;
}