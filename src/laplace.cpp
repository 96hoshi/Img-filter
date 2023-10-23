#include <iostream>
#include "filter.h"
#include "laplace.h"

/*
    Subclass that generates the Laplace kernel.
*/

Laplace::Laplace(int size)
{
    _size = size;
    _description = "Laplace Filter";
    _kernel = createKernel();
}

std::vector<std::vector<float>> Laplace::createKernel()
{
    std::vector<std::vector<float>> K(_size);
    for (int i = 0; i <_size; i++) {
        for (int j = 0; j < _size; j++) {
            if (j == i || j == _size - i - 1 || j == (_size - 1) / 2 || i == (_size - 1) / 2) {
                K[i].push_back(-1);
            } else {
                K[i].push_back(0);
            }
        }
    }
    K[(_size - 1) / 2][(_size - 1) / 2] = 4 * _size - 4;
    return K;
}
