//
//  ScenarioData.hpp
//  Interprettable
//
//  Created by Martial Geoffre-Rouland on 02/05/2019.
//

#ifndef ScenarioData_hpp
#define ScenarioData_hpp

#include "ofMain.h"

class ScenarioData {
    
public:
    
    void parse(ofJson & json, int id);
    vector<ofImage> images;
    
    int id;
    
};

#endif /* ScenarioData_hpp */
