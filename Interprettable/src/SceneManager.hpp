//
//  SceneManager.hpp
//  Interprettable
//
//  Created by Martial Geoffre-Rouland on 02/05/2019.
//

#ifndef SceneManager_hpp
#define SceneManager_hpp

#include "ofMain.h"
#include "ScenarioData.hpp"
#include "WelcomePage.hpp"
#include "ofxAnimatableFloat.h"

class SceneManager {
  
public:
    
    void setup();
    void update();
    void draw();
    
    void drawBack();
    void drawScenario(ScenarioData * scenario);
    void setScenario(ScenarioData * scenario);
    
    void onScenarioOutHandler();
    
private:
    
    ofImage leftImage;
    ofxAnimatableFloat leftImagePos;
    
    int currentScenarioID;
    ScenarioData * currentScenario;
    ScenarioData * outScenario;

    int margin;
    int rectInnerMargin;
    
    WelcomePage welcomePage;
    
    // time handling
    int currentTimeMillis;
    
    int roundNumberToTen(int num)
    {
        int rem = num % 10;
        return rem >= 5 ? (num - rem + 10) : (num - rem);
    }
    
};

#endif /* SceneManager_hpp */
