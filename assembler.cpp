/*  assembler.cpp
 *
 *  Created by Stavros Avramidis on 7/1/18.
*/

#include "assembler.hpp"

namespace asmbl {

Assembler::Assembler(bool mode = false) : onDebug{mode} {
  rom.reserve(SIZE);

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

short Assembler::Register(std::string s) {
  auto it = reg.find(s);
  if (it!=reg.end()) return it->second;
  else throw std::logic_error(s + " is not a  register");
}

void Assembler::setInputFile(std::string infile) {
  cur_file = infile;
}

bool Assembler::parseFile() {
  ifstream infile;
  infile.open(cur_file);

  if (onDebug) {
    cout << "Opening " << cur_file << " ..." << endl;
    if (infile) cout << "Ok\n\n";
    else {
      cout << "Cannot Open File\n\n";
    }
  }

  if (!infile) return 0;

  return parser(infile);
}

std::string tobinary(int n, int d = 16) {
  std::string s;
  while (d--) {
    s = std::to_string(n%2) + s;
    n = n >> 1;
  }
  return s;
}

bool Assembler::translate(std::string outName) {
  //get file name
  if (outName=="")
    outName =
        cur_file.substr(0, cur_file.find_last_of("."))
            + ".bin";

  if (onDebug) cout << "Opening " << outName << " for writing ...\n";

  //open file for writing
  ofstream outfile;
  outfile.open(outName, ios::binary | ios::out);

  if (!outfile) {
    cout << "Cannot Open File\n\n";
    return false;
  }

  //Set All Vector values to zero
  for (int i = 0; i < SIZE; i++)
    rom.push_back(0);

  int romAddressPointer = 1;
  int ramAddressPointer = 1;

  // Initialise memory at runtime
  for (auto &d : data) {
    /* Assembles a RI routine for to fill RAM with pre-compliled values
     *
     * RI 2 args (16-bit) -> 3 ROM addresses -> 3 clk cycles
     * */

    int ri = opCodes.find("RI")->binary;
    rom[romAddressPointer++] = opCodes.getBinary("RI") << (WORD - OPCODE_SIZE);
    rom[romAddressPointer++] = ramAddressPointer++;
    rom[romAddressPointer++] = d.second;
  }


  // Instructions
  for (auto inst : instructions) {                                             // parse every instruction
    int pos = 3;
    std::vector<std::string>::iterator arg_it = inst.arguments.begin();

    rom.at(romAddressPointer) = opCodes.getBinary(inst.opCode) << (WORD - OPCODE_SIZE);

    if (!inst.arguments.size()) continue;                                    // opCodes with no args

    // Initialise Instructions taking 2 addresses lines
    if (inst.opCode=="MI" || inst.opCode=="MO" || inst.opCode=="STA" || inst.opCode=="STA") {

      for (arg_it; arg_it!=inst.arguments.end() - 1; arg_it++)               // loop though gives arguments
        rom.at(romAddressPointer) += (Register(*arg_it) << pos--*3);

      rom.at(++romAddressPointer) = Register(*arg_it);

    } else {
      for (auto arg_it : inst.arguments)
        rom.at(romAddressPointer) += (Register(arg_it) << pos--*3);
    }
    romAddressPointer++;
  }

  for (auto &i : text) {                                                     // add text
    rom.at(romAddressPointer++) = i.second;
  }

  for (auto adr : rom)                                                       // Write to file
    outfile.write(reinterpret_cast<const char *>(&adr), sizeof(uint16_t));

  outfile.close();                                                           // Close file


  return true;
}

std::string Assembler::getErrors() const {
  std::string out = "";

  for (auto e :errors) {
    out.append(e.get());
    out.append("\n");
  }

  return out;
}

/*-------------------------------------------------- */
/* ---------------- Privare Methods ---------------- */
/*-------------------------------------------------- */

Assembler::instruction::instruction() : size{-1} {}

void Assembler::instruction::add(std::string s) {
  if (!++size) opCode = s;
  else arguments.push_back(s);
}

void Assembler::instruction::add(vector<std::string> &v) {
  arguments.reserve(arguments.size() + v.size());       //Memory Pre allocation
  arguments.insert(arguments.end(), v.begin(), v.end());
  size += v.size();
}

Assembler::error::error(int line,
                        int index,
                        std::string type = "",
                        std::string message = "",
                        std::string line_text = ""
) : line{line}, index{index}, type{type}, message{message}, line_text{line_text} {}

const std::string Assembler::error::get() {
  return type + " on line " + to_string(line) + ":" + to_string(index) + "\t" + message;
}

bool Assembler::isEmptyLine(const std::string &line) {

  auto iter_start = line.begin();
  auto iter_end = line.end();

  bool success = phrase_parse(
      iter_start,
      iter_end,
      *rules::comment,
      space
  );

  return success && (iter_end==iter_start);
}

bool Assembler::isData(const std::string &line) {

  auto iter_start = line.begin();
  auto iter_end = line.end();

  bool success = parse(
      iter_start,
      iter_end,
      skip(space)[rules::data] >> *(space | rules::comment)
  );

  return success && (iter_end==iter_start);
}

bool Assembler::isText(const std::string &line) {

  auto iter_start = line.begin();
  auto iter_end = line.end();

  bool success = parse(
      iter_start,
      iter_end,
      skip(space)[rules::text] >> *(space | rules::comment)
  );

  return success && (iter_end==iter_start);
}

bool Assembler::isStart(const std::string &line) {

  auto iter_start = line.begin();
  auto iter_end = line.end();

  bool success = parse(
      iter_start,
      iter_end,
      skip(space)[rules::start] >> *(space | rules::comment)
  );

  return success && (iter_end==iter_start);
}

int Assembler::to_int(std::string &s) {
  if (s.find("0b")==0) {                            // binary
    return stoi(
        std::string(s.begin() + 2, s.end()),
        nullptr,
        2
    );
  } else if (s.find("0x")==0) {                    // Hexadecimal
    return stoi(
        std::string(s.begin() + 2, s.end()),
        nullptr,
        16
    );
  } else if (s=="true") return true;

  else if (s=="false") return false;

  else if (s[0]=='\'') return s[1];

  else return std::stoi(s);
}

bool Assembler::data_typeParser(const std::string &line, map<std::string, short> &stack) {

  short int_val = 0;

  //string iterators
  auto iter_start = line.begin();
  auto iter_end = line.end();

  // Bind
  data_type d;
  auto setType = [&](auto &ctx) { d.type = _attr(ctx); };
  auto setName = [&](auto &ctx) { d.name = _attr(ctx); };
  auto setVal = [&](auto &ctx) { d.value = _attr(ctx); };

  bool result = parse(
      iter_start,
      iter_end,
      skip(rules::comment | space)[rules::name[setType]
          >> rules::name[setName]
          >> -rules::value[setVal]]
          >> *(space | rules::comment)
  );

  if (result && iter_end!=iter_start) {
    result = false;
    errors.emplace_back(
        error(
            cur_line,
            iter_start - line.begin(),
            "Logic Error",
            "Unexpected -> " + std::string(iter_start, iter_end),
            line
        )
    );
  } else if (d.type=="int" || d.type=="bool" || d.type=="char" || d.type=="float") {

    int_val = to_int(d.value);

  } else if (opCodes.isOpCode(d.name)
      || find(rules::types.begin(), rules::types.end(), d.name)!=rules::types.end()) {
    errors.emplace_back(
        error(
            cur_line,
            line.find(d.name),
            "Logic Error",
            d.name + " is not a valid identifier",
            line
        )
    );
  } else {
    errors.emplace_back(
        error(
            cur_line,
            iter_start - line.begin(),
            "Syntax Error",
            d.type + " does not name a type",
            line
        )
    );
    result = false;
  }

  if (onDebug) cout << d.type << ": " << d.name << "\n\t->" << d.value << " (" << int_val << ")" << endl;

  if (result) stack[d.name] = int_val;

  return result;

}

bool Assembler::lineParser(const std::string &line) {

  //string iterators
  auto iter_start = line.begin();
  auto iter_end = line.end();

  instruction inst;

  // Bind
  auto add = [&](auto &ctx) { inst.add(_attr(ctx)); };

  bool result = parse(
      iter_start,
      iter_end,
      skip(rules::comment | space)[rules::name[add]
          >> *(rules::name[add]%',')]
          >> *(space | rules::comment)
  );

  opCode *op = opCodes.find(inst.opCode);

  if (result && inst.size && !op) {    // given identifier is not valid opCode
    result = false;
    cout << opCodes.isOpCode(inst.opCode) << endl;
    errors.emplace_back(
        error(cur_line,
              iter_end - iter_start,
              "Syntax Error",
              inst.opCode + " is not a valid opcode.",
              line
        )
    );
  } else if (result && inst.size!=op->numOfArgs) {                 // Check arguments number

    result = false;

    errors.emplace_back(
        error(cur_line,
              iter_start - line.begin(),
              "Logic Error",
              inst.opCode + " expects " + to_string(op->numOfArgs) + " argument(s), got " +
                  to_string(inst.size) + ".",    // Example: ADD expects 3 arguments, got 4
              line
        )
    );
  }

  if (iter_end
      !=iter_start) {                                               // Create an error if parser hasn't come to end of line

    result = false;

    errors.emplace_back(
        error(cur_line,
              iter_start - line.begin(),
              "Syntax Error",
              "Unexpected -> " + std::string(iter_start, iter_end),
              line
        )
    );
  }

  // Code used for debugging
  if (onDebug) {
    cout << cur_line << ": " << inst.opCode << "(" << opCodes.numOfArgs(inst.opCode) << ")" << endl;

    for (auto i : inst.arguments)
      cout << "\t->" << i << std::endl;
  }

  // Add instruction to stack
  if (result) instructions.push_back(inst);

  return result;
}

bool Assembler::parser(const std::string &s) {

  cur_line = 0;

  stringstream stream(s);
  std::string line;
  while (std::getline(stream, line)) {
    cur_line++;
    lineParser(line);
  }

}   // I might fix this later, depreciated for now

bool Assembler::parser(ifstream &fs) {

  bool parseData = false;
  bool parseText = false;
  bool parseStart = false;

  cur_line = 0;
  std::string line;
  bool success = true;

  while (std::getline(fs, line)) {

    cur_line++;

    if (isEmptyLine(line)) continue;          //skip empty lines

    if (isData(line)) {
      if (parseData) {                        // Return Error if Already have parsed Data section
        errors.emplace_back(
            error(cur_line,
                  line.find('s'),             // Find where section starts
                  "Logic Error",
                  "Redefinition of .data ,already defined at " + std::to_string(parseData),
                  line
            )
        );
        success = false;
      } else parseData = true;
      continue;
    }

    if (isText(line)) {
      if (parseText) {                           // Return Error if Already have parsed Text section
        errors.emplace_back(
            error(cur_line,
                  line.find('s'),                // Find where section starts
                  "Logic Error",
                  "Redefinition of .text ,already defined at " + std::to_string(parseData),
                  line
            )
        );
        success = false;
      } else parseText = true;
      continue;
    }

    if (isStart(line)) {
      if (parseStart) {                           // Return Error if Already have parsed Start section
        errors.emplace_back(
            error(cur_line,
                  line.find('s'),                 // Find where section starts
                  "Logic Error",
                  "Redefinition of .text ,already defined at " + std::to_string(parseData),
                  line
            )
        );
        success = false;
      } else parseStart = true;
      continue;
    }

    if (parseStart)
      success = lineParser(line) && success;

    else if (parseText)
      success = data_typeParser(line, text) && success;

    else if (parseData)
      success = data_typeParser(line, data) && success;

    else {
      errors.emplace_back(                                //throw error for instructions/declerations outside of sections
          error(cur_line,
                line.find('s'),
                "Syntax Error",
                "Unexpeced outside of section",
                line
          )
      );
      success = false;
    }

  } //end of parse loop


  if (success) text[".start"] = data.size();             // Move .start to pointer set memory values on start

  if (!success && onDebug)
    for (auto e : errors)
      cout << e.get() << endl;

  if (onDebug) {                                          // Print Variables values onDebug mode
    cout << "_______" << " data " << "_______" << endl;
    for (auto &i:data)
      cout << i.first << " : " << i.second << endl;

    cout << "_______" << " text " << "_______" << endl;
    for (auto &i:text)
      cout << i.first << " : " << i.second << endl;
  }

  return success;
}

} //End of asmbl namesapce