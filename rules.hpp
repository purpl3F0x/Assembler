/*  assembler.cpp
 *
 *  Created by purpl3f0x on 9/16/18.
 *  Stavros Avramidis
*/

#ifndef ASSEMBLER_RULES_HPP
#define ASSEMBLER_RULES_HPP

// Boost libs
#include <boost/format.hpp>
#include <boost/fusion/adapted.hpp>
#include <boost/spirit/home/x3.hpp>

// STL libs
#include <string>
#include <vector>


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

const vector<std::string> types
    = {"int", "bool", "char", "float"};

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

} // end of rules namespace


#endif //ASSEMBLER_RULES_HPP
