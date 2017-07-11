//
//  Spektrogramm.cpp
//  SineSynthesis
//
//  Created by Nathalie Blasberg on 01.05.17.
//
//
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class Spektrogramm
{
    public:
    double alphaValue;
    double frequency;
    Spektrogramm(double alphaValue,double frequency)
    {
        this->alphaValue = alphaValue;
        this->frequency = frequency;
    }
};


