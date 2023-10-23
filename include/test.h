#ifndef TEST_H_
#define TEST_H_

#include <iostream>
#include "convolution.h"


class Test: public Convolution{
public:
    void testConvolution();
    void testConvolutionOpt();
};

#endif /* TEST_H_ */
