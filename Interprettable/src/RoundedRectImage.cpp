//
//  RoundedRectImage.cpp
//  Interprettable
//
//  Created by Martial Geoffre-Rouland on 15/05/2019.
//

#include "RoundedRectImage.hpp"


void RoundedRectImage::setup(string url) {
    
    this->load(url);
    
    opacityPct.reset(0.0);
    opacityPct.setRepeatType(PLAY_ONCE);

    
}

void RoundedRectImage::update() {
    
    
}

void RoundedRectImage::draw(float x, float y, float rectInnerMargin, float radius) {
    
    opacityPct.update(1.0 / 60.0f);

    
    float w =   getWidth();
    float h =   getHeight();
    
    ofSetColor(255, 255 * opacityPct.getCurrentValue());
    ofDrawRectRounded(x + rectInnerMargin, y + rectInnerMargin, w - rectInnerMargin * 2, h - rectInnerMargin * 2, radius);
    ofImage::draw(x, y, w, h);
    
}

void RoundedRectImage::show() {
    
    opacityPct.setCurve(EASE_IN);
    opacityPct.setDuration(0.55);
    opacityPct.animateTo(1.0);
}

void RoundedRectImage::hide() {
    opacityPct.setCurve(EASE_OUT);
    opacityPct.setDuration(0.55);
    opacityPct.animateTo(0.0);
}
