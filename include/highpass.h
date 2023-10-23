#ifndef HIGHPASS_H_
#define HIGHPASS_H_

#include "filter.h"

class HighPass: public Filter {
public:
    HighPass(int size);

protected:
    std::vector<std::vector<float>> createKernel() override;
};

#endif /* HIGHPASS_H_ */
