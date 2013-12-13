#include "testApp.h"

ofImage depth_image;
ofEasyCam cam;

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(30);
	ofSetVerticalSync(true);
	ofBackground(0);
    screenWidth  = ofGetWidth();
    screenHeight = ofGetHeight();

    //Load Video Clip of Static
    clipPlayer.loadMovie(ofToDataPath("../../assets/bad_reception.mov",true));
    clipLowOpacity = 50;
    clipHighOpactiy = 255;
    clipOpacity = clipLowOpacity;
    clipPlayer.play();

	//Setup OSC sender
	oscSender.setup(HOST, PORT);
	
	//Setup Kinect
        device.setup();
	
	if (tracker.setup(device))
	{
		cout << "tracker inited" << endl;
		tracker.setSkeletonSmoothingFactor(0.7);
	}
	
}

void testApp::exit()
{
	tracker.exit();
	device.exit();
}

//--------------------------------------------------------------
void testApp::update()
{
    clipPlayer.update();
    device.update();
	if(ofGetFrameNum() > 1){
		prevFrameUsers = currFrameUsers;
		currFrameUsers.clear();
	}
	for (int i = 0; i < tracker.getNumUser(); i++)
	{
		ofxNiTE2::User::Ref user = tracker.getUser(i);
		int userId = user->getId();
		
		if(isUserDisplayable(user))
		{
			currFrameUsers.push_back(userId);
			
			sendOscMessage(userId, "distancebetweenhands", getDistanceBetweenHands(user));
			sendOscMessage(userId, "handheightsavg", getHandHeightsAvg(user));
			sendOscMessage(userId, "distancefromsensor", getDistanceFromSensor(user));
			cout << userId << " - Sending...\n";
		} else {
			cout << userId << " - NOTHING\n";
		}
	}
	
	for (int i = 0; i < prevFrameUsers.size(); i++)
	{
		int prevUserId = prevFrameUsers.at(i);
		
		if(std::find(currFrameUsers.begin(), currFrameUsers.end(), prevUserId) == currFrameUsers.end())
		{
			//KillUser thread(prevUserId);
			//thread.startThread(true,false);
			//for(int i = 0; i <= 5; i++) {
			ofSleepMillis(100);
			sendOscMessage(prevUserId, "lostuser", 0);
			sendOscMessage(prevUserId, "lostuser", 0);
			ofSleepMillis(100);
			sendOscMessage(prevUserId, "lostuser", 0);
			sendOscMessage(prevUserId, "lostuser", 0);
			//}
			cout << "KILLS USER ID " << prevUserId << "\n";
		}
	}
}

void testApp::sendOscMessage(int id, string argName, float value){
	ofxOscMessage message;
	message.addIntArg(id);
	message.addFloatArg(value);
	message.setAddress(ofToString(MSG_PREFIX) + "/" +  argName);
	oscSender.sendMessage(message);
}

float testApp::getDistanceBetweenHands(ofxNiTE2::User::Ref user)
{
	const ofxNiTE2::Joint &leftHand  = user->getJoint(nite::JOINT_LEFT_HAND);
	const ofxNiTE2::Joint &rightHand = user->getJoint(nite::JOINT_RIGHT_HAND);
	
	ofVec3f lPoint = leftHand.getGlobalPosition();
	ofVec3f rPoint = rightHand.getGlobalPosition();
	
	return ofMap(lPoint.distance(rPoint),100,1500,0,1,true);
}

float testApp::getHandHeightsAvg(ofxNiTE2::User::Ref user)
{
	const ofxNiTE2::Joint &leftHand  = user->getJoint(nite::JOINT_LEFT_HAND);
	const ofxNiTE2::Joint &rightHand = user->getJoint(nite::JOINT_RIGHT_HAND);
	
	float lPoint = leftHand.getGlobalPosition().y;
	float rPoint = rightHand.getGlobalPosition().y;
	
	//cout << "\nleft hand height: " << lPoint;
	//cout << "\nright hand height: " << rPoint;
	
	lPoint = ofMap(lPoint,-1000,725,0,1,true);
	rPoint = ofMap(rPoint,-1000,725,0,1,true);
	float result = (lPoint + rPoint) * 0.5;
	
	//cout << "\nresult: " << result;
	
	return result;
}

float testApp::getDistanceFromSensor(ofxNiTE2::User::Ref user)
{
	
	float centerOfBoneZ = user->getCenterOfBone().z;
	centerOfBoneZ = ofMap(centerOfBoneZ,-700,-4000,0,1,true);
	
	//cout << "\n" << centerOfBoneZ;
	return centerOfBoneZ;
}

bool testApp::isUserDisplayable(ofxNiTE2::User::Ref user)
{
	return user->getNumJoints() > 0 && user->getJoint(nite::JOINT_TORSO).getPositionConfidence() > 0.3;
}

bool testApp::randomInteger(float maxRand)
{
   return (floor(ofRandom(maxRand)) == 0);
}

void testApp::drawJaggedClip()
{
    if(randomInteger(30)){
        if(clipOpacity != clipHighOpactiy){
            clipOpacity = clipHighOpactiy;
        }else{
            clipOpacity = clipLowOpacity;
        }
    }

    ofSetColor(255, 255, 255, clipOpacity);
    clipPlayer.draw(0, 0, screenWidth, screenHeight);
}

//--------------------------------------------------------------
void testApp::draw()
{
    ofEnableAlphaBlending();
    ofEnableBlendMode(OF_BLENDMODE_ADD);

    //static movie clip
    drawJaggedClip();

	// draw depth
	depth_image.setFromPixels(tracker.getPixelsRef(1000, 4000));
	
	ofSetColor(255,255,255,127);
	depth_image.draw(0, 0, screenWidth, screenHeight);
	
	// draw in 2D
	ofPushView();
	tracker.getOverlayCamera().begin(ofRectangle(0, 0, screenWidth, screenHeight));
	for (int i = 0; i < tracker.getNumUser(); i++)
	{
		ofxNiTE2::User::Ref user = tracker.getUser(i);
		
		if(isUserDisplayable(user))
		{
			user->draw();
		}
	}
	tracker.getOverlayCamera().end();
	ofPopView();
    ofDisableAlphaBlending();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{

}