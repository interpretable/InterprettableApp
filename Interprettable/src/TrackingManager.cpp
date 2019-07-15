//
//  TrackingManager.cpp
//  Interprettable
//
//  Created by Martial Geoffre-Rouland on 02/05/2019.
//

#include "TrackingManager.hpp"

void TrackingManager::setup() {
    
    detector.setup();

}

void TrackingManager::addImage(string url) {
    
    ofImage img;
    img.load(url);
    detector.addImageToTrack(img, url);
    images.push_back(img);
    detecteds.push_back(false);
    
}

void TrackingManager::start() {
    detector.start();

    
}

void TrackingManager::update(ofPixels & input) {
    
    detector.update(input);
    
    for(int i=0; i<images.size(); i++) {
        
        bool bIsDetected = detector.getDetected(i);
       
        // check status
        if(detecteds[i] != bIsDetected) {
            
            if(bIsDetected)
                ofNotifyEvent(onMarkerFound, i, this);
            else
                ofNotifyEvent(onMarkerLost, i, this);

        }
        
        detecteds[i] = bIsDetected;
    }
    
}

string TrackingManager::getLabel(int index) {
    
    return detector.labels[index];
    
}

int TrackingManager::getNumDetecteds() {
    
    int result = 0;
    for(int i=0; i<images.size(); i++) {
        
        bool bIsDetected = detector.getDetected(i);
        if(bIsDetected)
            result ++;
    }
    
    return result;
}


void TrackingManager::setProps(float distanceRatio, int nTries) {
    
    detector.distanceRatio = distanceRatio;
    detector.nTries = nTries;
    
}


void TrackingManager::debugDraw(float x, float y) {
    
    int lowest = detector.getLowestScoreIndex();
    
    ofSetColor(255);
    detector.camGrayImg.draw(0.0,600);

    ofPushMatrix();
    ofTranslate(x, y);
    
    for(int i=0; i<images.size(); i++) {
        
            ofDrawBitmapStringHighlight(detector.labels[i] + " " + ofToString(detector.detectedsDistanceResult[i]) + "(" + ofToString(detector.detectedsScore[i]) + ")", 0, i * 20, ofColor(255,0,0), ofColor(255));
        
    }
    
    for(int i=0; i<images.size(); i++) {
        
        bool bIsDetected = detector.getDetected(i);
        if(bIsDetected && i == lowest) {
            ofDrawBitmapStringHighlight(detector.labels[i] + " " +  ofToString(detector.detectedsDistanceResult[i])+ "(" + ofToString(detector.detectedsScore[i]) + ")", 0, i * 20);
        }

    }
    
    ofPopMatrix();
    
}


