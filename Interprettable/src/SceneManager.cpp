//
//  SceneManager.cpp
//  Interprettable
//
//  Created by Martial Geoffre-Rouland on 02/05/2019.
//

#include "SceneManager.hpp"

void SceneManager::setup() {
    
    currentScenario = NULL;
    leftImage.load("gabarit.png");
    
    margin = 0;
    rectInnerMargin  = 5;
}

void SceneManager::update() {
    
}

void SceneManager::draw() {
    
    if(currentScenario) {
        drawScenario();
    }
    
}

void SceneManager::drawBack() {
    
    leftImage.draw(margin,margin);
    
    
}


void SceneManager::drawScenario() {
    
    drawBack();
    
    float x = margin + leftImage.getWidth();
    
    ofPushMatrix();
    ofTranslate(x, margin);
    
    int nImages = currentScenario->images.size();
    
    switch(nImages) {
            
        case 0:
            break;
            
        case 1:
            
            for(int i=0; i<currentScenario->images.size(); i++) {
                currentScenario->images[i].draw(0.0, 0.0);
            }
            
            break;
            
        case 2: {
            
            float y = 0.0;
            for(int i=0; i<currentScenario->images.size(); i++) {
                currentScenario->images[i].draw(0.0, y);
                y +=  currentScenario->images[i].getHeight() + margin;
            }
            
            break;
        }
            
        case 3: {
            
            float y = 0.0;
            for(int i=0; i<currentScenario->images.size(); i++) {
                currentScenario->images[i].draw(0.0, y);
                y +=  currentScenario->images[i].getHeight() + margin;
            }
            
            break;
        }
            
        case 4: {
            
            float y = 0.0;
            x = 0.0;
            for(int i=0; i<currentScenario->images.size(); i++) {
                
                float w =   roundNumberToTen(currentScenario->images[i].getWidth());
                float h =   roundNumberToTen(currentScenario->images[i].getHeight());
                
                //ofSetColor(ofRandom(255), ofRandom(255),ofRandom(255), 255);
                //ofDrawRectRounded(x , y , w , h , 0);
                
                ofSetColor(255, 255);
                ofDrawRectRounded(x + rectInnerMargin, y + rectInnerMargin, w - rectInnerMargin * 2, h - rectInnerMargin * 2, 20);
                ofSetColor(255, 255);
                currentScenario->images[i].draw(x, y, w, h);
                if(i < 2) {
                    y += h + margin;
                } else {
                    x += w + margin;
                }
            }
            
            break;
        }
            
    }
    
    
    ofPopMatrix();
}


void SceneManager::setScenario(ScenarioData * scenario) {
    
    currentScenarioID   = scenario->id;
    currentScenario     = scenario;
    
}

