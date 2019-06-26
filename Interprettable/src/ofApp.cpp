#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofLogNotice("Setting up");

    bDebugMode      = false;
    bDebugWarpMode  = false;
    
    ofSetFrameRate(60);
    ofSetLogLevel(OF_LOG_NOTICE);
    ofSetEscapeQuitsApp(true);
    ofHideCursor();
    
    configJsonFile.open("config.json");
    loadConfigJson();
    
    dataManager.getItems(configJson["backoffice-getall-url"]);
    sceneManager.setup();
    
    ofLogNotice("Setting up camera");
  
#ifdef __linux__
    
    cam.allocate(1280,720, OF_PIXELS_RGB);
    cam.setPipeline("v4l2src device=/dev/video0 ! queue ! video/x-h264,width=1280,height=720,framerate=30/1 ! h264parse ! avdec_h264 ! videoconvert", OF_PIXELS_RGB, false, 1280, 720);
    cam.startPipeline();
    cam.play();

#else

    cam.listDevices();
    cam.setDeviceID(2);
    cam.setup(configJson["webcam-width"],configJson["webcam-height"]);
    
    
#endif

    ofLogNotice("Setting tracking manager");

    trackingManager.setup();
    
    // add pictures
    for(int i=0; i<dataManager.mainJson.size(); i++) {
        string url = dataManager.mainJson[i]["card_picture"];
        trackingManager.addImage(url);
    }
    
    ofAddListener(trackingManager.onMarkerFound, this, &ofApp::onMarkerFoundHandler);
    ofAddListener(trackingManager.onMarkerLost, this, &ofApp::onMarkerLostHandler);
    trackingManager.start();
    
    ofLogNotice("Setting scene manager");

    sceneManager.setScenario(&dataManager.scenarios[0]);
    
    fbo.allocate(800, 800);

    int x = (ofGetWidth() - fbo.getWidth()) * 0.5;       // center on screen.
    int y = (ofGetHeight() - fbo.getHeight()) * 0.5;     // center on screen.
    int w = fbo.getWidth();
    int h = fbo.getHeight();
    
    
    ofLogNotice("Setting up warp");

    // quad warping 
    warper.setSourceRect(ofRectangle(0, 0, w, h));
    warper.setTopLeftCornerPosition(ofPoint(x, y));
    warper.setTopRightCornerPosition(ofPoint(x + w, y));
    warper.setBottomLeftCornerPosition(ofPoint(x, y + h));
    warper.setBottomRightCornerPosition(ofPoint(x + w, y + h));
    warper.setup();
    warper.load();
    warper.disableMouseControls();
    warper.disableKeyboardShortcuts();
    
    
    ofLogNotice("Setting up logger");

    currentTimeMillis  = ofGetElapsedTimeMillis();
    
    string loggerUrl = configJson.value("backoffice-log-post-url", "http://interpretable.erasme.org/api/public/api/machine") + "/" + ofToString(machineId);
    
    logger.setUrl(loggerUrl);
    logger.setup();
    logger.log("start");

    ofLogNotice("Setting up done");


    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if(ofGetFrameNum() == 5 ) {
        ofSetFullscreen(true);
    }
    
    sceneManager.update();
    
    
    if(ofGetFrameNum() % 100 == 0) {
        
        // check for config changes
        int currentTimeStamp = std::filesystem::last_write_time(configJsonFile);
        
        if(currentTimeStamp != configJsonTimeStamp) {
            loadConfigJson();
        }
        
    }
    
    cam.update();
    if(cam.isFrameNew()) {
        
        if(cropRectangle.width > 0 && cropRectangle.height > 0)
            cam.getPixels().crop(cropRectangle.x, cropRectangle.y, cropRectangle.width, cropRectangle.height);
        
        trackingManager.update(cam.getPixels());
        
    }
    
    // check for inactivity
    int curTime = ofGetElapsedTimeMillis();
    int diff    = curTime - currentTimeMillis;
    
    // if nothings happends for 10mn, restart.
    if( diff > 600000) {
        int start = 0;
        onMarkerFoundHandler(start);
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){


    
    ofBackground(0);
    ofFill();
    ofSetColor(255);
    ofEnableAlphaBlending();
    
    fbo.begin();
    ofEnableAlphaBlending();
    ofClear(0, 0, 0, 0);
    ofSetColor(255,255);
    sceneManager.draw();
    ofDisableAlphaBlending();
    fbo.end();
    
    
    //======================== use the matrix to transform our fbo.
    ofSetColor(255,255);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    ofPushMatrix();
    ofMultMatrix(warper.getMatrix());
    fbo.draw(0, 0);
    ofPopMatrix();
    glDisable(GL_BLEND);
    
    
    if(bDebugWarpMode) {
        
        ofSetColor(ofColor::magenta);
        warper.drawQuadOutline();
        
        ofSetColor(ofColor::yellow);
        warper.drawCorners();
        
        ofSetColor(ofColor::magenta);
        warper.drawHighlightedCorner();
        
        ofSetColor(ofColor::red);
        warper.drawSelectedCorner();
        ofSetColor(255,255);
        
    }
    
    if(bDebugMode) {

        ofImage image;
        image.setFromPixels(cam.getPixels());
        image.draw(0.0,0.0);
        ofNoFill();
        ofSetColor(255,0,0);
        ofDrawRectangle(cropRectangle);
        trackingManager.debugDraw(10, 20);

    }

}

