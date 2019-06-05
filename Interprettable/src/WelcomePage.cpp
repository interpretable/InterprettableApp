//
//  WelcomePage.cpp
//  Interprettable
//
//  Created by Martial Geoffre-Rouland on 05/06/2019.
//

#include "WelcomePage.hpp"


void WelcomePage::setup() {
    
    opacity.reset(0);
    opacity.setDuration(2.0);
    
    layerPeople.load("assets/welcome/layer_people.png");
    
    string path = "assets/welcome/text";
    ofDirectory dir(path);
    dir.allowExt("png");
    dir.listDir();
    
    //go through and print out all the paths
    for(int i = 0; i < dir.size(); i++){
        
        ofImage img;
        img.load(dir.getPath(i));
        textImages.push_back(img);
        
        randoms.push_back(ofRandom(10000));
        
    }
    
}

void WelcomePage::update() {
    
    opacity.update(1.0 / 60.0f);
    
}

void WelcomePage::draw() {
    
    ofPushMatrix();
    ofTranslate(400,400);
    float scale = ofMap(opacity.getCurrentValue(), 0, 255, 0.8, 1.0);
    ofScale(scale, scale);
    ofTranslate(-400, -400);
    
    if( opacity.getCurrentValue() > 0.0 ) {
    
        drawCircleNoise(ofVec2f(200,200), 100, ofColor(198, 223, 227, opacity.getCurrentValue()));
        drawCircleNoise(ofVec2f(600,200), 100, ofColor(198, 223, 227, opacity.getCurrentValue()));
        drawCircleNoise(ofVec2f(200,600), 100, ofColor(198, 223, 227, opacity.getCurrentValue()));
        drawCircleNoise(ofVec2f(600,600), 100, ofColor(198, 223, 227, opacity.getCurrentValue()));
        
        layerPeople.draw(0,0);
        
        drawText();
        
    }
            
    ofPopMatrix();

    
}

void WelcomePage::drawText() {
    
    for(int i = 0; i < textImages.size(); i++){

        
        float alpha = 0.5 + cos((ofGetElapsedTimeMillis() + randoms[i]) / 1000) * 0.5;
        alpha *= ofNormalize(opacity.getCurrentValue(), 0, 255);
        ofSetColor(255, alpha * 255);
        textImages[i].draw(0.0,0.0);
        
    }
    
}


void setOpacity(float pct) {
    
    
}

void WelcomePage::show() {
    
    
    opacity.animateTo(255);
    
}
void WelcomePage::hide() {
    opacity.animateTo(0.0);

}


void WelcomePage::drawCircleNoise(ofVec2f pos, float radius, ofColor col) {
    
    int len = 15;
    
    
    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    
    vector<glm::vec2> out_vertices;
    vector<glm::vec2> in_vertices;
    for (int deg = 0; deg < 360; deg += 8) {
        
        auto noise_point = glm::vec2(pos.x + radius * cos(deg * DEG_TO_RAD), pos.y + radius * sin(deg * DEG_TO_RAD));
        float noise_len = ofMap(ofNoise(noise_point.x * 0.01, noise_point.y * 0.01, ofGetFrameNum() * 0.01), 0, 1, 5, len);
        
        float r = radius + noise_len;
        out_vertices.push_back(glm::vec2(r * cos(deg * DEG_TO_RAD), r * sin(deg * DEG_TO_RAD)));
        
        
    }
    
    ofBeginShape();
    ofSetColor(col);
    ofVertices(out_vertices);
    ofEndShape(true);
    
    ofPopMatrix();
    
}
