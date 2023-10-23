#include <iostream>
#include "filter.h"
#include "highpass.h"

/*
    Subclass that generates the HighPass kernel.
*/

HighPass::HighPass(int size)
{
    _size = size;
    _description = "HighPass Filter";
    _kernel = createKernel();
}

std::vector<std::vector<float>> HighPass::createKernel()
{
    std::vector<std::vector<float>> K(_size);
    for (auto i = 0; i < _size; ++i) {
        for (auto j = 0; j < _size; ++j) {
            if ((i == _size / 2) || (j == _size / 2)) {
                K[i].push_back(-1);
            } else {
                K[i].push_back(0);
            }
        }
    }
    K[_size / 2][_size / 2] = (_size * 2) - 1;

    return K;
}
