#ifndef LOWPASS_H_
#define LOWPASS_H_

#include "filter.h"

class LowPass: public Filter {
public:
    LowPass(int size);

protected:
    std::vector<std::vector<float>> createKernel() override;
};

#endif /* LOWPASS_H_ */
