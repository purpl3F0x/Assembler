/*  opcodes.cpp
 *
 *  Created by purpl3f0x on 9/12/18.
 *  Stavros Avramidis
*/

#include "opcodes.hpp"


/*-------------------------------------------*/
/*--------------    OpCodes   ---------------*/

unsigned int OpCodes::getBinary(std::string name) const {
  for (auto op: *this)
    if (op.name==name)
      return op.binary;

  return 0; // returns NOP
}

unsigned int OpCodes::numOfArgs(std::string name) const {
  for (auto op: *this)
    if (op.name==name)
      return op.numOfArgs;

  return 0;
}

std::string OpCodes::getName(unsigned short bin) const {
  for (auto op: *this)
    if (op.binary==bin)
      return op.name;

  return "";
}

bool OpCodes::isOpCode(std::string name) const {
  for (auto op: *this)
    if (op.name==name)
      return true;

  return false;
}

opCode *OpCodes::find(std::string name) {
  for (auto &&op: *this)
    if (op.name==name)
      return &op;

  return nullptr;
}

std::string OpCodes::operator[](int bin) {
  return this->getName(bin);
}

int OpCodes::operator[](std::string name) {
  return this->getBinary(name);
}

/* Declare opCodes*/


OpCodes opCodes = {
    //Basic operations
    opCode("NOP", 0b0000000, 0),
    opCode("RI", 0b0000001, 0),
    opCode("LDA", 0b0000010, 1),
    opCode("LDI", 0b0000011, 1),
    opCode("STA", 0b0000100, 1),
    opCode("JMP", 0b0000101, 1),
    opCode("MI", 0b0000110, 2),
    opCode("MO", 0b0000111, 2),


    //Conditional Jumps
    opCode("JC", 0b0001000, 2),
    opCode("JO", 0b0001001, 2),
    opCode("JG", 0b0001010, 2),
    opCode("JGE", 0b0001011, 2),
    opCode("JE", 0b0001100, 2),
    opCode("JLE", 0b0001101, 2),
    opCode("JL", 0b0001110, 2),
    opCode("JNO", 0b0001111, 2),
    opCode("JZ", 0b0010000, 2),
    opCode("JNZ", 0b0010001, 2),
    opCode("JS", 0b0010010, 2),
    opCode("JNS", 0b0010011, 2),

    // ALU
    opCode("ADD", 0b1000000, 3),
    opCode("SUB", 0b1000001, 3),
    opCode("MUL", 0b1000010, 3),
    opCode("DIV", 0b1000011, 3),
    opCode("ADDI", 0b1000100, 3),
    opCode("SUBI", 0b1000101, 3),
    opCode("MULI", 0b1000110, 3),
    opCode("DIVI", 0b1000111, 3),
    //Complex ALU operations
    opCode("SQRT", 0b1001000, 2),
    opCode("MOD", 0b1001001, 3),

    // Bitwise operationns
    opCode("AND", 0b1100000, 3),
    opCode("OR", 0b11000001, 3),
    opCode("NAND", 0b1100010, 3),
    opCode("NOR", 0b1100011, 3),
    opCode("NOT", 0b1100100, 3),
    opCode("XOR", 0b1100101, 3),
    opCode("XOR", 0b1100110, 3),
    opCode("SHL", 0b1100111, 3),
    opCode("SHR", 0b1101000, 3),
    opCode("ROT", 0b1101001, 3),
    opCode("CMP", 0b1101010, 3),

    //FPU
    opCode("FADD", 0b1110000, 3),
    opCode("FSUB", 0b1110001, 3),
    opCode("FMUL", 0b1110010, 3),
    opCode("FDIV", 0b1110011, 3),
    opCode("FSQRT", 0b1110100, 3),
    opCode("ITOF", 0b1110101, 3),
    opCode("FTOI", 0b1110111, 3),
    opCode("FMOD", 0b1111000, 3),

    //HACF -- HALT AND CATCH FIRE
    opCode("HLT", 0b1111111, 0),

};


