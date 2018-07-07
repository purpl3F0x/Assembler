#include <iostream>


#include "assembler.h"




int main() {
    using namespace asmbl;

    Assembler a(true);

    a.setInputFile("test.asm");
    bool result = a.parseFile();


    cout << boolalpha << result << endl;



    return 0;
}