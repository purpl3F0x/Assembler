/*  disassembler.cpp
 *
 *  Created by purpl3f0x on 9/10/18.
 *  Stavros Avramidis
*/


#include "disassembler.hpp"

namespace disasmbl {

Disassembler::Disassembler() {
  infile = "";
}

Disassembler::Disassembler(std::string infile) {
  this->infile = infile;
}

void Disassembler::setFile(std::string infile) {
  this->infile = infile;
}

bool Disassembler::disassemble(bool silent) {
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

    cout << (boost::format("%04x : %s \n")%adr%opCodes.getName(adr >> WORD - OPCODE_SIZE));

    if (!adr) {
      count++;
      if (count > 5) break;
    } else {
      count = 0;
    }
  }

  input.close();

  return true;
}

} //End of disasmbl namesapce


