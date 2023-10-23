#ifndef CONVOLUTION_H_
#define CONVOLUTION_H_

#include "image.h"
#include "filter.h"
#include "laplace.h"


class Convolution {
public:
    void applyFilter(Image& srcImage, Filter& filter);
    void applyFilter(Image& srcImage, Laplace& filter);

protected:
    void applyConvolution(const cv::Mat& src, cv::Mat& dst,
                        std::vector<std::vector<float>>& kernel);
    void applyConvolutionGray(const cv::Mat& src, cv::Mat& dst,
                             std::vector<std::vector<float>>& kernel);
    void applyConvolutionOptimized(const cv::Mat& src, cv::Mat& dst,
                                std::vector<std::vector<float>>& kernel);
    void applyConvolutionGrayOptimized(const cv::Mat& src, cv::Mat& dst,
                                    std::vector<std::vector<float>>& kernel);

private:
    void displayImage(const cv::Mat& src,
                    const cv::Mat& dst,
                    const std::string description);
    void border(const cv::Mat& src, cv::Mat& dst,
                std::vector<std::vector<float>>& kernel,
                const int xstart,
                const int xend,
                const int ystart,
                const int yend);
    void handleBorders(const cv::Mat& src, cv::Mat& dst,
                    std::vector<std::vector<float>>& kernel);
    void borderGray(const cv::Mat& src, cv::Mat& dst,
                    std::vector<std::vector<float>>& kernel,
                    const int xstart,
                    const int xend,
                    const int ystart,
                    const int yend);
    void handleBordersGray(const cv::Mat& src, cv::Mat& dst,
                        std::vector<std::vector<float>>& kernel);
};

#endif /* CONVOLUTION_H_ */
