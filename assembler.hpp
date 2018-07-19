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

// Boost libs (tested on boost 1.64 & 1.65 revisions)
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


/*---------------namespace asmbl-------------*/

namespace rules /* Define parser rules*/ {
using namespace std;    /*Please fix this ,just don't do it*/

namespace x3 = boost::spirit::x3;
namespace ascii = boost::spirit::x3::ascii;

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

const vector<string> types = {"int", "bool", "char", "float"};

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

using namespace std;

namespace x3 = boost::spirit::x3;
namespace ascii = boost::spirit::x3::ascii;

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

  short opCode(string s);

  short Register(string s);

  void setInputFile(string file);

  bool parseString(const string &s) { return parser(s); }

  bool parseFile() {
    ifstream infile;
    infile.open(cur_file);
    if (onDebug) {
      cout << "Opening " << cur_file << " ..." << endl;
      if (infile) cout << "Ok\n\n";
      else cout << "Cannot Open File\n\n";
    }
    return parser(infile);
  }

  bool translate(string outName = "");

  std::string getErrors() const;

 private:
  struct instruction {
    string opCode;
    vector<string> arguments;
    int size;

    instruction();

    void add(string s);

    void add(vector<std::string> &v);
  };

  struct data_type {
    string type;
    string name;
    string value;
  };

  struct error {
    int line;
    int index;
    string type;
    string message;
    string line_text;

    error(int line, int index, string type, string message, string line_text);

    const string get();
  };

  int cur_line;
  string cur_file;
  bool onDebug;

  std::vector<short> rom;

  // All maps provιde binary value of each type
  map<string, short> opCodes;       // List off all opCodes
  map<string, short> numOfArgs;     // Number of Arguments of each opCode
  map<string, short> reg;           // Registers
  map<string, short> data;          // Variable Stack
  map<string, short> text;          // Const Stack

  vector<instruction> instructions;
  vector<error> errors;
  vector<string> out;

  bool isEmptyLine(const string &line);

  bool isData(const string &line);

  bool isText(const string &line);

  bool isStart(const string &line);

  int to_int(std::string &s);

  bool data_typeParser(const string &line, map<string, short> &stack);

  bool lineParser(const string &line);

  bool parser(const string &s);

  bool parser(ifstream &fs);

};

}

#endif // end of definition ASSEMBLER_H
