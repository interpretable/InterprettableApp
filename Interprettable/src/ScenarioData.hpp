//
//  ScenarioData.hpp
//  Interprettable
//
//  Created by Martial Geoffre-Rouland on 02/05/2019.
//

#ifndef ScenarioData_hpp
#define ScenarioData_hpp

#include "ofMain.h"
#include "RoundedRectImage.hpp"

class ScenarioData {
    
public:
    
    void parse(ofJson & json, int id);
    vector<RoundedRectImage> images;
    
    int id;
    string theme;
};

#endif /* ScenarioData_hpp */
