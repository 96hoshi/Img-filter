#include "image.h"

/*
    Abstraction of an Image to be processed and also a filtered Image.
    The memver values that describe an image are:
    - filepath          (_path)
    - matrix of pixel   (_src)
*/
void Image::setPath(std::string path)
{
    if (path.empty()) {
        path = "../img/lena.jpg";
    }

    _src = cv::imread(path);
    while (_src.data == nullptr) {
        std::cout << "Cannot find the image, please enter a correct filepath." << std::endl;
        std::cin >> path;
        _src = cv::imread(path);
    }

    std::cout << "Processing " << path << "." << std::endl;
    _path = path;
}

std::string Image::getPath()
{
    return _path;
}

cv::Mat Image::getSrc()
{
    return _src;
}
