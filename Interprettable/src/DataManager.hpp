//
//  DataManager.hpp
//  Interprettable
//
//  Created by Martial Geoffre-Rouland on 02/05/2019.
//

#ifndef DataManager_hpp
#define DataManager_hpp

#include "ofMain.h"
#include "ScenarioData.hpp"
class DataManager {
  
public:
    
    void setup();
    void getItems();
    
    void log(){};
    
    ofJson mainJson;
    
    vector<ScenarioData> scenarios;

    
private:
    

};

#endif /* DataManager_hpp */
