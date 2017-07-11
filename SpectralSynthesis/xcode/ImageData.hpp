//
//  ImageData.hpp
//  SpectralSynthesis
//
//  Created by Nathalie Blasberg on 10.07.17.
//
//

#ifndef ImageData_hpp
#define ImageData_hpp

#include <stdio.h>

using namespace std;

class ImageData
{
public:
    //variablen
    double amplitude;
    double frequency;
    //konstruktoren
    ImageData();
    ImageData(double amplitude, double frequency);
};

#endif /* ImageData_hpp */
