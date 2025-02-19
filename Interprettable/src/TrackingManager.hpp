//
//  TrackingManager.hpp
//  Interprettable
//
//  Created by Martial Geoffre-Rouland on 02/05/2019.
//

#ifndef TrackingManager_hpp
#define TrackingManager_hpp

#include <stdio.h>
#include "ofxFeatureDetector.hpp"

class TrackingManager {
  
public:
    
    void setup();
    void addImage(string url);
    void start();
    void update(ofPixels & input);
    
    int getNumDetecteds();
    string getLabel(int index);
    void debugDraw(float x, float y);
    
    ofEvent<int> onMarkerFound;
    ofEvent<int> onMarkerLost;
    
    void setProps(float distanceRatio, int nTries);
    ofxFeatureDetector  detector;

    
private:
    
    vector<ofImage>     images;
    vector<bool>        detecteds;
    
    
};

#endif /* TrackingManager_hpp */
