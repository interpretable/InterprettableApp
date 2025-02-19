#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofLogNotice("Setting up");

    bDebugMode      = false;
    bDebugWarpMode  = false;
    bIsInactive     = false;
    
    ofSetFrameRate(60);
    ofSetLogLevel(OF_LOG_NOTICE);
    ofSetEscapeQuitsApp(true);
    ofHideCursor();
    
    trackingManager.setup();

    
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
    cam.setDeviceID(0);
    cam.setup(configJson["webcam-width"],configJson["webcam-height"]);
    
#endif

    ofLogNotice("Setting tracking manager");

    
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

    currentTimeMillis       = ofGetElapsedTimeMillis();
    currentShutDownMillis   = ofGetElapsedTimeMillis();
    
    string loggerUrl = configJson.value("backoffice-log-post-url", "http://interpretable.erasme.org/api/public/api/machine") + "/" + ofToString(machineId);
    
    logger.setUrl(loggerUrl);
    logger.setup();
    logger.log("start");

    ofLogNotice("Setting up done");
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
#ifdef __linux__

    // linux hack to get full screen
    if(ofGetFrameNum() == 5 ) {
        ofSetFullscreen(true);
    }
    
#endif
    
    
    //======================== check for new config.json changes
    if(ofGetFrameNum() % 100 == 0) {
        
        // check for config changes
        int currentTimeStamp = std::filesystem::last_write_time(configJsonFile);
        if(currentTimeStamp != configJsonTimeStamp) {
            loadConfigJson();
        }
        
    }
    
    //======================== update scene
    
    sceneManager.update();

    //======================== update camera & tracking

    cam.update();
    if(cam.isFrameNew()) {
        
        ofPixels pixels = cam.getPixels();
        
        // we use the cropped camera image
        if(cropRectangle.width > 0 && cropRectangle.height > 0)
            pixels.crop(cropRectangle.x, cropRectangle.y, cropRectangle.width, cropRectangle.height);
        
        trackingManager.update(pixels);
        
    }
    
    
    //======================== check for inactivity, 10 mn
    
    int curTime = ofGetElapsedTimeMillis();
    int diff    = curTime - currentTimeMillis;
    
    //ofLogNotice("Time Elapsed") << diff;

    // if nothing is detected we lower down the delay, in case of glitch
    
    if( diff > welcomeIdleDelay) {
        
        
        int markerId = 0;
        
        if(sceneManager.currentScenarioID != 0) {
            
            inactiveLastRecordedId = sceneManager.currentScenarioID;
            logger.logScenario(markerId,dataManager.scenarios[0].themeName, dataManager.scenarios[0].cardName);
            
            currentShutDownMillis   = ofGetElapsedTimeMillis();
            sceneManager.setScenario(&dataManager.scenarios[0]);
            bIsInactive = true;
            currentTimeMillis       = ofGetElapsedTimeMillis();
            
        }
        
    }
    
    //======================== check for inactivity, shutdown
    
    curTime = ofGetElapsedTimeMillis();
    diff    = curTime - currentShutDownMillis;
    
    // if nothing is detected we lower down the delay, in case of glitch
    
    if( bIsInactive && diff > shutdownDelay) {
        ofSystem("sudo poweroff");
    }
    
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    //======================== set canvas
    ofBackground(0);
    ofFill();
    ofSetColor(255);
    ofEnableAlphaBlending();
    
    //======================== create frame buffer object of the drawn scene

    fbo.begin();
    ofEnableAlphaBlending();
    ofClear(0, 0, 0, 0);
    ofSetColor(255,255);
    sceneManager.draw();
    ofDisableAlphaBlending();
    fbo.end();
    
    //======================== use the matrix to transform our fbo, and warp
    ofSetColor(255,255);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    ofPushMatrix();
    ofMultMatrix(warper.getMatrix());
    fbo.draw(0, 0);
    ofPopMatrix();
    glDisable(GL_BLEND);
    
    //======================== draw warped configuration

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
    
    //======================== draw camera
    
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
    configJson          = ofLoadJson("config.json");
    cropRectangle.set( configJson["camera_crop"]["x"], configJson["camera_crop"]["y"], configJson["camera_crop"]["width"], configJson["camera_crop"]["height"]);
    trackingManager.setProps(configJson["features-tracking-ratio"], configJson["features-ntries"]);
    machineId           = configJson.value("machine-id", 0);
    welcomeIdleDelay    = configJson.value("welcome-delay", 600000);
    shutdownDelay       =  configJson.value("shutdown-delay", 3600000);
    
}


//--------------------------------------------------------------

void ofApp::onMarkerFoundHandler(int & markerId) {
    
    //ofLogNotice("Marker found.") << "bIsInactive is " << bIsInactive << " and inactiveLastRecordedId is " << inactiveLastRecordedId ;

    
    if( bIsInactive && markerId == inactiveLastRecordedId) {
        ofLogNotice("Marker found.") << "but we're already inactive with this same id..";
        return;
    }
    
    if( markerId == trackingManager.detector.getLowestScoreIndex()) {
        
        ofLogNotice("Marker found.") << "Current id is " << sceneManager.currentScenarioID << " and catched id is " << dataManager.scenarios[markerId].id;
        if(sceneManager.currentScenarioID != dataManager.scenarios[markerId].id) {
            
            
            //ofLogNotice("Log scenario") << dataManager.scenarios[markerId].themeName << " " << dataManager.scenarios[markerId].cardName;
            logger.logScenario(markerId,dataManager.scenarios[markerId].themeName, dataManager.scenarios[markerId].cardName);
            
            if(markerId !=0 ) {
                currentTimeMillis       = ofGetElapsedTimeMillis();
                //ofLogNotice("Reset Timer delay") << welcomeIdleDelay;

            }
            
            currentShutDownMillis   = ofGetElapsedTimeMillis();

        }
        
        sceneManager.setScenario(&dataManager.scenarios[markerId]);
        // restart time
        
        bIsInactive = false;
    }

}

//--------------------------------------------------------------

void ofApp::onMarkerLostHandler(int & markerId) {
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key == 'd') {
        
        bDebugMode = !bDebugMode;
    
        if(bDebugWarpMode) {
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
