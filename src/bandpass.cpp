#include <iostream>
#include "filter.h"
#include "bandpass.h"
#include "identity.h"
#include "highpass.h"
#include "lowpass.h"

/*
    Subclass that generates the BandPass kernel.
*/

BandPass::BandPass(int size)
{
    _size = size;
    _description = "BandPass Filter";
    _kernel = createKernel();
}

std::vector<std::vector<float>> BandPass::createKernel()
{
    Identity ide(_size);
    LowPass lp(_size);
    HighPass hp(_size);

    auto K = subtract_matrix(subtract_matrix(ide.getKernel(), hp.getKernel()), lp.getKernel());

    return K;
}

template<typename T>
std::vector<std::vector<T>> BandPass::subtract_matrix( const std::vector<std::vector<T>>& a, const std::vector<std::vector<T>>& b)
{
    int N = a[0].size();

    std::vector<std::vector<T>> diff(N);
    for (auto i = 0; i < N; ++i) {
        for (auto j = 0; j < N; ++j) {
            diff[i].push_back(a[i][j] - b[i][j]);
        }
    }
    return diff;
}
