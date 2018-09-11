//
// Created by purpl3f0x on 9/10/18.
//

#pragma once
#ifndef ASSEMBLER_OPCODES_HPP
#define ASSEMBLER_OPCODES_HPP

#include <boost/bind.hpp>

#include <algorithm>
#include <string>
#include <vector>

struct opCode {
  unsigned short binary;
  std::string name;
  unsigned char numOfArgs;

  opCode(std::string n, unsigned short b, unsigned char noa)
      : binary{b}, name{n}, numOfArgs{noa} {}
};

const std::vector<opCode> opCodes = {
    //Basic operations
    opCode("NOP", 0b0000000, 0),
    opCode("RI", 0b0000001, 0),
    opCode("LDA", 0b0000010, 0),
    opCode("LDI", 0b0000011, 0),
    opCode("STA", 0b0000100, 0),
    opCode("JMP", 0b0000101, 0),
    opCode("MI", 0b0000110, 0),
    opCode("MO", 0b0000111, 0),


    //Conditional Jumps
    opCode("JC", 0b0001000, 0),
    opCode("JO", 0b0001001, 0),
    opCode("JG", 0b0001010, 0),
    opCode("JGE", 0b0001011, 0),
    opCode("JE", 0b0001100, 0),
    opCode("JLE", 0b0001101, 0),
    opCode("JL", 0b0001110, 0),
    opCode("JNO", 0b0001111, 0),
    opCode("JZ", 0b0010000, 0),
    opCode("JNZ", 0b0010001, 0),
    opCode("JS", 0b0010010, 0),
    opCode("JNS", 0b0010011, 0),

    // ALU
    opCode("ADD", 0b1000000, 0),
    opCode("SUB", 0b1000001, 0),
    opCode("MUL", 0b1000010, 0),
    opCode("DIV", 0b1000011, 0),
    opCode("ADDI", 0b1000100, 0),
    opCode("SUBI", 0b1000101, 0),
    opCode("MULI", 0b1000110, 0),
    opCode("DIVI", 0b1000111, 0),
    //Complex ALU operations
    opCode("SQRT", 0b1001000, 0),
    opCode("MOD", 0b1001001, 0),

    // Bitwise operationns
    opCode("AND", 0b1100000, 0),
    opCode("OR", 0b11000001, 0),
    opCode("NAND", 0b1100010, 0),
    opCode("NOR", 0b1100011, 0),
    opCode("NOT", 0b1100100, 0),
    opCode("XOR", 0b1100101, 0),
    opCode("XOR", 0b1100110, 0),
    opCode("SHL", 0b1100111, 0),
    opCode("SHR", 0b1101000, 0),
    opCode("ROT", 0b1101001, 0),
    opCode("CMP", 0b1101010, 0),

    //FPU
    opCode("FADD", 0b1110000, 0),
    opCode("FSUB", 0b1110001, 0),
    opCode("FMUL", 0b1110010, 0),
    opCode("FDIV", 0b1110011, 0),
    opCode("FSQRT", 0b1110100, 0),
    opCode("ITOF", 0b1110101, 0),
    opCode("FTOI", 0b1110111, 0),
    opCode("FMOD", 0b1111000, 0),

    //HACF -- HALT AND CATCH FIRE
    opCode("HLT", 0b1111111, 0),

};

unsigned short getBinary(std::string name) {
  for (auto op: opCodes) {
    if (op.name==name)
      return op.binary;
  }
  return 0; //returns NOP
}

bool isOpCode(std::string name) {
  for (auto op: opCodes) {
    if (op.name==name)
      return true;
  }
  return false;
}

unsigned char getNumOfArgs(std::string name) {
  for (auto op: opCodes) {
    if (op.name==name)
      return op.numOfArgs;
  }
  return 0;
}

#endif //ASSEMBLER_OPCODES_HPP
