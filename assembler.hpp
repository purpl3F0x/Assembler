/*  assembler.hpp
 *
 *
 *  Created by Stavros Avramdis on 7/1/18.
*/

#ifndef UNTITLED_ASSEMBLER_HPP
#define UNTITLED_ASSEMBLER_HPP

#include <boost/fusion/adapted.hpp>
#include <boost/spirit/home/x3.hpp>

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
using x3::lexeme;
using x3::lit;
using x3::parse;
using x3::phrase_parse;
using x3::skip;

using ascii::space;

class Assembler {

 public:
 Assembler(bool mode);

 int opCode(string s);

 int Register(string s);

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

 void translate();

 void printErrors();

 friend string tobinary(int, int);

 private:
 struct instruction {
   string opCode;
   vector<string> arguments;
   int size;

   instruction();

   void add(string s);

   void add(vector<std::string> &v);
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

 // All maps provιde binary value of each type
 map<string, int> opCodes;       // List off all opCodes
 map<string, int> numOfArgs;     // Number of Arguments of each opCode
 map<string, int> reg;           // Registers
 map<string, int> variables;     // Variable Stack
 map<string, int> consts;        // Const Stack

 vector<instruction> instructions;
 vector<error> errors;
 vector<string> out;

 bool isEmptyLine(const string &line);

 bool isData(const string &line);

 bool isText(const string &line);

 bool isStart(const string &line);

 bool lineParser(const string &line);

 bool parser(const string &s);

 bool parser(ifstream &fs);

};

}

#endif //UNTITLED_ASSEMBLER_H
