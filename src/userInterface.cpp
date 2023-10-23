#include "userInterface.h"

/*
   Class that contains user interaction functions.
   It contains the primary member funcion to execute the project used by the main function.
*/

void UserInterface::execute()
{
    do {
        askImage();
        askFilter();
        askSize();
        apply();
    } while (_decide == 'y');
}

void UserInterface::askImage()
{
// handle imagepath
    std::cout << "Enter the image filepath (default: \"../img/lena.jpg\")" << std::endl;
    getline(std::cin, _imagepath);

    _src.setPath(_imagepath);
}

void UserInterface::askFilter()
{
// handle filter
    std::cout << "'l' \t Low pass filter" << std::endl;
    std::cout << "'h' \t Hign pass filter" << std::endl;
    std::cout << "'b' \t Bandpass filter" << std::endl;
    std::cout << "'g' \t Gaussian filter" << std::endl;
    std::cout << "'p' \t Laplace filter" << std::endl;
    std::cout << "Choose an image filter: " << std::endl;

    bool filter = false;
    while (!filter) {
        std::cin >> _fil;
        switch (_fil) {
            case 'l': filter = true; break;
            case 'h': filter = true; break;
            case 'b': filter = true; break;
            case 'g': filter = true; break;
            case 'p': filter = true; break;
            default: std::cout << "Not a filter corresponding to the letter. Please try again:" << std::endl;
        }
    }
}

void UserInterface::askSize()
{
// handle kernel size
    std::cout << "Choose the kernel size: " << std::endl;
    std::cin >> _ksize;
    while (_ksize % 2 == 0) {
        std::cout << "The value entered is not an ODD number! Please try again: " << std::endl;
        std::cin >> _ksize;
    }
}

void UserInterface::apply()
{
// apply the right filter
    switch (_fil) {
        case 'l': _chosenFilter = std::make_unique<LowPass>(_ksize); break;
        case 'h': _chosenFilter = std::make_unique<HighPass>(_ksize); break;
        case 'b': _chosenFilter = std::make_unique<BandPass>(_ksize); break;
        case 'g': _chosenFilter = std::make_unique<Gauss>(_ksize); break;
        case 'p': _chosenFilter = std::make_unique<Laplace>(_ksize); break;
        default: std::cout << "Some error occurred. Please try again:" << std::endl;
    }

    _master.applyFilter(_src, *_chosenFilter);

    std::cout << "Press y to continue with processing, press q to quit." << std::endl;
    std::cin >> _decide;
    // Ignore to the end of line
    std::cin.ignore(256, '\n');
}
