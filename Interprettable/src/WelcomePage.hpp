//
//  WelcomePage.hpp
//  Interprettable
//
//  Created by Martial Geoffre-Rouland on 05/06/2019.
//

#ifndef WelcomePage_hpp
#define WelcomePage_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxAnimatableFloat.h"

class WelcomePage {
  
public:
    
    void setup();
    void update();
    void draw();
    
    void drawText();
    
    void setOpacity(float pct);
    
    void show();
    void hide();
    
private:
    
    ofxAnimatableFloat opacity;
    void  drawCircleNoise(ofVec2f pos, float radius, ofColor col);
    
    ofImage layerPeople;
    vector<ofImage> textImages;
    vector<float> randoms;

};



#endif /* WelcomePage_hpp */
