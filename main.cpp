/*  assembler.cpp
 *
 *  Created by Stavros Avramidis on 7/1/18.
*/

#include <iostream>

// C libraries

#include "assembler.hpp"

int main() {
  using namespace asmbl;


  Assembler a(true);

  a.setInputFile("bench.asm");

  //Bench mark time
  clock_t t;
  t = clock();

  bool result = a.parseFile();

  t = clock() - t;

  cout << "File parsed in: " << t*1.0/CLOCKS_PER_SEC << " (s) and returned " << boolalpha << result << endl;

  return 0;
}