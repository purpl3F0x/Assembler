#include <iostream>
#include <string>
#include <fstream>



#include "assembler.h"




int main() {
    using namespace asmbl;
    Assembler a(true);

    string test = "ADD r1,x2,r4\n"
                  "j\n"
                  "ADD r2,r3,r2 \n"
                  "NOP r1,";

    bool result = a.parseString(test);

    cout<<endl;
    cout<< boolalpha<<result <<endl;


    return 0;
}