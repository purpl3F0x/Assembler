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

#define SIZE 65536
#define WORD 16
#define OPCODE_SIZE 7


/*---------------namespace rules-------------*/

namespace rules {

namespace x3 = boost::spirit::x3;
namespace ascii = boost::spirit::x3::ascii;


using std::vector;

using x3::_attr;
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

const vector<std::string> types = {"int", "bool", "char", "float"};

auto const name = x3::rule<class name, std::string>{}
                      = lexeme[char_("a-zA-Z") >> *char_("a-z_A-Z0-9")];

/*-------------------------------------------*/
/*--------------Section Parser---------------*/

auto const data = x3::rule<class data, std::string>{}
                      = lexeme["section "] >> lexeme[".data"];

auto const text = x3::rule<class text, std::string>{}
                      = lexeme["section "] >> lexeme[".text"];

auto const start = x3::rule<class start, std::string>{}
                       = lexeme["section "] >> lexeme[".start"];

/*-------------------------------------------*/
/*-------------Data type parsers-------------*/

auto const bin_val = x3::rule<class bin_val, std::string>{}
                         = lexeme[lit("0b") >> +char_("0-1")];

auto const hex_val = x3::rule<class hex_val, std::string>{}
                         = lexeme[char_("0") >> char_("x") >> +char_("0-9a-fA-F")];

auto const chr = x3::rule<class str, std::string>{}
                     = lexeme[char_("\'") >> *(ascii::char_ - '\'') >> char_("\'")];

auto const boolean = x3::rule<class boolean, std::string>{}
                         = lexeme[lit("true") | lit("false")];

//auto const floating

auto const value = x3::rule<class declaration, std::string>{}
                       = hex_val | bin_val | chr | (+char_("0-9")) | boolean;

/*-------------------------------------------*/
/*------------------Comment------------------*/

auto const comment = x3::omit[
    "//" >> *(char_ - eol)
        | "/*" >> *(char_ - "*/") >> "*/"
];

}

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

  short opCode(std::string s);

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
  map<std::string, short> opCodes;       // List off all opCodes
  map<std::string, short> numOfArgs;     // Number of Arguments of each opCode
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
