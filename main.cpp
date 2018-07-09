#include <iostream>


#include "assembler.hpp"




int main() {
    using namespace asmbl;

    Assembler a(true);

    a.setInputFile("test.asm");
    bool result = a.parseFile();


    cout << boolalpha << result << endl;


    return 0;
}