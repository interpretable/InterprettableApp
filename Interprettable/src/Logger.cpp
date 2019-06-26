//
//  Logger.cpp
//  Interprettable
//
//  Created by Martial Geoffre-Rouland on 22/05/2019.
//

#include "Logger.hpp"



void Logger::setUrl(string url) {
    
    this->url = url;
    
}


void Logger::setup() {
    
    string time = getTime();
    jsonName = "logs/log_" + time + ".json";
    
    auto jsonObjects = nlohmann::json::array();
    logJson["cards"] = jsonObjects;
    ofSaveJson(jsonName, logJson);
    
    batchUploadLog();
}

void Logger::log(string name) {
    
    logJson[name]["time"] = getTime();
    ofSaveJson(jsonName, logJson);
   // uploadLog();
    
}


void Logger::log(string name, vector<string> args) {
    

    logJson[name]["time"] = getTime();
    ofSaveJson(jsonName, logJson);
    
    
}

void Logger::logScenario(int id, string thematicName,  string cardname) {
    
    auto jsonObject = nlohmann::json::object();
    jsonObject["time"] = getTime();
    jsonObject["thematic_id"] = id;
    jsonObject["card-name"] = cardname;
    jsonObject["thematic-name"] = thematicName;

    logJson["cards"].push_back(jsonObject);
    ofSaveJson(jsonName, logJson);
    
}

void Logger::uploadLog(string file) {
    
    std::vector<ofxHTTP::FormPart> formFiles = {
        ofxHTTP::FormPart(ofxHTTP::FormPart::Type::FILE,
                          "log",
                          file)
    };
    
    // Create a client.
    ofxHTTP::Client client;
    ofxHTTP::PostRequest request(url);
    request.addFormParts(formFiles);
    
    try
    {
        // Execute the request.
        auto response = client.execute(request);
        
        // Check the response.
        if (response->getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
        {
   
            ofLogNotice("Logger::upload") << "logs/" + response->buffer().getText();
            
            // remove quotes
            string url = response->buffer().getText();
            if ( url.front() == '"' ) {
                url.erase( 0, 1 ); // erase the first character
                url.erase( url.size() - 1 ); // erase the last character
            }

            // try to move json to uploaded
            ofFile file;
            file.open("logs/" +  url, ofFile::ReadWrite, false);
            file.moveTo("uploadedlogs/" +  url);
            file.close();
            
        }
        else
        {
            ofLogError("Logger::setup") << response->getStatus() << " " << response->getReason();
        }
    }
    catch (const Poco::Exception& exc)
    {
        ofLogError("Logger::setup") << exc.displayText();
    }
    catch (const std::exception& exc)
    {
        ofLogError("Logger::setup") << exc.what();
    }
}

void Logger::batchUploadLog() {
    
    // here we check if there are still .json files in logs folder
    // if there is, we try to upload them.
    
    string path = "logs";
    ofDirectory dir(path);
    dir.allowExt("json");
    dir.listDir();
    
    //go through and print out all the paths
    for(int i = 0; i < dir.size(); i++){
        
        if(dir.getName(i) != jsonName ) {
            uploadLog("logs/"+dir.getName(i));
        }
       
    }
    
    
}


string Logger::getTime() {
    
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    
    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
    auto str = oss.str();
    
    return str;
    
}
