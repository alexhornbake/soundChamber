#include "testApp.h"

ofImage depth_image;
ofEasyCam cam;

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(30);
	ofSetVerticalSync(true);
	ofBackground(0);
	
	device.setup();
	
	if (tracker.setup(device))
	{
		cout << "tracker inited" << endl;
	}
    tracker.setSkeletonSmoothingFactor(0.7);
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
		const ofxNiTE2::Joint &leftHand  = user->getJoint(nite::JOINT_LEFT_HAND);
		const ofxNiTE2::Joint &rightHand = user->getJoint(nite::JOINT_RIGHT_HAND);
        //cout << "\nleft: [" << leftHand.getX() << " " << leftHand.getY() << " " << leftHand.getZ() << "]";
        //cout << "\nright: [" << rightHand.getX() << " " << rightHand.getY() << " " << rightHand.getZ() << "]";
        
        //cout << "\n" << i << ": " << leftHand.getX();
        
        
        
        
		//ofVec3f centerOfMass = user->getCenterOfMass();
        //cout << "\n" << centerOfMass;
        
        
		ofVec3f centerOfBone = user->getCenterOfBone();
        cout << "\n" << centerOfBone;
    }
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
	ofDrawAxis(100);
	tracker.draw();
	tracker.getOverlayCamera().end();
	ofPopView();
	
	// draw in 3D
	cam.begin();
	ofDrawAxis(100);
	tracker.draw();
	
	// draw box
	ofNoFill();
	ofSetColor(255, 0, 0);
	for (int i = 0; i < tracker.getNumUser(); i++)
	{
		ofxNiTE2::User::Ref user = tracker.getUser(i);
		const ofxNiTE2::Joint &joint = user->getJoint(nite::JOINT_HEAD);
		
		joint.transformGL();
		ofBox(300);
		joint.restoreTransformGL();
	}
	
	cam.end();
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