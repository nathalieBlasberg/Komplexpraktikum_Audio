#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include "cinder/gl/Texture.h"
#include "cinder/Surface.h"
#include "cinder/ImageIo.h"
#include "cinder/Rand.h"
#include "cinder/Perlin.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Timeline.h"
#include "cinder/Log.h"
#include "cinder/TriMesh.h"
#include "cinder/gl/Texture.h"

#include "cinder/audio/Node.h"
#include "cinder/audio/Context.h"
#include "cinder/audio/GenNode.h"
#include "cinder/audio/MonitorNode.h"
#include "cinder/audio/NodeEffects.h"
#include "cinder/audio/Utilities.h"
#include "cinder/audio/Param.h"
#include "cinder/Camera.h"
#include "cinder/params/Params.h"
#include "../xcode/ImageData.hpp"
#include "../xcode/Spectrogramm.hpp"
#include "../xcode/SoundData.hpp"
#include "../xcode/AudioDrawUtils.h"

//#inlcude "../xcode/AudioDrawUtils.h"


using namespace ci;
using namespace ci::app;
using namespace std;

class SpectralSynthesisApp : public App {
public:
    void setup() override;
    void mouseDown( MouseEvent event ) override;
    void update() override;
    void draw() override;
    SpectralSynthesisApp();
    void updateNode();
    
    Spectrogramm spectrogramm;
    SoundData soundData;
    Surface bitmap;
    std::vector<std::vector<ImageData>> spVector;
    CameraPersp				mCam;
    params::InterfaceGlRef	mParams;
    audio::MonitorNodeRef mMonitor;
    std::vector<audio::MonitorNodeRef> monitorNodes;
    audio::AddNodeRef addNode;
    gl::Texture2dRef texture;
    
};

SpectralSynthesisApp::SpectralSynthesisApp()
{
    bitmap = loadImage(loadAsset("KPAudio_pixelbeat.bmp"));
    texture = gl::Texture2d::create(bitmap);
    //cout << "height: " << bitmap.getHeight();
    spectrogramm = Spectrogramm::Spectrogramm(bitmap);
    spVector.resize(bitmap.getWidth());
    spVector = spectrogramm.getVector();
    soundData = SoundData::SoundData();
    
    //soundData.playSound(spVector);
}

void SpectralSynthesisApp::setup()
{
    // Setup our default camera, looking down the z-axis.
    mCam.lookAt( vec3( -20, 0, 0 ), vec3( 0 ) );
    // Create the interface and give it a name.
    mParams = params::InterfaceGl::create( getWindow(), "Definiere Frequenz", toPixels( ivec2( 200, 340 ) ) );

    auto ctx = audio::master();
    addNode = ctx->makeNode(new audio::AddNode);
    mMonitor = ctx->makeNode(new audio::MonitorNode);
    //cout << monitorNodes.size()<<"\n";
    int j=1, k=0;
    for(int i=0; i<spVector[0].size(); i+=50)
    {
        string name = "Oszillator " + std::to_string(j);
        mParams->addParam(name, &spVector[0][i].frequency).updateFn(
                [this, &i, &ctx, &k]
                {
//                    auto sine = ctx->makeNode(new audio::GenSineNode());
//                    sine->setFreq(spVector[0][i].frequency);
//                    auto monitor = ctx->makeNode(new audio::MonitorNode);
//                    sine >> monitor;
//                    sine->enable();
//                    monitorNodes.at(k) = monitor;
                });
        
        auto sine = ctx->makeNode(new audio::GenSineNode);
        sine->setFreq(spVector[0][i].frequency);
        auto monitor = ctx->makeNode(new audio::MonitorNode);
        sine>>monitor;
        sine>>addNode;
        sine->enable();
        monitorNodes.push_back(monitor);
        j++;
        k++;
    }
    addNode>>mMonitor;
    ctx->enable();
}

void SpectralSynthesisApp::mouseDown( MouseEvent event )
{
}

void SpectralSynthesisApp::update()
{
}

void SpectralSynthesisApp::draw()
{
    // this pair of lines is the standard way to clear the screen in OpenGL
    gl::enableDepthRead();
    gl::enableDepthWrite();
    gl::clear( Color::gray( 0.1f ) );
    //gl::setMatrices( mCam );
    
    // Draw the interface
    mParams->draw();

    //draw surface
    Rectf rect(15, 360,216,460);
    gl::draw(texture, rect);
    //draw waves
    int y1 = 20, y2 = 40;
    //cout << monitorNodes.size() << "\n";
    for(int i=0; i<monitorNodes.size(); i++)
    {
        if(monitorNodes[i] && monitorNodes[i]->getNumConnectedInputs())
        {
            Rectf scopeRect(250, y1, 450, y2);
            drawAudioBuffer(monitorNodes[i]->getBuffer(), scopeRect, true);
            //monitorNodes[i]>>addNode;
        }
        y1 = y2;
        y2 += 20;
    }
    
    
//    auto ctx = audio::master();
//    auto add = ctx->makeNode(new audio::AddNode);
//    auto monitor = ctx->makeNode(new audio::MonitorNode);
//    for(int i=0; i<monitorNodes.size(); i++)
//    {
//        cout<< monitorNodes[i]<<"\n";
//        monitorNodes[i]>>addNode;
//    }
//    addNode>>mMonitor;
//    ctx->enable();
    
    if(mMonitor && mMonitor->getNumConnectedInputs())
    {
        Rectf scopeRect(250,500,450,510);
        drawAudioBuffer(mMonitor->getBuffer(), scopeRect, true);
    }
}

void SpectralSynthesisApp::updateNode()
{
//    monitorNodes.clear();
//    mMonitor.reset();
//    auto ctx = audio::master();
//    auto add = ctx->makeNode(new audio::AddNode);
//    for(int i=0; spVector[0].size(); i++)
//    {
//        auto sine = ctx->makeNode(new audio::GenSineNode);
//        sine->setFreq(spVector[0][i].frequency);
//        auto monitor = ctx->makeNode(new audio::MonitorNode);
//        sine>>add;
//        sine>>monitor;
//        monitorNodes.push_back(monitor);
//        sine->enable();
//    }
//    add>>mMonitor;
//    ctx->enable();
    
}

CINDER_APP( SpectralSynthesisApp, RendererGl )
