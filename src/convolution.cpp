#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

#include "convolution.h"
#include "identity.h"
#include "bandpass.h"
#include "highpass.h"
#include "lowpass.h"
#include "laplace.h"
#include "gauss.h"

/*
    The convolution class contains all the members necessary to process an Image object.
    The main purpose is to:
    - correctly apply the filter to each pixel of the image,
    - accordingly process the borders of the image,
    - display the images to the user.
*/

void Convolution::applyFilter(Image& srcImage, Filter& filter)
{
    cv::Mat src = srcImage.getSrc();
    cv::Mat dst = cv::Mat(src.rows, src.cols, src.type());
    std::vector<std::vector<float>> kernel = filter.getKernel();
    applyConvolutionOptimized(src, dst, kernel);

    displayImage(src, dst, filter.getDescription());
}

void Convolution::applyFilter(Image& srcImage, Laplace& filter)
{
    cv::Mat src = srcImage.getSrc();
    cv::Mat src_gray;
    cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);
    cv::Mat dst = cv::Mat(src_gray.rows, src_gray.cols, src_gray.type());

    std::vector<std::vector<float>> kernel = filter.getKernel();
    applyConvolutionGrayOptimized(src_gray, dst, kernel);

    displayImage(src, dst, filter.getDescription());
}

void Convolution::displayImage(const cv::Mat& src,
                            const cv::Mat& dst,
                            const std::string description)
{
    // Display Original image
    cv::namedWindow("Display Image");
    cv::imshow("Display Image", src);
    // Display Filtered image
    cv::namedWindow(description + " Image");
    cv::imshow(description + " Image", dst);

    cv::waitKey();
    cv::destroyAllWindows();
}

void Convolution::applyConvolution(const cv::Mat& src, cv::Mat& dst,
                                std::vector<std::vector<float>>& kernel)
{
    const int ksize = kernel[0].size();
    const int channels = 3;

    handleBorders(src, dst, kernel);

    for (int x = 0; x < src.rows - ksize + 1; x++){
        for (int y = 0; y < src.cols - ksize + 1; y++){
            double sum[3] = {0, 0, 0};
            // iterate over the kernel values
            for (int i = 0; i < ksize; i++){
                for (int j = 0; j < ksize; j++){
                    cv::Vec3b pixel = src.at<cv::Vec3b>(x + i, y + j);
                    // iterate over the channels
                    for (int c = 0; c < channels; c++){
                        sum[c] += pixel.val[c] * kernel[i][j];
                    }
                }
            }
            // clamping
            for (int c = 0; c < channels; c++){
                if (sum[c] > 255){
                    sum[c] = 255;
                }
                if (sum[c] < 0){
                    sum[c] = 0;
                }
            }
            dst.at<cv::Vec3b>(x + ksize / 2, y + ksize / 2) = cv::Vec3b(sum[0], sum[1], sum[2]);
        }
    }
}

void Convolution::applyConvolutionOptimized(const cv::Mat& src, cv::Mat& dst,
                                std::vector<std::vector<float>>& kernel)
{
    const uchar * src_data = static_cast<const uchar*>(src.data);
    uchar * dst_data = static_cast<uchar*>(dst.data);
    const int rows = src.rows;
    const int cols = src.cols;
    const int channels = src.channels();

    const int ksize = kernel[0].size();

    handleBorders(src, dst, kernel);

    for (int x = 0; x < rows - ksize + 1; x++){
        for (int y = 0; y < cols - ksize + 1; y++){
            double sum[3] = {0, 0, 0};
            // iterate over the kernel values
            for (int i = 0; i < ksize; i++){
                for (int j = 0; j < ksize; j++){
                    uchar B = src_data[(x + i) * cols * channels + (y + j) * channels + 0];
                    uchar G = src_data[(x + i) * cols * channels + (y + j) * channels + 1];
                    uchar R = src_data[(x + i) * cols * channels + (y + j) * channels + 2];
                    sum[0] += B * kernel[i][j];
                    sum[1] += G * kernel[i][j];
                    sum[2] += R * kernel[i][j];
                }
            }
            // clamping and storing
            dst_data[(x + ksize / 2) * cols * channels + (y + ksize / 2) * channels + 0] =
                (sum[0] > 255.0 ? 255 : (sum[0] < 0.0 ? 0 : sum[0]));
            dst_data[(x + ksize / 2) * cols * channels + (y + ksize / 2) * channels + 1] =
                (sum[1] > 255.0 ? 255 : (sum[1] < 0.0 ? 0 : sum[1]));
            dst_data[(x + ksize / 2) * cols * channels + (y + ksize / 2) * channels + 2] =
                (sum[2] > 255.0 ? 255 : (sum[2] < 0.0 ? 0 : sum[2]));
        }
    }
}

void Convolution::handleBorders(const cv::Mat& src, cv::Mat& dst,
                                std::vector<std::vector<float>>& kernel)
{
    const int ksize = kernel[0].size();
    const int shift = ksize / 2;

    //top
    border(src, dst, kernel, 0, shift, 0, src.cols);
    //bottom
    border(src, dst, kernel, src.rows - shift, src.rows, 0, src.cols);
    //left
    border(src, dst, kernel, 0, src.rows, 0, shift);
    //right
    border(src, dst, kernel, 0, src.rows, src.cols - shift, src.cols);
}