void ofApp::exit() {
    
    logger.log("end");
    warper.save();
    std::exit(0);
    trackingManager.detector.stop();
   // logger.uploadLog();
    
    
#ifdef __linux__
    
   
    cam.stop();
    cam.close();
    
#endif

}

//--------------------------------------------------------------


void ofApp::loadConfigJson() {
    
    
    configJsonTimeStamp = std::filesystem::last_write_time(configJsonFile);
    configJson = ofLoadJson("config.json");
    cropRectangle.set( configJson["camera_crop"]["x"], configJson["camera_crop"]["y"], configJson["camera_crop"]["width"], configJson["camera_crop"]["height"]);
    trackingManager.setProps(configJson["features-tracking-ratio"], configJson["features-ntries"]);
    
    machineId = configJson.value("machine-id", 0);
    
   // trackingManager.detector.setExtractorSettings(60, 4);
    
}


//--------------------------------------------------------------

void ofApp::onMarkerFoundHandler(int & markerId) {
    
    
    if( markerId == trackingManager.detector.getLowestScoreIndex()) {
        
       // ofLogNotice("Marker found !" ) << trackingManager.getLabel(markerId);//
        
        sceneManager.setScenario(&dataManager.scenarios[markerId]);
        logger.logScenario(markerId,dataManager.scenarios[markerId].theme);
    
        // restart time
        currentTimeMillis  = ofGetElapsedTimeMillis();
        
    }

}

//--------------------------------------------------------------

void ofApp::onMarkerLostHandler(int & markerId) {
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key == 'd') {
        
        bDebugMode = !bDebugMode;
        
        if(bDebugMode) {
            ofShowCursor();
            
        } else {
            ofHideCursor();
        }

    }
    
    if(key == 'c')
        ofShowCursor();
    
    if(key == 'w') {
        
        bDebugWarpMode =!bDebugWarpMode;
        
        if(bDebugWarpMode) {
            ofShowCursor();
            warper.enableMouseControls();
            warper.enableKeyboardShortcuts();
        } else {
            ofHideCursor();
            warper.disableMouseControls();
            warper.disableKeyboardShortcuts();
            warper.save();
        }
        
    }
    
    
    if (key == 's') {
        int rdm = floor(ofRandom(dataManager.scenarios.size()));
        sceneManager.setScenario(&dataManager.scenarios[rdm]);
        
    }
    
    if (key == 'i') {
        sceneManager.setScenario(&dataManager.scenarios[0]);
        
    }
    
    if (key == 'f')
        ofToggleFullscreen();
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
    if(bDebugMode) {

        cropRectangle.x = storedMousePosition.x;
        cropRectangle.y = storedMousePosition.y;

        cropRectangle.width = ofClamp(x - cropRectangle.x, 240, 1200);
        cropRectangle.height = ofClamp(y - cropRectangle.y, 200, 1200);
        
        configJson["camera_crop"]["x"] = cropRectangle.x;
        configJson["camera_crop"]["y"] = cropRectangle.y;
        configJson["camera_crop"]["width"] = cropRectangle.width;
        configJson["camera_crop"]["height"] = cropRectangle.height;
        ofSaveJson("config.json", configJson);
    }
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
    storedMousePosition.set(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
