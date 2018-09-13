//
// Created by purpl3f0x on 9/10/18.
//

#pragma once

#ifndef ASSEMBLER_OPCODES_HPP
#define ASSEMBLER_OPCODES_HPP

#include <boost/bind.hpp>
#include <boost/format.hpp>

#include <algorithm>
#include <string>
#include <vector>

struct opCode {
  unsigned short binary:7;
  std::string name;
  unsigned int numOfArgs;

  opCode(std::string n, unsigned short b, unsigned char noa)
      : binary{b}, name{n}, numOfArgs{noa} {}

  friend std::ostream &operator<<(std::ostream &out, const opCode &op) {

    out << (boost::format("%02x %5s (%d)")%op.binary%op.name%op.numOfArgs).str();
//    std::stringstream stream;
//
//    stream << std::hex << op.binary;
//    out << stream.str() << std::string("  ")
//        << op.name << std::string(" (") << std::to_string(op.numOfArgs) << std::string(")");
    return out;
  }
};

struct OpCodes : public std::vector<opCode> {
  using std::vector<opCode>::vector;

  unsigned short getBinary(std::string name) const;

  unsigned int numOfArgs(std::string name) const;

  std::string getName(unsigned short bin) const;

  bool isOpCode(std::string name) const;

  opCode *find(std::string name);

};

extern OpCodes opCodes;

#endif //ASSEMBLER_OPCODES_HPP
