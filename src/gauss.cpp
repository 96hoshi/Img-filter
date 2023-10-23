#include <iostream>
#include "filter.h"
#include "gauss.h"

/*
    Subclass that generates the Gauss kernel.
*/

Gauss::Gauss(int size)
{
    _size = size;
    _description = "Gauss Filter";
    _kernel = createKernel();
}

std::vector<std::vector<float>> Gauss::createKernel()
{
    float sigma = 1.0;
    float r = 0;
    float s = 2.0 * sigma * sigma;
    float sum = 0.0;
    std::vector<std::vector<float>> K(_size);
    for (int i = (1 - _size) / 2; i <= (_size - 1) / 2; i++) {
        for (int j = (1 - _size) / 2; j <= (_size - 1) / 2; j++) {
            r = i * i + j * j;
            float val = exp(-r / s) / (M_PI * s);
            K[i + (_size - 1) / 2].push_back(val);
            sum += val;
        }
    }
    for (auto &row : K)
        for (auto &col : row)
            {col /= sum;}
    return K;
}
