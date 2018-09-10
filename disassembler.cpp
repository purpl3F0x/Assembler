/*  disassembler.cpp
 *
 *  Created by Stavros Avramidis on 9/10/18.
*/

#include "assembler.hpp"
#include "disassembler.hpp"

namespace disasmbl {

disassembler::disassembler() {
  infile = "";
}

disassembler::disassembler(std::string infile) {
  this->infile = infile;
}

void disassembler::setFile(std::string infile) {
  this->infile = infile;
}

bool disassembler::disassemble(bool silent) {
  ifstream input(infile, ios::binary);

  if (!input) {
    cout << "Can not Open file: " << infile << endl;
    return false;
  }

  cout << "ok!" << endl;

  unsigned short adr;
  char count = 0;

  while (!input.eof()) {
    input.read(reinterpret_cast<char *>(&adr), sizeof(uint16_t));

    cout << adr
         << " : "
         << (adr >> WORD - OPCODE_SIZE)
         << endl;

    if (!adr) {
      count += 1;
      if (count > 10) break;
    } else {
      count = 0;
    }
  }

  input.close();

  return true;
}

} //End of disasmbl namesapce


