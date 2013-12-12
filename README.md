# soundChamber

A full-body interface for interacting with synthesized sound. Uses a blend of [openFrameworks (oF)](http://www.openframeworks.cc/), [openNI](http://www.openni.org/), [Kinect](http://en.wikipedia.org/wiki/Kinect) and [SuperCollider (SC)](http://supercollider.sourceforge.net/) allowing you to control the properties of virtual synthesizers by moving around.

### To install

- Download [openFrameworks v0.8.0 for xcode](http://openframeworks.cc/download/)
- Clone this repo into `myApps/soundChamber`
- Clone [ofxNI2](https://github.com/satoruhiga/ofxNI2) to `addons/ofxNI2` (tested against [this version](https://github.com/satoruhiga/ofxNI2/commit/537854cd1fe93b68b28a18c7568f30f75f4b3256))
- Download [SuperCollider 3.6.5](http://supercollider.sourceforge.net/downloads/)
- Run the SC code in `SoundChamber1.sc`
- Plug in your Kinect
- Build & run from xcode

### To use
- You should see a depth map in the oF window
- Move around in front of the sensor until a skeleton overlay appears
- You should now hear sound and be able to control it by moving around!
