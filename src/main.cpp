#include "userInterface.h"
#include "test.h"

/*
    Image Filter Project.
    Authors:
        Marta Lo Cascio
        Qiyang Zong
    Class:
        Advanced Programming
*/

int main() {
    #ifdef BENCHMARK
        Test t;
        t.testConvolution();
        t.testConvolutionOpt();
    #else
        UserInterface interface;
        interface.execute();
    #endif

    return 0;
}
