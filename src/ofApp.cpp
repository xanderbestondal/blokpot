#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	testShape.setup();
	testShape.cylinder(10, 10, 100, 100, false, true);

	//we need to call this for textures to work on models
	//ofDisableArbTex();
	//this makes sure that the back of the model doesn't show through the front
	glEnable(GL_DEPTH_TEST);

	ofSetWindowShape(1280, 800);

	light.setPosition(1000, 1000, 1000);
	light.enable();

	scat.setSurface(testShape);

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

	cam.begin();
	cam.enableOrtho();

	cam.setPosition(0, 0, 1000);

	ofRotate(45, 1, 1, 0);
	testShape.show(ofPoint(0,0,0), ofVec3f(1,0,0));
	scat.setSurface(testShape);
	scat.scatter();
	//testShape.object.drawNormals(false);
	//testShape.object.drawWireframeSet = true;

	//testShape.mapByLight(light.getPosition());

	cam.end();
	//testShape.vertPosToRgb();
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
