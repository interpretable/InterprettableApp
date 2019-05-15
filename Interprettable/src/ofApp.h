#pragma once

#include "ofMain.h"
#include "TrackingManager.hpp"
#include "DataManager.hpp"
#include "SceneManager.hpp"
#include "ofxQuadWarp.h"

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
    
        void onMarkerFoundHandler(int & markerId);
        void onMarkerLostHandler(int & markerId);
    
    void exit();
    		
        ofVideoGrabber cam;
    
        DataManager     dataManager;
        TrackingManager trackingManager;
        SceneManager    sceneManager;
    
        ofxQuadWarp warper;
        ofFbo fbo;
        ofPoint points[10];
    
        // for rectangle crop
        ofPoint storedMousePosition;
        ofRectangle cropRectangle;

};
