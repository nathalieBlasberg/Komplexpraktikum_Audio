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
#include "../xcode/Spektrogramm.cpp"

using namespace ci;
using namespace ci::app;
using namespace std;

class SineSynthesisApp : public App {
  public:
    
    void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
//    double createSineWave();
//    double changeFrequency(double frequency);
    
    Surface bitmap;
    double actualFrequency=0;                 //frequenz, das jedem pixel zugeteilt wird
    double heightFraction;
    std::vector<std::vector<Spektrogramm>> spVector; //Array, der seine Größe beliebig ändern kann
    Area area;
    //gl::Texture texture;
    
    //für update-Funktion
//    int i = 0;
    audio::AddNodeRef wholeAdd;
};

void SineSynthesisApp::setup()
{
    bitmap = loadImage(loadAsset("KPAudio_pixelbeat.bmp"));
    heightFraction = (double) round(bitmap.getHeight()/5); //ingesammt 5 Frequenzen für test
    actualFrequency = 420;
    
    area.set(0, 0, bitmap.getWidth(), bitmap.getHeight());
    
    //Bitmap abtasten
    Surface::Iter iter = bitmap.getIter(area);
    int w=0;  //counter für breite und höhe
    
    spVector.resize(bitmap.getWidth());
    while(iter.line()) //iteriert jede zeile
    {
//        if(h!= 0 && (h % (int)heightFraction == 0)) //damit die frequenz nicht sofort vermindert wird
//        {
//            switch((int)actualFrequency) //Frequenz ändern, Zahlen naiv ausgedacht
//            {
//                case 420: actualFrequency = 320; break;
//                case 320: actualFrequency = 220; break;
//                case 220: actualFrequency = 120; break;
//                case 120: actualFrequency = 20; break;
//                case 20: actualFrequency = 420; break;
//            }
//        }
        
//        spVector[w].resize(bitmap.getHeight());
        while(iter.pixel()) //jeden pixel der zeile iterieren
        {
            //rgb in y umwandeln für Helligkeit
            double luminance = round((iter.r()/255)*0.299 + (iter.g()/255)*0.587 + (iter.b()/255)*0.114);
            Spektrogramm temp(luminance,actualFrequency);
            spVector[w].push_back(temp);
            w++;
        }
        w=0; //Zeile zu Ende, width für vektor zurücksetzen
//        h++;
        actualFrequency -= 100;
        if(actualFrequency< 20)
            actualFrequency = 420;
    }
    
    auto ctx = audio::master();
    //spielt die Töne der ganzen bmp ab
    wholeAdd = ctx->makeNode(new audio::AddNode);
    
    int l=1;
    //Abspielen
    for(int i=0; i<spVector.size(); i++)
    {
        //spielt die Töne einer Spalte von der bmp ab
        auto tableAdd = ctx->makeNode(new audio::AddNode);
//        double sineMedian = 0;
//        double frequencyMedian = 0;
//        
//        for(int j=0; j<spVector[i].size(); j++)
//        {
//            double tmp = spVector[i][j].alphaValue;
//            sineMedian += pow(tmp,128);
//            tmp = spVector[i][j].frequency;
//            frequencyMedian += pow(tmp,2);
//        }
//        
//        sineMedian = 1/sqrt(sineMedian);
//        frequencyMedian = 1/sqrt(5000);
        
        for(int j=0; j<spVector[i].size(); j++)
        {
            if(spVector[i][j].alphaValue > 0)
            {
                //einzelne Pixel nach Frequenz und Alphawert abfragen und zu einem Node generieren
                auto sine = ctx->makeNode(new audio::GenSineNode);
                sine->setFreq(spVector[i][j].frequency/sqrt(sqrt(spVector[i].size())));
                auto gain = ctx->makeNode(new audio::GainNode);
                gain->setValue(spVector[i][j].alphaValue/sqrt(spVector[i].size()));
//                auto filterHigh = ctx->makeNode(new audio::FilterHighPassNode);
//                filterHigh->setCutoffFreq(20);
//                auto filterLow = ctx->makeNode(new audio::FilterLowPassNode);
//                filterLow->setCutoffFreq(5000);
//                sine >> filterHigh >> filterLow >> gain;
                sine >> gain;
                cout << "\nAmplitude: " << gain->getValue() << "\n";
                cout << "Frequenz: " << sine->getFreq() << "\n";
                gain >> tableAdd;
                sine->enable();
            }

        }
        
        if(tableAdd->getNumConnectedInputs() != 0)
        {
            //tableAdd >> ctx->getOutput(); //hier sollte der ton rauskommen...
            //ctx->enable();

            cout << "\nBLUB " << l << " number inputs: " << tableAdd->getNumConnectedInputs() << "\n\n";
        
            auto delay = ctx->makeNode(new audio::DelayNode);
            delay->setMaxDelaySeconds(10);
            delay->getParamDelaySeconds()->setProcessor(tableAdd);
        
            delay >> tableAdd >> wholeAdd;
//            ctx->enable();
//            ctx->disable();
            l++;
        }
    }
    wholeAdd >> ctx->getOutput();
    ctx->enable();
    
}

