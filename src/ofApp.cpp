#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//we need to call this for textures to work on models
	//ofDisableArbTex();
	//this makes sure that the back of the model doesn't show through the front
	glEnable(GL_DEPTH_TEST);

	ofSetWindowShape(1280, 800);

	light.setPosition(1000, 1000, 1000);
	light.enable();

	setupCam();

	testShape.setup();
	testShape.cylinder(10, 10, 200, 200, false, true);
	//testShape.object.solidShading();


	map.setMeshBase(&testShape.object);
	map.noiseMap(1,-.5, ofVec3f(1,1,1),0);

	deform.setMeshBase(&testShape.object);
	deform.setMap(map.vertMap);
	deform.deformMultiplier = 100;
	deform.deform(1, 0);

	scat.setSurfaceShape(testShape);


	//testShape.loadObj("meshes/sam.obj");

	//cam.begin();
	//cam.setPosition(0, 0, 1000);
	//cam.enableOrtho();
	//testShape.mapPlannar(cam);
	////testShape.setTexture(displaceImg);
	//cam.end();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	//webcam stuff
	motionDetector.update();
	//motionDetector.colorImg.draw(0, 0, ofGetWidth(), ofGetHeight());

	ofPushMatrix();
	cam.begin();
	//cam.enableOrtho();
	cam.setPosition(0, 0, 400);
	
	map.mapPlannar(cam);

	testShape.object.setTexture(motionDetector.colorImg.getPixels());
	ofRotate(ofGetFrameNum(), 1, 0, 0);
	testShape.show(ofPoint(0,0,0), ofVec3f(1,0,0));
	//testShape.object.loopTexture();
	//scat.setSurface(testShape);
	//scat.scatter();



	//testShape.object.drawNormals(false);
	//testShape.object.drawWireframeSet = true;

	//testShape.mapByLight(light.getPosition());

	cam.end();
	//testShape.vertPosToRgb();
	ofPopMatrix();



}


void ofApp::setupCam() {
	//camWidth = 640;	// try to grab at this size.
	//camHeight = 480;
	//camWidth = 1000;
	//camHeight = 1000;
	camWidth = 1280;
	camHeight = 800;
	cout << "-------------" << endl;
	cout << motionDetector.vidGrabber.isInitialized() << endl;
	motionDetector.setup(camWidth, camHeight);
	motionDetector.hMirror = true;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
