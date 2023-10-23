#include <iostream>
#include "filter.h"
#include "lowpass.h"

/*
    Subclass that generates the LowPass kernel.
*/

LowPass::LowPass(int size)
{
    _size = size;
    _description = "LowPass Filter";
    _kernel = createKernel();
}

std::vector<std::vector<float>> LowPass::createKernel()
{
    const float k = 1 / static_cast<float>(_size * _size);
    std::vector<std::vector<float> > K(_size, std::vector<float>(_size, k));

    return K;
}
