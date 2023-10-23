#ifndef IMAGE_H_
#define IMAGE_H_

#include <string>
#include <opencv2/opencv.hpp>


class Image {
private:
    std::string _path;
    cv::Mat _src;

public:
    void setPath(std::string path);
    std::string getPath();
    cv::Mat getSrc();
};

#endif /* IMAGE_H_ */
