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

class SceneManager {
  
public:
    
    void setup();
    void update();
    void draw();
    
    void drawBack();
    void drawScenario();
    void setScenario(ScenarioData * scenario);
    
private:
    
    ofImage leftImage;
    
    int currentScenarioID;
    ScenarioData * currentScenario;
    
    int margin;
    int rectInnerMargin;
    
    int roundNumberToTen(int num)
    {
        int rem = num % 10;
        return rem >= 5 ? (num - rem + 10) : (num - rem);
    }
    
};

#endif /* SceneManager_hpp */
