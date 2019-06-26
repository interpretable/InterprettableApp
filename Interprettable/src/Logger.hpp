//
//  Logger.hpp
//  Interprettable
//
//  Created by Martial Geoffre-Rouland on 22/05/2019.
//

#ifndef Logger_hpp
#define Logger_hpp

#include "ofMain.h"
#include "ofxHTTP.h"

class Logger {
  
public:
    
    void setUrl(string url);
    void setup();
    void log(string name);
    void log(string name, vector<string> args);
    void logScenario(int id, string thematicName,  string cardname);
    
    void uploadLog(string file);
    void batchUploadLog();
    
    string getTime();
    
private:
    
    ofURLFileLoader http;
    string url;
    
    ofJson logJson;
    string jsonName;
    

};

#endif /* Logger_hpp */
