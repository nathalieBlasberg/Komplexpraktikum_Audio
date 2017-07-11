//
//  Spectrogramm.cpp
//  SpectralSynthesis
//
//  Created by Nathalie Blasberg on 10.07.17.
//
//

#include "Spectrogramm.hpp"
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
#include <math.h>
#include "../xcode/ImageData.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;

Spectrogramm::Spectrogramm()
{
    
}

Spectrogramm::Spectrogramm(Surface bitmap)
{
    this->bitmap = bitmap;
}

std::vector<std::vector<ImageData>> Spectrogramm::getVector()
{
    area.set(0, 0, bitmap.getWidth(), bitmap.getHeight());
    std::vector<std::vector<ImageData>> spVector;
    
    //für die Erstellung der Frequenzen
    int numOsc = 20;
    int maxFreq = 54;
    int minFreq = 1;
    int i = numOsc;
    int width = 0, tmpHeight = 0;
    //Bitmap abtasten
    cinder::Surface::Iter iter = bitmap.getIter(area);
    spVector.resize(bitmap.getWidth());
    
    while(iter.line())
    {
        while(iter.pixel())
        {
            //amplitude
            double luminance;
            luminance = getLuminance(iter.r(), iter.g(), iter.b());
            //frequenz
            double frequency = getFrequency(minFreq, maxFreq, i, numOsc);
            //ins vector einfügen
            ImageData tmp(luminance, frequency);
            spVector[width].push_back(tmp);
            width++;
        }
        width = 0;
        tmpHeight++;
        if((tmpHeight!=0)&&(tmpHeight%50==0))
            i--;
    }
    
    return spVector;
}

double Spectrogramm::getLuminance(int r, int g, int b)
{
    double luminance;
    if((r==0)&&(g==0)&&(b==0))
        return luminance = 0;
    else
        return luminance = round((r/255)*0.299 + (g/255)*0.587 + (b/255)*0.114);
    
}

double Spectrogramm::getFrequency(double minFreq, double maxFreq, int currentOsc, int numOsc)
{
    //Frequenzbereiche mit log bestimmen
    double minFreqRange = std::log(minFreq);
    double maxFreqRange = std::log(maxFreq);
    //intervall der Frequenzbereiche bestimmen
    double interval = maxFreqRange - minFreqRange;
    //intervallbereich in äquidisante Teile bestimmen
    double linDist = (currentOsc * interval)/(numOsc-1) + std::log(minFreq);
    //Stützstellen logarithmisch bestimmen
    double logDist = pow(exp(1), linDist);
    return logDist;
}
