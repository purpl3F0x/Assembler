/*  assembler.hpp
 *
 * /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
 * --------------------------------------------------------
 * This project contains an Assembler I made in C++
 * for a project I am  making, by re-inventing the wheel,
 * and creating a 16-bit computer on an FPGA.
 * --------------------------------------------------------
 * \/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
 *
 *  Created by Stavros Avramidis on 7/1/18
 *  Published under MIT License
*/

#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

// Boost libs
#include <boost/format.hpp>
#include <boost/fusion/adapted.hpp>
#include <boost/spirit/home/x3.hpp>

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

#include "opcodes.hpp"
#include "rules.hpp"


#define SIZE 65536
#define WORD 16
#define OPCODE_SIZE 7


/*---------------namespace asmbl-------------*/

namespace asmbl {

namespace x3 = boost::spirit::x3;
namespace ascii = boost::spirit::x3::ascii;

using std::cout;
using std::endl;
using std::ifstream;
using std::ios;
using std::map;
using std::ofstream;
using std::stringstream;
using std::to_string;
using std::vector;

using x3::_attr;
using x3::alnum;
using x3::alpha;
using x3::char_;
using x3::double_;
using x3::eol;
using x3::int_;
using x3::lexeme;
using x3::lit;
using x3::parse;
using x3::phrase_parse;
using x3::skip;

using ascii::space;

class Assembler {

 public:
  Assembler(bool mode);

  short Register(std::string s);

  void setInputFile(std::string file);

  bool parseString(const std::string &s) { return parser(s); }

  bool parseFile();

  bool translate(std::string outName = "");

  std::string getErrors() const;

 private:
  struct instruction {
    std::string opCode;
    vector<std::string> arguments;
    int size;

    instruction();

    void add(std::string s);

    void add(vector<std::string> &v);
  };

  struct data_type {
    std::string type;
    std::string name;
    std::string value;
  };

  struct error {
    int line;
    int index;
    std::string type;
    std::string message;
    std::string line_text;

    error(int line, int index, std::string type, std::string message, std::string line_text);

    const std::string get();
  };

  int cur_line;
  std::string cur_file;
  bool onDebug;

  std::vector<short> rom;

  // All maps provιde binary value of each type
  map<std::string, short> reg;           // Registers
  map<std::string, short> data;          // Variable Stack
  map<std::string, short> text;          // Const Stack

  vector<instruction> instructions;
  vector<error> errors;
  vector<std::string> out;

  bool isEmptyLine(const std::string &line);

  bool isData(const std::string &line);

  bool isText(const std::string &line);

  bool isStart(const std::string &line);

  int to_int(std::string &s);

  bool data_typeParser(const std::string &line, map<std::string, short> &stack);

  bool lineParser(const std::string &line);

  bool parser(const std::string &s);

  bool parser(ifstream &fs);

};

}

#endif // end of definition ASSEMBLER_H
