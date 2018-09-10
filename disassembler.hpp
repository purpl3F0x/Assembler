/* disassembler.hpp
 *
 * /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
 * --------------------------------------------------------
 * Maybe later :P
 * --------------------------------------------------------
 * \/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
 *
 *  Created by Stavros Avramidis on 7/1/18
 *  Published under MIT License
*/
#ifndef ASSEMBLER_DISASSEMBLER_HPP
#define ASSEMBLER_DISASSEMBLER_HPP

// Boost libs
#include <boost/fusion/adapted.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/phoenix.hpp>

// STL libs (Yeah I know that's redundant)
#include <algorithm>
#include <bitset>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// Project header files
#include "assembler.hpp"

namespace disasmbl {

using std::cout;
using std::endl;
using std::ifstream;
using std::vector;
using std::ios;

class disassembler {
 public:
  disassembler();
  disassembler(std::string infile);

  void setFile(std::string infile);

  bool disassemble(bool silent = true);

 private:
  vector<short> instructions;
  std::string infile;

};

} // end of disasmbl namsespace

#endif //ASSEMBLER_DISASSEMBLER_HPP
