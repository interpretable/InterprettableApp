//
//  RoundedRectImage.hpp
//  Interprettable
//
//  Created by Martial Geoffre-Rouland on 15/05/2019.
//

#ifndef RoundedRectImage_hpp
#define RoundedRectImage_hpp

#include "ofMain.h"
#include "ofxAnimatableFloat.h"

class RoundedRectImage : public ofImage {
    
public:
    
    void setup(string url);
    void update();
    void draw(float x, float y, float rectInnerMargin, float radius = 10);
    
    void show(float duration = 1.0, float delay = 0.0);
    void hide(float duration = 1.0, float delay = 0.0);
    
private:
    
    ofxAnimatableFloat opacityPct;
    string url;
};

#endif /* RoundedRectImage_hpp */
