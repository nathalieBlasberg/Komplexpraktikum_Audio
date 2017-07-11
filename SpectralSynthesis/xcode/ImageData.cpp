//
//  ImageData.cpp
//  SpectralSynthesis
//
//  Created by Nathalie Blasberg on 10.07.17.
//
//

#include "ImageData.hpp"
#include <stdio.h>

using namespace std;

ImageData::ImageData()
{
    
}

ImageData::ImageData(double amplitude, double frequency)
{
    this->amplitude = amplitude;
    this->frequency = frequency;
}
