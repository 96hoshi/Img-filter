#ifndef USER_INTERFACE_H_
#define USER_INTERFACE_H_

#include <iostream>
#include "filter.h"
#include "bandpass.h"
#include "highpass.h"
#include "lowpass.h"
#include "laplace.h"
#include "gauss.h"
#include "image.h"
#include "convolution.h"


class UserInterface {
public:
    void execute();

private:
    Convolution _master;
    Image _src;
    std::unique_ptr<Filter> _chosenFilter;
    std::string _imagepath;
    int _ksize;
    char _decide;
    char _fil;

    void askImage();
    void askFilter();
    void askSize();
    void apply();
};

#endif /* USER_INTERFACE_H_ */
