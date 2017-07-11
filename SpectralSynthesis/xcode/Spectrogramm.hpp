//
//  Spectrogramm.hpp
//  SpectralSynthesis
//
//  Created by Nathalie Blasberg on 10.07.17.
//
//

#ifndef Spectrogramm_hpp
#define Spectrogramm_hpp

#include <stdio.h>
#include "cinder/gl/Texture.h"
#include "cinder/Surface.h"
#include "cinder/ImageIo.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/Rand.h"
#include "cinder/Perlin.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Timeline.h"
#include "cinder/Log.h"
#include "cinder/TriMesh.h"
#include "cinder/gl/gl.h"
#include "../xcode/ImageData.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;

class Spectrogramm
{
public:
    //variablen
    cinder::Surface bitmap;
    cinder::Area area;
    //konstruktoren
    Spectrogramm();
    Spectrogramm(Surface bitmap);
    //funktionen
    std::vector<std::vector<ImageData>> getVector();
    double getLuminance(int r, int g, int b);
    double getFrequency(double minFreq, double maxFreq, int currentOsc, int numOsc);
    
};
#endif /* Spectrogramm_hpp */
