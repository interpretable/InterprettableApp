//
//  SceneManager.cpp
//  Interprettable
//
//  Created by Martial Geoffre-Rouland on 02/05/2019.
//

#include "SceneManager.hpp"

void SceneManager::setup() {
    
    currentScenarioID   = - 1;
    currentScenario     = NULL;
    outScenario         = NULL;
    leftImage.load("gabarit.png");
    
    margin = 0;
    rectInnerMargin  = 5;
    
    welcomePage.setup();
    welcomePage.show();
    
    
    leftImagePos.reset(-leftImage.getWidth());
    leftImagePos.setDuration(2.0);
    
    rightImagePos.reset(800);
    rightImagePos.setDuration(2.0);
    
}

void SceneManager::update() {
    
    leftImagePos.update(1.0 / 60.0f);
    rightImagePos.update(1.0 / 60.0f);

    welcomePage.update();
    
    
}

void SceneManager::draw() {
    
    welcomePage.draw();

    
    drawBack();
        

    
    if(outScenario) {
        drawScenario(outScenario);
    }
    
    if(currentScenario && currentScenarioID > 0 ) {
        drawScenario(currentScenario);
    }
        
    
    
}

void SceneManager::drawBack() {
    
    ofSetColor(255);
    leftImage.draw(margin + leftImagePos.getCurrentValue(),margin);
    
    
}


void SceneManager::drawScenario(ScenarioData * scenario) {
    
    
    float x = margin;
    x += rightImagePos.getCurrentValue();
    
    
    ofPushMatrix();
    ofTranslate(x, margin);
    
    int nImages = scenario->images.size();
    
    switch(nImages) {
            
        case 0:
            break;
            
        case 1:
            
            for(int i=0; i<scenario->images.size(); i++) {
                scenario->images[i].draw(0.0, 0.0, rectInnerMargin);
            }
            
            break;
            
        case 2: {
            
            float y = 0.0;
            for(int i=0; i<scenario->images.size(); i++) {
                scenario->images[i].draw(0.0, y, rectInnerMargin);
                y +=  currentScenario->images[i].getHeight() + margin;
            }
            
            break;
        }
            
        case 3: {
            
            float y = 0.0;
            for(int i=0; i<scenario->images.size(); i++) {
                scenario->images[i].draw(0.0, y, rectInnerMargin);
                y +=  scenario->images[i].getHeight() + margin;
            }
            
            break;
        }
            
        case 4: {
            
            float y = 0.0;
            x = 0.0;
            for(int i=0; i<scenario->images.size(); i++) {
                
                ofSetColor(255, 255);
                scenario->images[i].draw(x, y, rectInnerMargin);
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
        
        outScenario     = currentScenario;

        int nImages = outScenario->images.size();
        for(int i=0; i<nImages; i++) {
            outScenario->images[i].hide(1.0,i / 8.0);
        }
        
        ofLogNotice("hiding... ") << currentScenarioID;

       

    }
    if ( scenario->id != currentScenarioID ) {
    
        currentScenarioID   = scenario->id;
        currentScenario     = scenario;
                
        if(currentScenarioID == 0) {
            welcomePage.show();
            leftImagePos.animateTo(-leftImage.getWidth());
            rightImagePos.animateTo(800);

            
        } else {
            
            welcomePage.hide();
            leftImagePos.animateTo(0.0);
            rightImagePos.animateTo(leftImage.getWidth());
            
        
            int nImages = currentScenario->images.size();
            for(int i=0; i<nImages; i++) {
                currentScenario->images[i].show(1.0, i / 8.0);
            }
            
        }
        
    }

    
}

