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
        
        ofImage image;
        image.load(json["medias"][i]);
        images.push_back(image);
        
        ofLogNotice("jso[i]") << json["medias"][i];

        
    }
    
}

