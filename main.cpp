/*  assembler.cpp
 *
 *  Created by Stavros Avramidis on 7/1/18.
*/

#include <chrono>  // for high_resolution_clock
#include <iostream>

#include "assembler.hpp"
#include "disassembler.hpp"
#include "opcodes.hpp"

int main() {
  using namespace asmbl;
  using namespace disasmbl;

  using std::chrono::high_resolution_clock;

  Assembler a(false);

  a.setInputFile("test.asm");

  // Record start time
  auto start = high_resolution_clock::now();

  bool result = a.parseFile();

  // Record end time
  auto finish = high_resolution_clock::now();

  std::chrono::duration<double> elapsed = finish - start;

  cout << "File parsed in: " << elapsed.count() << (result ? " with success." : " and failed.") << endl;

  if (!result)
    cout << a.getErrors();
  else {
    start = high_resolution_clock::now();

    result = a.translate();

    finish = high_resolution_clock::now();

    elapsed = finish - start;
    cout << "Assembled in: " << elapsed.count() << (result ? " assembled successfully" : " build failed.") << endl;
  }

  // \/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/

  disassembler d("test.bin");

  bool res = d.disassemble();

  // \/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/

  cout << getBinary("JMP") << endl;
  cout << isOpCode("JMP") << endl;
  cout << getNumOfArgs("JMP") << endl;


  return 0;
}