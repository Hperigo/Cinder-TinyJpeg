#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/Log.h"
#include "ImageTargetTinyJpeg.h"

using namespace ci;
using namespace ci::app;

// We'll create a new Cinder Application by deriving from the App class.
class BasicApp : public App {
  public:
	void mouseDrag( MouseEvent event ) override;

  void setup() override;
  void update() override;
	void keyDown( KeyEvent event ) override;

	void draw() override;

  private:

    ci::SurfaceRef mSurf;
    ci::SurfaceRef mOutSurf;

};


void prepareSettings( BasicApp::Settings* settings )
{
	settings->setMultiTouchEnabled( false );
}

void BasicApp::setup(){

  ImageTargetTinyJpeg::registerSelf();


  try{

    CI_LOG_V("loading file..");
    mSurf = ci::Surface::create( loadImage( loadAsset("image.png")  ) );


    CI_LOG_V("writting stream...");
    OStreamMemRef os = OStreamMem::create();
    DataTargetRef target = DataTargetStream::createRef( os );
    writeImage( target, *mSurf, ImageTarget::Options().quality(1.0f), "jpeg" );


    CI_LOG_V("creating new surface..");
    auto buff =  ci::Buffer::create( os->getBuffer(), os->tell() );
    mOutSurf = Surface::create(loadImage( DataSourceBuffer::create(  buff )), SurfaceConstraintsDefault(), false );

    ci::Channel channel = mOutSurf->getChannelRed();
    writeImage( getAssetPath("") / "output.jpeg", channel, ImageTarget::Options().quality(1.0f), "jpeg" );


  }catch(std::exception &e){

    CI_LOG_E(e.what());

  }




}

void BasicApp::mouseDrag( MouseEvent event )
{

}

void BasicApp::keyDown( KeyEvent event )
{

}

void BasicApp::update(){


}

void BasicApp::draw()
{


}

// This line tells Cinder to actually create and run the application.
CINDER_APP( BasicApp, RendererGl, prepareSettings )
