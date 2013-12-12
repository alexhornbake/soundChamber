#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

#define HOST "localhost"
#define PORT 57121
#define MSG_PREFIX "/soundchamber"

class KillUser : public ofThread {
	
	
    void threadedFunction() {
		
		while(isThreadRunning()) {
			cout << "INSIDE THREAD =======\n";
			for(int i = 0; i <= 5; i++) {
				sendOscMessage(_userId, "lostuser", 0);
				sleep(100);
			}
			stopThread();
		}
		
    }
public:
    ofxOscSender oscSender;
	int _userId;
	
	KillUser(int userId)
	{
		_userId = userId;
	}
	
	void sendOscMessage(int id, string argName, float value){
		ofxOscMessage message;
		message.addIntArg(id);
		message.addFloatArg(value);
		message.setAddress(ofToString(MSG_PREFIX) + "/" +  argName);
		oscSender.sendMessage(message);
	}
	
};