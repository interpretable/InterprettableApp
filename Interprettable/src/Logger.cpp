//
//  Logger.cpp
//  Interprettable
//
//  Created by Martial Geoffre-Rouland on 22/05/2019.
//

#include "Logger.hpp"

void Logger::setup() {
    
    string time = getTime();
    jsonName = "logs/log_" + time + ".json";
    
    auto jsonObjects = nlohmann::json::array();
    logJson["scenarios"] = jsonObjects;
    ofSaveJson(jsonName, logJson);
}

void Logger::log(string name) {
    
    logJson[name]["time"] = getTime();
    ofSaveJson(jsonName, logJson);
    
    
}


void Logger::log(string name, vector<string> args) {
    

    logJson[name]["time"] = getTime();
    ofSaveJson(jsonName, logJson);
    
    
}

void Logger::logScenario(string name) {
    
    auto jsonObject = nlohmann::json::object();
    jsonObject["time"] = getTime();
    jsonObject["name"] = name;
    
    logJson["scenarios"].push_back(jsonObject);
    ofSaveJson(jsonName, logJson);

    
}


string Logger::getTime() {
    
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    
    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
    auto str = oss.str();
    
    return str;
    
}