void Convolution::border(const cv::Mat& src, cv::Mat& dst,
                        std::vector<std::vector<float>>& kernel,
                        const int xstart,
                        const int xend,
                        const int ystart,
                        const int yend)
{

    const int ksize = kernel[0].size();
    const int shift = ksize / 2;
    const int channels = 3;

    const int cols = src.cols;
    uchar * dst_data = static_cast<uchar*>(dst.data);
    uchar * src_data = static_cast<uchar*>(src.data);

    for (int x = xstart; x < xend; x++) {
        for (int y = ystart; y < yend; y++) {
            double sum[3] = {0, 0, 0};
            // iterate over the kernel values
            for (int i = -shift; i <= shift; i++) {
                for (int j = -shift; j <= shift; j++) {
                    int sx = x + i;
                    int sy = y + j;

                    sx = std::min(std::max(sx, 0), src.rows - 1);
                    sy = std::min(std::max(sy, 0), src.cols - 1);

                    // iterate over the channels
                    uchar B = src_data[sx * cols * channels + sy * channels + 0];
                    uchar G = src_data[sx * cols * channels + sy * channels + 1];
                    uchar R = src_data[sx * cols * channels + sy * channels + 2];
                    sum[0] += B * kernel[i + shift][j + shift];
                    sum[1] += G * kernel[i + shift][j + shift];
                    sum[2] += R * kernel[i + shift][j + shift];
                }
            }
            // clamping and store values
            dst_data[x  * cols * channels + y * channels + 0] =
                (sum[0] > 255.0 ? 255 : (sum[0] < 0.0 ? 0 : sum[0]));
            dst_data[x  * cols * channels + y * channels + 1] = 
                (sum[1] > 255.0 ? 255 : (sum[1] < 0.0 ? 0 : sum[1]));
            dst_data[x  * cols * channels + y * channels + 2] = 
                (sum[2] > 255.0 ? 255 : (sum[2] < 0.0 ? 0 : sum[2]));
        }
    }
}

void Convolution::applyConvolutionGrayOptimized(const cv::Mat& src, cv::Mat& dst,
                                    std::vector<std::vector<float>>& kernel)
{
    const int ksize = kernel[0].size();
    const int cols = src.cols;
    uchar * src_data = static_cast<uchar*>(src.data);
    uchar * dst_data = static_cast<uchar*>(dst.data);

    handleBorders(src, dst, kernel);

    // optimization:
    for (int x = 0; x < src.rows - ksize + 1; x++){
        for (int y = 0; y < cols - ksize + 1; y++){
            int index = x * cols + y;
            double sum = 0;
            // iterate over the kernel values
            for (int i = 0; i < ksize; i++){
                for (int j = 0; j < ksize; j++){
                    sum += src_data[index + j] * kernel[i][j];
                }
                index += cols;
            }
            // clamping and store
            dst_data[(x + ksize / 2) * cols + y + ksize / 2] = std::min(std::max(sum, 0.0), 255.0);
        }
    }
}

void Convolution::applyConvolutionGray(const cv::Mat& src, cv::Mat& dst,
                                    std::vector<std::vector<float>>& kernel)
{
    const int ksize = kernel[0].size();

    handleBorders(src, dst, kernel);

    for (int x = 0; x < src.rows - ksize + 1; x++){
        for (int y = 0; y < src.cols - ksize + 1; y++){
            double sum = 0;
            // iterate over the kernel values
            for (int i = 0; i < ksize; i++){
                for (int j = 0; j < ksize; j++){
                    sum += src.at<uchar>(x + i, y + j) * kernel[i][j];
                }
            }
            // clamping
            if (sum > 255){
                sum = 255;
            }
            if (sum < 0){
                sum = 0;
            }
            dst.at<uchar>(x + ksize / 2, y + ksize / 2) = sum;
        }
    }
}

void Convolution::handleBordersGray(const cv::Mat& src, cv::Mat& dst,
                                    std::vector<std::vector<float>>& kernel)
{
    const int ksize = kernel[0].size();
    const int shift = ksize / 2;

    //top
    borderGray(src, dst, kernel, 0, shift, 0, src.cols);
    //bottom
    borderGray(src, dst, kernel, src.rows - shift, src.rows, 0, src.cols);
    //left
    borderGray(src, dst, kernel, 0, src.rows, 0, shift);
    //right
    borderGray(src, dst, kernel, 0, src.rows, src.cols - shift, src.cols);
}

void Convolution::borderGray(const cv::Mat& src, cv::Mat& dst,
                            std::vector<std::vector<float>>& kernel,
                            const int xstart,
                            const int xend,
                            const int ystart,
                            const int yend)
    {

    const int ksize = kernel[0].size();
    const int shift = ksize / 2;
    const int cols = src.cols;

    uchar * src_data = static_cast<uchar*>(src.data);
    uchar * dst_data = static_cast<uchar*>(dst.data);

    for (int x = xstart; x < xend; x++) {
        for (int y = ystart; y < yend; y++) {
            double sum = 0;
            // iterate over the kernel values
            for (int i = -shift; i <= shift; i++) {
                for (int j = -shift; j <= shift; j++) {
                    int sx = x + i;
                    int sy = y + j;

                    sx = std::min(std::max(sx, 0), src.rows - 1);
                    sy = std::min(std::max(sy, 0), cols - 1);

                    sum += src_data[sx * cols + sy] * kernel[i + shift][j + shift];;
                }
            }
            // clamping
            sum = std::min(std::max(sum, 0.0), 255.0);
            dst_data[x * cols + y] = sum;
        }
    }
}
