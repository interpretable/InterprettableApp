//
//  Logger.hpp
//  Interprettable
//
//  Created by Martial Geoffre-Rouland on 22/05/2019.
//

#ifndef Logger_hpp
#define Logger_hpp

#include "ofMain.h"

class Logger {
  
public:
    
    void setup();
    void log(string name);
    void log(string name, vector<string> args);
    
    void logScenario(string name);

    
    string getTime();
    
private:
    
    ofJson logJson;
    string jsonName;
    

};

#endif /* Logger_hpp */
