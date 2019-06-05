#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
        
    ofGLFWWindowSettings settings;
    settings.setGLVersion(3, 2);
    settings.setSize(1200, 800);
    settings.windowMode = OF_FULLSCREEN;
    ofCreateWindow(settings);
    ofRunApp(new ofApp());
    

}
