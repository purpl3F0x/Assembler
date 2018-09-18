/*  opcodes.hpp
 *
 *  Created by purpl3f0x on 9/10/18.
 *  Stavros Avramidis
*/

#pragma once

#ifndef ASSEMBLER_OPCODES_HPP
#define ASSEMBLER_OPCODES_HPP

#include <boost/bind.hpp>
#include <boost/format.hpp>

#include <algorithm>
#include <string>
#include <vector>


/*-------------------------------------------*/
/*--------------    OpCode    ---------------*/
struct opCode {
  unsigned int binary:7;
  std::string name;
  unsigned int numOfArgs;

  opCode(std::string n, unsigned short b, unsigned char noa)
      : binary{b}, name{n}, numOfArgs{noa} {}

  friend std::ostream &operator<<(std::ostream &out, const opCode &op) {
    out << (boost::format("%02x %5s (%d)")%op.binary%op.name%op.numOfArgs);
    return out;
  }
};


/*-------------------------------------------*/
/*--------------    OpCodes   ---------------*/


struct OpCodes : public std::vector<opCode> {
  using std::vector<opCode>::vector;

  unsigned int getBinary(std::string name) const;

  unsigned int numOfArgs(std::string name) const;

  std::string getName(unsigned short bin) const;

  bool isOpCode(std::string name) const;

  opCode *find(std::string name);

  int operator[](std::string name);

  std::string operator[](int bin);
};

extern OpCodes opCodes;

#endif //ASSEMBLER_OPCODES_HPP
