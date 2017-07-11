//
//  SoundData.cpp
//  SpectralSynthesis
//
//  Created by Nathalie Blasberg on 10.07.17.
//
//

#include "SoundData.hpp"
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

SoundData::SoundData()
{
    
}

void SoundData::playSound(std::vector<std::vector<ImageData>> spVector)
{
    //        for(int i=0; i<spVector.size();i++)
    //        {
    //            for(int j=0; j<spVector[i].size(); j++)
    //            {
    //                cout << spVector[i][j].frequency << "\n";
    //            }
    //        }
    //    return;
    auto ctx = audio::master();
    auto addSound = ctx->makeNode(new audio::AddNode);
    for(int i=0; i<spVector.size(); i++)
    {
        double tmp = 0, tmpV = 0;
        auto addTable = ctx->makeNode(new audio::AddNode);
        for(int j=0; j<spVector[i].size(); j++)
        {
            if(spVector[i][j].amplitude!=0)
            {
                auto sine = ctx->makeNode(new audio::GenSineNode);
                sine->setFreq(spVector[i][j].frequency);
                auto gain = ctx->makeNode(new audio::GainNode);
                gain->setValue(spVector[i][j].amplitude/spVector[i].size());
                sine >> gain;
                sine->enable();
                gain >> addTable;
                tmp += spVector[i][j].frequency;
                tmpV += gain->getValue();
            }
        }
        
        if(addTable->getNumConnectedInputs() !=0)
        {
            auto lowPass = ctx->makeNode(new audio::FilterLowPassNode);
            lowPass->setCutoffFreq(20000);
            lowPass >> addTable;
            auto highPass = ctx->makeNode(new audio::FilterHighPassNode);
            highPass->setCutoffFreq(20);
            highPass >> addTable;
            auto delay = ctx->makeNode(new audio::DelayNode);
            delay->setMaxDelaySeconds(0.1f);
            auto gain = ctx->makeNode(new audio::GainNode);
            gain->setValue(tmpV);
            gain >> addTable >> delay >> addSound;
            delay->getParamDelaySeconds()->setProcessor(addSound);
            //addTable >> addSound;
            cout << i << ": " << tmp << " Volume: " << tmpV << "\n";
        }
        
        
    }
    
    addSound >> ctx->getOutput();
    //cout << addSound->getValue();
    addSound->getParam()->applyRamp(54, 5.0f);
    ctx->enable();
}