void SineSynthesisApp::mouseDown( MouseEvent event )
{
    
}

void SineSynthesisApp::update()
{

    
//    if(i >= spVector.size())
//        i = 0;
    
//    auto ctx = audio::master();
//    auto sine = ctx->makeNode(new audio::GenSineNode);
//    auto gain = ctx->makeNode(new audio::GainNode);
//    sine->setFreq(spVector[100][100].frequency);
//    gain->setValue(spVector[100][100].alphaValue);
//    sine >> gain >> ctx->getOutput();
//    sine->enable();
//    ctx->enable();
//
////    auto mainSine = ctx->makeNode(new audio::GenSineNode);
//    auto mainGain = ctx->makeNode(new audio::GainNode);
//    mainGain->setValue(1.0f);
//    auto delay = ctx->makeNode(new audio::DelayNode);
//    delay->setMaxDelaySeconds(5000);
//    int k = 0;
//    
//    for(int j=0; j<spVector[i].size(); j++)
//    {
//        if(spVector[i][j].alphaValue != 0)
//        {
//            auto sine = ctx->makeNode(new audio::GenSineNode());
//            sine->setFreq(spVector[i][j].frequency);
//            auto gain = ctx->makeNode(new audio::GainNode());
//            gain->setValue(spVector[i][j].alphaValue);
//            auto lowpass = ctx->makeNode(new audio::FilterLowPassNode());
//            lowpass->setFreq(400);
//            sine >> lowpass >> gain >> mainGain;
////            sine->enable();
////            lowpass->enable();
////            gain->enable();
//        }
////        delay->setMaxDelaySeconds(100);
////        auto add = ctx->makeNode(new audio::AddNode(220));
////        sine >> gain;
////        delay->getParamDelaySeconds()->setProcessor(gain);
////        sine >> gain >> ctx->getOutput();
////        sine >> gain >> delay >> ctx->getOutput();
//        cout << "blub" << k;
//        k++;
//    }
////    mainSine >> mainGain;
//    mainGain >> ctx->getOutput();
//    mainGain->enable();
//    cout << "\nOutputs: "<< mainGain->getNumConnectedInputs() << "\n";
//    delay->getParamDelaySeconds()->setProcessor(mainGain);
//    mainGain >> delay >> ctx->getOutput();
//    ctx->enable();
////    mainSine->enable();
////    delay->enable();
//    
////    usleep(10000);
//    i++; //Pro 60 sekunden sollen nur eine Spalte des Array abgespielt werden
//    
}

void SineSynthesisApp::draw()
{
    gl::clear(Color(0,0,0));
    //gl::draw(texture);
}

//double createSineWave()
//{
//    double rphase = (double) rand() * M_PI;
//    double sine = cos(0*2.0*M_PI*0.25+rphase);
//    return sine;
//}
//
//double changeFrequency(double frequency) //banal ausgedacht
//{
//    
//    switch((int)frequency)
//    {
//        case 20000: return 15200;
//        case 15200: return 10200;
//        case 10200: return 5200;
//        case 5200: return 200;
//        case 200: return 200;
//        default: return 200;
//    }
//}


CINDER_APP( SineSynthesisApp, RendererGl )
