/*
    assembler.h

    Created by Stavros Avramdis on 7/1/18.
*/

#ifndef UNTITLED_ASSEMBLER_H
#define UNTITLED_ASSEMBLER_H


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

using x3::double_;
using x3::phrase_parse;
using x3::_attr;
using x3::parse;
using x3::lit;
using x3::char_;
using x3::lexeme;
using x3::alpha;
using x3::alnum;
using x3::skip;
using ascii::space;

class Assembler {
public:
    Assembler();

    int opCode(string s);

    int Register(string s);

    int operator[](string s);

    void foo(const string& s){ parser(s);}

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
    };

    struct error {
        int line;
        int index;
        string type;
        string message;
        string line_text;

        error(int line, int index, string type, string message, string line_text);

        friend ostream &operator<<(ostream& out ,const error e);

    };


    int cur_line;

    map<string, int> opCodes;
    map<string, int> reg;
    map<string, int> variables;

    vector<instruction> instructions;
    vector<error> errors;
    vector<string> out;

    bool lineParser(const string line);

    bool parser(const string& s);
    bool parser(ifstream fs);

};

}


#endif //UNTITLED_ASSEMBLER_H
