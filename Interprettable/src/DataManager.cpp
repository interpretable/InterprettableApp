//
//  DataManager.cpp
//  Interprettable
//
//  Created by Martial Geoffre-Rouland on 02/05/2019.
//

#include "DataManager.hpp"

void DataManager::setup() {
    
  
    
}

void DataManager::getItems() {
    
    // load main json
    ofHttpResponse resp = ofLoadURL("http://interpretable.erasme.org/api/public/api/items");
    cout << resp.data << endl;
    mainJson = nlohmann::json::parse(resp.data);
    
    for(size_t i=0; i<mainJson.size(); i++) {
        
        ScenarioData scenario;
        scenario.parse(mainJson[i], i);
        scenarios.push_back(scenario);
        
    }
    
}

