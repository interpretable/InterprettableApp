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
    
    ofSetColor(126, 255 * opacityPct.getCurrentValue());
    ofDrawRectRounded(x + rectInnerMargin, y + rectInnerMargin, w - rectInnerMargin * 2, h - rectInnerMargin * 2, radius);
    ofSetColor(255, 255 * opacityPct.getCurrentValue());

    ofImage::draw(x, y, w, h);
    
    
}

void RoundedRectImage::show(float duration, float delay) {
    
    opacityPct.setCurve(EASE_IN);
    opacityPct.setDuration(duration);
    opacityPct.animateToAfterDelay(1.0, delay);
}

void RoundedRectImage::hide(float duration, float delay) {
    
    opacityPct.setCurve(EASE_OUT);
    opacityPct.setDuration(duration);
    opacityPct.animateToAfterDelay(0.0, delay);
    
}
