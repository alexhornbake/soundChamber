#pragma once

#include "ofMain.h"

#include "ofxNI2.h"
#include "ofxNiTE2.h"

#include "ofxOsc.h"

#define HOST "localhost"
#define PORT 12345
#define MSG_PREFIX "soundchamber"

class testApp : public ofBaseApp
{
public:
	void setup();
	void exit();
	
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
	ofxNI2::Device device;
	ofxNiTE2::UserTracker tracker;

private:
    
    float getDistanceBetweenHands(ofxNiTE2::User::Ref user);
    float getHandHeightsAvg(ofxNiTE2::User::Ref user);
    float getDistanceFromSensor(ofxNiTE2::User::Ref user);

    ofxOscSender oscSender;
    void sendOscMessage(int id, string argName, float value);
};