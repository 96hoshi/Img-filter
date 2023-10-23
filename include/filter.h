#ifndef FILTER_H_
#define FILTER_H_

#include <iostream>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

/*
    Filter interface.
    All the subclasses are real Image Processing Filters described by
    their _kernel (odd square matrix)
*/

class Filter {
protected:
    int _size;
    std::string _description;
    std::vector<std::vector<float>> _kernel;

    virtual std::vector<std::vector<float>> createKernel() = 0;

public:
    std::vector<std::vector<float>> getKernel()
    {
        return _kernel;
    }

    std::string getDescription()
    {
        return _description;
    }
};

#endif /* FILTER_H_ */
