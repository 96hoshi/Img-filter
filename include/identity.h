#ifndef IDENTITY_H_
#define IDENTITY_H_

#include "filter.h"

class Identity: public Filter {
public:
    Identity(int size);

protected:
    std::vector<std::vector<float>> createKernel() override;
};

#endif /* IDENTITY_H_ */
