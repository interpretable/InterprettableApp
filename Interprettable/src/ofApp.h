#pragma once

#include "ofMain.h"
#include "TrackingManager.hpp"
#include "DataManager.hpp"
#include "SceneManager.hpp"
#include "ofxQuadWarp.h"
#include "Logger.hpp"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void exit();
    
        void onMarkerFoundHandler(int & markerId);
        void onMarkerLostHandler(int & markerId);
    
        void loadConfigJson();
    		    
        DataManager     dataManager;
        TrackingManager trackingManager;
        SceneManager    sceneManager;
    
        ofxQuadWarp warper;
        ofFbo fbo;
        ofPoint points[10];
    
        bool bDebugMode, bDebugWarpMode;
    
        // for rectangle crop
        ofPoint storedMousePosition;
        ofRectangle cropRectangle;
    
        int currentTimeMillis, currentShutDownMillis;
        Logger logger;
    
        // main config
        ofJson configJson;
        ofFile configJsonFile;
        int configJsonTimeStamp;
    
        int machineId;
    
#ifdef __linux__

        ofGstVideoUtils cam;
    
#else
    
        ofVideoGrabber cam;
    
#endif
    
};
