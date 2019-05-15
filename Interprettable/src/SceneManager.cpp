//
//  SceneManager.cpp
//  Interprettable
//
//  Created by Martial Geoffre-Rouland on 02/05/2019.
//

#include "SceneManager.hpp"

void SceneManager::setup() {
    
    currentScenario = NULL;
    outScenario     = NULL;
    leftImage.load("gabarit.png");
    
    margin = 0;
    rectInnerMargin  = 5;
}

void SceneManager::update() {
    
}

void SceneManager::draw() {
    
    drawBack();

    if(outScenario) {
        drawScenario(outScenario);
    }
    
    if(currentScenario) {
        drawScenario(currentScenario);
    }
    
}

void SceneManager::drawBack() {
    
    leftImage.draw(margin,margin);
    
    
}


void SceneManager::drawScenario(ScenarioData * scenario) {
    
    
    float x = margin + leftImage.getWidth();
    
    ofPushMatrix();
    ofTranslate(x, margin);
    
    size_t nImages = scenario->images.size();
    
    switch(nImages) {
            
        case 0:
            break;
            
        case 1:
            
            for(int i=0; i<scenario->images.size(); i++) {
                scenario->images[i].draw(0.0, 0.0, rectInnerMargin, 20);
            }
            
            break;
            
        case 2: {
            
            float y = 0.0;
            for(int i=0; i<scenario->images.size(); i++) {
                scenario->images[i].draw(0.0, y, rectInnerMargin, 20);
                y +=  currentScenario->images[i].getHeight() + margin;
            }
            
            break;
        }
            
        case 3: {
            
            float y = 0.0;
            for(int i=0; i<scenario->images.size(); i++) {
                scenario->images[i].draw(0.0, y, rectInnerMargin, 20);
                y +=  scenario->images[i].getHeight() + margin;
            }
            
            break;
        }
            
        case 4: {
            
            float y = 0.0;
            x = 0.0;
            for(int i=0; i<scenario->images.size(); i++) {
                
                ofSetColor(255, 255);
                scenario->images[i].draw(x, y, rectInnerMargin, 20);
                if(i < 2) {
                    y += scenario->images[i].getHeight() + margin;
                } else {
                    x += scenario->images[i].getWidth() + margin;
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
        
        size_t nImages = currentScenario->images.size();
        for(int i=0; i<nImages; i++) {
            currentScenario->images[i].hide();
        }
        outScenario     = scenario;

    }
    
    currentScenarioID   = scenario->id;
    currentScenario     = scenario;
    
    size_t nImages = currentScenario->images.size();
    for(int i=0; i<nImages; i++) {
        currentScenario->images[i].show();
    }

    
}

