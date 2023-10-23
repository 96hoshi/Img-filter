#include <chrono>
#include <iomanip>
#include <iostream>
#include <functional>
#include <cstring>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

#include "identity.h"
#include "bandpass.h"
#include "highpass.h"
#include "lowpass.h"
#include "laplace.h"
#include "gauss.h"
#include "test.h"

#define N 8

#define COUT_STRING_H           12
#define COUT_STRING_W           16
#define COUT_DOUBLE_W           10
#define COUT_INTEGER_W          8
#define COUT_PRECISION          0

#define COUT_HEADER             std::setw(COUT_STRING_H) << std::right
#define COUT_STRING             std::setw(COUT_STRING_W) << std::right
#define COUT_FLOAT              std::setw(COUT_DOUBLE_W) << std::right << std::fixed << std::setprecision(COUT_PRECISION)
#define COUT_INTEGER            std::setw(COUT_INTEGER_W) << std::right << std::fixed
#define COUT_BOOLEAN            std::boolalpha


void print_result(
    std::string filename,
    std::string filter_name,
    int kernel_size,
    int width,
    int height,
    double time
)
{
    std::cout << COUT_STRING << filename << ";"
            << COUT_STRING << filter_name << ";"
            << COUT_INTEGER << kernel_size << ";"
            << COUT_INTEGER << width << ";"
            << COUT_INTEGER << height << ";"
            << COUT_FLOAT << time << ";"
            << "\n";
}

void print_top()
{
    std::cout << COUT_HEADER << "name"
            << COUT_HEADER << "filter"
            << COUT_HEADER << "k_size"
            << COUT_HEADER << "width"
            << COUT_HEADER << "height"
            << COUT_HEADER << "useconds"
            << "\n";
}


void Test::testConvolution() {

    const int N_FILTERS = 5;
    const int N_IMGS = 3;

    std::string path[N_IMGS];
    path[0] = "../img/lena.jpg";
    path[1] = "../img/test.jpg";
    path[2] = "../img/2k.png";
    Filter* fs[N_FILTERS];
    cv::Mat src;
    cv::Mat dst;
    std::chrono::time_point<std::chrono::system_clock> start;
    std::chrono::time_point<std::chrono::system_clock> end;

    std::cout << " ========================== NEW TEST BASE ==========================" << std::endl;
    print_top();

    for (int k = 3; k < N; k = k + 2){
        fs[0] = new LowPass(k);
        fs[1] = new HighPass(k);
        fs[2] = new BandPass(k);
        fs[3] = new Gauss(k);
        fs[4] = new Laplace(k);

        for (int img = 0; img < N_IMGS; img++){
            std::cout << "..................................................................." << std::endl;
            for (int i = 0; i < N_FILTERS - 1; i++ ){
                src = cv::imread(path[img]);
                dst = cv::Mat(src.rows, src.cols, src.type());;
                std::vector<std::vector<float>> kernel = fs[i]->getKernel();

                start = std::chrono::system_clock::now();

                applyConvolution(src, dst, kernel);

                end = std::chrono::system_clock::now();
                auto elapsed =
                    std::chrono::duration_cast<std::chrono::microseconds>(end - start);
                print_result(path[img], fs[i]->getDescription(), k, src.cols, src.rows, elapsed.count());

                src.release();
                dst.release();
            }
            // laplace filtering
            src = cv::imread(path[img]);
            dst = cv::Mat(src.rows, src.cols, src.type());;
            std::vector<std::vector<float>> kernel = fs[N_FILTERS-1]->getKernel();

            start = std::chrono::system_clock::now();

            applyConvolutionGray(src, dst, kernel);

            end = std::chrono::system_clock::now();
            auto elapsed =
                std::chrono::duration_cast<std::chrono::microseconds>(end - start);

            print_result(path[img], fs[N_FILTERS-1]->getDescription(), k, src.cols, src.rows, elapsed.count());

            src.release();
            dst.release();
        }
    }
}


void Test::testConvolutionOpt() {

    const int N_FILTERS = 5;
    const int N_IMGS = 3;

    std::string path[N_IMGS];
    path[0] = "../img/lena.jpg";
    path[1] = "../img/test.jpg";
    path[2] = "../img/2k.png";
    Filter* fs[N_FILTERS];
    cv::Mat src;
    cv::Mat dst;
    std::chrono::time_point<std::chrono::system_clock> start;
    std::chrono::time_point<std::chrono::system_clock> end;

    std::cout << " ========================== NEW TEST OPTIMIZED ==========================" << std::endl;
    print_top();

    for (int k = 3; k < N; k = k + 2){
        fs[0] = new LowPass(k);
        fs[1] = new HighPass(k);
        fs[2] = new BandPass(k);
        fs[3] = new Gauss(k);
        fs[4] = new Laplace(k);

        for (int img = 0; img < N_IMGS; img++){
            std::cout << "..................................................................." << std::endl;
            for (int i = 0; i < N_FILTERS - 1; i++ ){
                src = cv::imread(path[img]);
                dst = cv::Mat(src.rows, src.cols, src.type());;
                std::vector<std::vector<float>> kernel = fs[i]->getKernel();

                start = std::chrono::system_clock::now();

                applyConvolutionOptimized(src, dst, kernel);

                end = std::chrono::system_clock::now();
                auto elapsed =
                    std::chrono::duration_cast<std::chrono::microseconds>(end - start);

                print_result(path[img], fs[i]->getDescription(), k, src.cols, src.rows, elapsed.count());

                src.release();
                dst.release();
            }
            // laplace filtering
            src = cv::imread(path[img]);
            dst = cv::Mat(src.rows, src.cols, src.type());;
            std::vector<std::vector<float>> kernel = fs[N_FILTERS-1]->getKernel();

            start = std::chrono::system_clock::now();

            applyConvolutionGrayOptimized(src, dst, kernel);

            end = std::chrono::system_clock::now();
            auto elapsed =
                std::chrono::duration_cast<std::chrono::microseconds>(end - start);

            print_result(path[img], fs[N_FILTERS-1]->getDescription(), k, src.cols, src.rows, elapsed.count());

            src.release();
            dst.release();
        }
    }
}
