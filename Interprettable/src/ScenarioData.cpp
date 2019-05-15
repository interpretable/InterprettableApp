//
//  ScenarioData.cpp
//  Interprettable
//
//  Created by Martial Geoffre-Rouland on 02/05/2019.
//

#include "ScenarioData.hpp"

void ScenarioData::parse(ofJson & json, int id) {
    
    this->id = id;
    
    // load images
    for(int i=0; i<json["medias"].size(); i++) {
        
        RoundedRectImage image;
        image.setup(json["medias"][i]);
        images.push_back(image);
        
       // ofLogNotice("json[i]") << json["medias"][i];

        
    }
    
    
    
}

