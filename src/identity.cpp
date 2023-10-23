#include <iostream>
#include "filter.h"
#include "identity.h"

/*
    Subclass that generates the Identity kernel.
*/

Identity::Identity(int size)
{
    _size = size;
    _description = "Identity Filter";
    _kernel = createKernel();
}

std::vector<std::vector<float>> Identity::createKernel()
{
    std::vector<std::vector<float> > K(_size, std::vector<float>(_size));
    K[_size / 2][_size / 2] = 1;

    return K;
}
