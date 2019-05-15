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
                currentScenario->images[i].draw(0.0, 0.0, rectInnerMargin, 20);
            }
            
            break;
            
        case 2: {
            
            float y = 0.0;
            for(int i=0; i<currentScenario->images.size(); i++) {
                currentScenario->images[i].draw(0.0, y, rectInnerMargin, 20);
                y +=  currentScenario->images[i].getHeight() + margin;
            }
            
            break;
        }
            
        case 3: {
            
            float y = 0.0;
            for(int i=0; i<currentScenario->images.size(); i++) {
                currentScenario->images[i].draw(0.0, y, rectInnerMargin, 20);
                y +=  currentScenario->images[i].getHeight() + margin;
            }
            
            break;
        }
            
        case 4: {
            
            float y = 0.0;
            x = 0.0;
            for(int i=0; i<currentScenario->images.size(); i++) {
                
                ofSetColor(255, 255);
                currentScenario->images[i].draw(x, y, rectInnerMargin, 20);
                if(i < 2) {
                    y += currentScenario->images[i].getHeight() + margin;
                } else {
                    x += currentScenario->images[i].getWidth() + margin;
                }
            }
            
            break;
        }
            
    }
    
    
    ofPopMatrix();
}

void SceneManager::onScenarioOutHandler() {
    
    
    
    
}


void SceneManager::setScenario(ScenarioData * scenario) {
    
    if(currentScenario) {
        
        int nImages = currentScenario->images.size();
        for(int i=0; i<nImages; i++) {
            currentScenario->images[i].hide();
        }
        
    }
    
    currentScenarioID   = scenario->id;
    currentScenario     = scenario;
    
    int nImages = currentScenario->images.size();
    for(int i=0; i<nImages; i++) {
        currentScenario->images[i].show();
    }

    
}

