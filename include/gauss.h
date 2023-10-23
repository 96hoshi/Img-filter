#ifndef GAUSS_H_
#define GAUSS_H_

#include "filter.h"

class Gauss: public Filter {
public:
    Gauss(int size);

protected:
    virtual std::vector<std::vector<float>> createKernel() override;
};

#endif /* GAUSS_H_ */
