#ifndef BANDPASS_H_
#define BANDPASS_H_

#include "filter.h"

class BandPass: public Filter {
public:
    BandPass(int size);

protected:
    std::vector<std::vector<float>> createKernel() override;

private:
template<typename T>
std::vector<std::vector<T>> subtract_matrix( const std::vector<std::vector<T>>& a, const std::vector<std::vector<T>>& b);
};

#endif /* BANDPASS_H_ */
