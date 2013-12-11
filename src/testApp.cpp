#include "testApp.h"

ofImage depth_image;
ofEasyCam cam;

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(30);
	ofSetVerticalSync(true);
	ofBackground(0);
	
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
	device.update();
	for (int i = 0; i < tracker.getNumUser(); i++)
	{
		ofxNiTE2::User::Ref user = tracker.getUser(i);
		
		if(isUserDisplayable(user))
		{
			sendOscMessage(i, "distancebetweenhands", getDistanceBetweenHands(user));
			sendOscMessage(i, "handheightsavg", getHandHeightsAvg(user));
			sendOscMessage(i, "distancefromsensor", getDistanceFromSensor(user));
			cout << i << " - Sending...\n";
		} else {
			cout << i << " - NOTHING\n";
		}
	}
}

void testApp::sendOscMessage(int id, string argName, float value){
	ofxOscMessage m;
	m.setAddress(ofToString(MSG_PREFIX) + "/" +  argName);
	m.addIntArg(id);
	m.addFloatArg(value);
	oscSender.sendMessage(m);
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

//--------------------------------------------------------------
void testApp::draw()
{
	// draw depth
	depth_image.setFromPixels(tracker.getPixelsRef(1000, 4000));
	
	ofSetColor(255);
	depth_image.draw(0, 0);
	
	// draw in 2D
	ofPushView();
	tracker.getOverlayCamera().begin(ofRectangle(0, 0, depth_image.getWidth(), depth_image.getHeight()));
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