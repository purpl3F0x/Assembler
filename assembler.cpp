//
// Created by purpl3f0x on 7/1/18.
//

#include "assembler.h"


namespace asmbl {


Assembler::Assembler(bool mode = false):onDebug{mode} {

    //Basic operationns
    opCodes["NOP"] = 0b0000001;
    opCodes["LDA"] = 0b0000010;
    opCodes["LDI"] = 0b0000011;
    opCodes["JMP"] = 0b0000100;
    opCodes["MI"] = 0b0000101;
    opCodes["MO"] = 0b0000110;

    //Conditional Jumps
    opCodes["JC"] = 0b0001000;
    opCodes["JO"] = 0b0001001;
    opCodes["JG"] = 0b0001010;
    opCodes["JGE"] = 0b0001011;
    opCodes["JE"] = 0b0001100;
    opCodes["JLE"] = 0b0001101;
    opCodes["JL"] = 0b0001110;
    opCodes["JNO"] = 0b0001111;
    opCodes["JZ"] = 0b0010000;
    opCodes["JNZ"] = 0b0010001;
    opCodes["JS"] = 0b0010010;
    opCodes["JNS"] = 0b0010011;

    // ALU
    opCodes["ADD"] = 0b1000000;
    opCodes["SUB"] = 0b1000001;
    opCodes["MUL"] = 0b1000010;
    opCodes["DIV"] = 0b1000011;
    opCodes["ADDI"] = 0b1000100;
    opCodes["SUBI"] = 0b1000101;
    opCodes["MULI"] = 0b1000110;
    opCodes["DIVI"] = 0b1000111;
    //Complex ALU operations
    opCodes["SQRT"] = 0b1001000;
    opCodes["MOD"] = 0b1001001;

    // Bitwise operationns
    opCodes["AND"] = 0b1100000;
    opCodes["OR"] = 0b11000001;
    opCodes["NAND"] = 0b1100010;
    opCodes["NOR"] = 0b1100011;
    opCodes["NOT"] = 0b1100100;
    opCodes["XOR"] = 0b1100101;
    opCodes["XOR"] = 0b1100110;
    opCodes["SHL"] = 0b1100111;
    opCodes["SHR"] = 0b1101000;
    opCodes["ROT"] = 0b1101001;

    //FPU
    opCodes["FADD"] = 0b1110000;
    opCodes["FSUB"] = 0b1110001;
    opCodes["FMUL"] = 0b1110010;
    opCodes["FDIV"] = 0b1110011;
    opCodes["FSQRT"] = 0b1110100;
    opCodes["ITOF"] = 0b1110101;
    opCodes["FTOI"] = 0b1110111;
    opCodes["FMOD"] = 0b1111000;

    //HACF -- HALT AND CATCH FIRE
    opCodes["HLT"] = 0b1111111;


    /////// Number of Arguments of each opCode
    numOfArgs["NOP"] = 0;
    numOfArgs["ADD"] = 3;


    //Registers
    reg["r1"] = 0b000;
    reg["r2"] = 0b001;
    reg["r3"] = 0b010;
    reg["r4"] = 0b011;
    reg["r5"] = 0b100;
    reg["r6"] = 0b101;
    reg["r7"] = 0b110;
    reg["r8"] = 0b111;

}


int Assembler::opCode(string s) {
    map<string, int>::const_iterator it = opCodes.find(s);

    if (it != opCodes.end()) return it->second;
    else throw logic_error("Got \" " + s + "\" expected an opcode");
}

int Assembler::Register(string s) {
    map<string, int>::const_iterator it = reg.find(s);
    if (it != reg.end()) return it->second;
    else throw logic_error(s + " is not a  register");
}

int Assembler::operator[](string s) {

    try {
        return opCode(s);
    } catch (logic_error) {}
    try {
        return Register(s);
    } catch (logic_error) {
        throw logic_error(s + " is not an identifier");
    }

}

void Assembler::setInputFile(std::string file) {

}

string tobinary(int n, int d = 16) {
    string s;
    while (d--) {
        s = std::to_string(n % 2) + s;
        n = n >> 1;
    }
    return s;
}

/* Privare Methods */
Assembler::instruction::instruction() : size{-1} {}

void Assembler::instruction::add(std::string s) {
    if (!++size) opCode = s;
    else arguments.push_back(s);
}

void Assembler::instruction::add(vector<std::string> &v) {
    arguments.reserve(arguments.size() + v.size());       //Memory Preallocation
    arguments.insert(arguments.end(), v.begin(), v.end());
    size+= v.size();
}

Assembler::error::error(int line, int index, string type = "", string message = "", string line_text = "")
        : line{line}, index{index}, type{type}, message{message}, line_text{line_text} {
}



const string Assembler::error::get() {
    return type + " on line :" + to_string(line) + ":" + to_string(index) + "\t" + message ;
}

bool Assembler::lineParser(const string line) {

    // Define parser rules
    auto name = x3::rule<class name, std::string>{}
                    = char_("a-zA-Z") >> *char_("a-z_A-Z0-9");

    auto args_l = x3::rule<class l, std::vector<std::string> >{}
                    = " " >> (name % skip(space)[","]);

    auto comment = x3::rule<class comment>{}
                    = "//" >> *(char_ - eol);

    //string iterators
    auto iter_start = line.begin();
    auto iter_end = line.end();

    instruction inst;
    vector<string> slt; // this is just a test

    auto add = [&](auto &ctx) { inst.add(_attr(ctx)); };

    bool result = parse(
            iter_start,
            iter_end,
            name[add] >> -args_l[add] >> *(char_(" ") )
    );

    //Testing

    if ( opCodes.find( inst.opCode ) == opCodes.end() ){    // given identifier is not valid opCode
        result = false;

        errors.push_back(
                error(cur_line,
                      iter_end - iter_start,
                      "Syntax Error",
                      inst.opCode + " is not a valid opcode.",    // Example: 42 is no a valid opcode
                      line
                )
        );
    }else if (inst.size != numOfArgs[inst.opCode]){  // Check if have right number of arguments

        result = false;

        errors.push_back(
                error(cur_line,
                      iter_start - line.begin(),
                      "Logic Error",
                      inst.opCode +" expects " + to_string(numOfArgs[inst.opCode]) + " argument(s), got "  + to_string(inst.size) + ".",    // Example: ADD expects 3 arguments, got 4
                      line
                )
        );
    }

    if (iter_end != iter_start) {   // Create an error if parser

        result = false;

        errors.push_back(
                error(cur_line,
                      iter_start - line.begin(),
                      "Syntax Error",
                      "Unexpected -> " + string(iter_start,iter_end),
                      line
                )
        );
    }

    // Code used for debugging
    if (onDebug) {
        cout << inst.opCode << endl;
        for (auto i : inst.arguments) cout << "\t->" << i << std::endl;

        for (auto e : errors) {
            cout << e.get() << endl;
        }
    }

    //

    return result;
}

bool Assembler::parser(const string &s) {
    cur_line = 0;

    stringstream stream(s);
    string line;
    while (std::getline(stream, line)) {
        cur_line ++;
        lineParser(line);
    }

}

bool Assembler::parser(std::ifstream fs) {
    string line;
    bool success = true;

    while (std::getline(fs, line)) {
        cur_line++;
        if(line.size() > 0) success += lineParser(line);

    }
}

} //End of asmbl namesapce