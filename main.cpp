/*  assembler.cpp
 *
 *  Created by Stavros Avramidis on 7/1/18.
*/

#include <iostream>

#include "assembler.hpp"

int main() {
  using namespace asmbl;

  Assembler a(false);
  clock_t t;

  a.setInputFile("bench.asm");

  t = clock();    //Benchmark

  bool result = a.parseFile();

  t = clock() - t;

  cout << "File parsed in: " << t*1.0/CLOCKS_PER_SEC << (result ? " with success." : " and failed.") << endl;

  if (!result)
    cout << a.getErrors();
  else {
    t = clock();
    result = a.translate();
    cout << "Assembled in: " << t*1.0/CLOCKS_PER_SEC << (result ? " assembled successfully" : " build failed.") << endl;
  }


  return 0;
}