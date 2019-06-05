//
//  DataManager.cpp
//  Interprettable
//
//  Created by Martial Geoffre-Rouland on 02/05/2019.
//

#include "DataManager.hpp"

void DataManager::setup() {
    
  
    
}

void DataManager::getItems(string url) {
    
    // load main json
    ofHttpResponse resp = ofLoadURL(url);
    mainJson = nlohmann::json::parse(resp.data);
    
    for(int i=0; i<mainJson.size(); i++) {
        
        ScenarioData scenario;
        scenario.parse(mainJson[i], i);
        scenarios.push_back(scenario);
        
    }
    
}

