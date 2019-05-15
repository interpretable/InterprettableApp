//
//  TrackingManager.cpp
//  Interprettable
//
//  Created by Martial Geoffre-Rouland on 02/05/2019.
//

#include "TrackingManager.hpp"

void TrackingManager::setup() {
    
    detector.setup();
    
    /*
    string path = "markers";
    ofDirectory dir(path);
    dir.allowExt("jpg");
    dir.listDir();
    
    //go through and print out all the paths
    for(int i = 0; i < dir.size(); i++){
        
        ofImage img;
        img.load(dir.getPath(i));
        detector.addImageToTrack(img, dir.getFile(i).getFileName());
        images.push_back(img);
        detecteds.push_back(false);
    }
    
     */

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

void TrackingManager::debugDraw(float x, float y) {
    
    ofPushMatrix();
    ofTranslate(x, y);
    
    for(int i=0; i<images.size(); i++) {
        
        bool bIsDetected = detector.getDetected(i);
        if(bIsDetected) {
            ofDrawBitmapStringHighlight(detector.labels[i], 0, i * 20);
        }

    }
    
    ofPopMatrix();
    
}


