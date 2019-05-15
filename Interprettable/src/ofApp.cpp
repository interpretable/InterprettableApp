#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFullscreen(true);
    
    ofSetFrameRate(60);
    ofSetLogLevel(OF_LOG_NOTICE);
    dataManager.getItems();
    sceneManager.setup();
    
    cam.setDeviceID(0);
    cam.setup(640,480);
    trackingManager.setup();
    
    cropRectangle.set(0,0, 640, 480);
    
    // add pictures
    for(int i=0; i<dataManager.mainJson.size(); i++) {
        string url = dataManager.mainJson[i]["card_picture"];
        trackingManager.addImage(url);
    }
    
    ofAddListener(trackingManager.onMarkerFound, this, &ofApp::onMarkerFoundHandler);
    ofAddListener(trackingManager.onMarkerLost, this, &ofApp::onMarkerLostHandler);
    trackingManager.start();
    
    sceneManager.setScenario(&dataManager.scenarios[0]);
    
    fbo.allocate(ofGetWidth(), ofGetHeight());

    int x = (ofGetWidth() - fbo.getWidth()) * 0.5;       // center on screen.
    int y = (ofGetHeight() - fbo.getHeight()) * 0.5;     // center on screen.
    int w = fbo.getWidth();
    int h = fbo.getHeight();
    
    // quad warping 
    warper.setSourceRect(ofRectangle(0, 0, w, h));
    warper.setTopLeftCornerPosition(ofPoint(x, y));
    warper.setTopRightCornerPosition(ofPoint(x + w, y));
    warper.setBottomLeftCornerPosition(ofPoint(x, y + h));
    warper.setBottomRightCornerPosition(ofPoint(x + w, y + h));
    warper.setup();
    warper.load();

}

//--------------------------------------------------------------
void ofApp::update(){
    
    cam.update();
    if(cam.isFrameNew()) {
        ofPixels pixels = cam.getPixels();
        
        if(cropRectangle.width > 0 && cropRectangle.height > 0)
            pixels.crop(cropRectangle.x, cropRectangle.y, cropRectangle.width, cropRectangle.height);
        
        trackingManager.update(pixels);
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    ofFill();
    ofSetColor(255);
    ofEnableAlphaBlending();
    
  //  cam.draw(0.0,0.0);
   /// trackingManager.debugDraw(20, 0);

    fbo.begin();
    ofClear(0,0);
    ofEnableAlphaBlending();
    ofSetColor(255,255);
    sceneManager.draw();
    fbo.end();
    
    ofMatrix4x4 mat = warper.getMatrix();
    
    //======================== use the matrix to transform our fbo.
    ofSetColor(255,255);
    ofPushMatrix();
    ofMultMatrix(mat);
    fbo.draw(0, 0);
    ofPopMatrix();
    
    ofSetColor(ofColor::magenta);
    warper.drawQuadOutline();
    
    ofSetColor(ofColor::yellow);
    warper.drawCorners();
    
    ofSetColor(ofColor::magenta);
    warper.drawHighlightedCorner();
    
    ofSetColor(ofColor::red);
    warper.drawSelectedCorner();
    ofSetColor(255,255);

    cam.draw(0.0,0.0);
    
    ofNoFill();
    ofSetColor(255,0,0);
    ofDrawRectangle(cropRectangle);
    
    trackingManager.debugDraw(10, 20);

}

void ofApp::exit() {
    
    warper.save();
    
}


//--------------------------------------------------------------

void ofApp::onMarkerFoundHandler(int & markerId) {
    
    ofLogNotice("Marker found !" ) << markerId;
    sceneManager.setScenario(&dataManager.scenarios[markerId]);

}

//--------------------------------------------------------------

void ofApp::onMarkerLostHandler(int & markerId) {
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
    
    cropRectangle.x = storedMousePosition.x;
    cropRectangle.y = storedMousePosition.y;

    cropRectangle.width = x - cropRectangle.x;
    cropRectangle.height = y - cropRectangle.y;


    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
    storedMousePosition.set(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

   // cropRectangle.set(0,0,0,0);
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
