#include <iostream>
#include <string>




#include "assembler.h"




int main() {
    using namespace asmbl;
    Assembler a;

    string test = "Hello\n my\n         \n name\n is\n Stavros\n";

    a.foo(test);


    cout<<endl;
    return 0;
}