#ifndef LAPLACE_H_
#define LAPLACE_H_

#include "filter.h"

class Laplace: public Filter {
public:
    Laplace(int size);

protected:
    std::vector<std::vector<float>> createKernel() override;
};

#endif /* LAPLACE_H_ */
