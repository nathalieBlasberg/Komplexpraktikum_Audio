//
//  SoundData.hpp
//  SpectralSynthesis
//
//  Created by Nathalie Blasberg on 10.07.17.
//
//

#ifndef SoundData_hpp
#define SoundData_hpp

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

#include "cinder/audio/Node.h"
#include "cinder/audio/Context.h"
#include "cinder/audio/GenNode.h"
#include "cinder/audio/NodeEffects.h"
#include "cinder/audio/Utilities.h"
#include "cinder/audio/Param.h"
#include "../xcode/ImageData.hpp"
#include "../xcode/Spectrogramm.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;


class SoundData
{
public:
    //variablen
    ci::audio::AddNodeRef addSound;
    ci::audio::ContextRef ctx;
    //konstruktoren
    SoundData();
    void playSound(std::vector<std::vector<ImageData>> spVector);
    //funktionen
};

#endif /* SoundData_hpp */
