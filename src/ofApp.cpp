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


	testShape.setup();
	testShape.cylinder(10, 10, 200, 200, false, true);

	map.setMeshBase(testShape.object);
	map.noiseMap(1,.5, ofVec3f(1,1,1),0);

	deform.setMeshBase(testShape.object);
	deform.setMap(map.vertMap);
	deform.deformMultiplier = 1;
	testShape.object.mesh = deform.deform(1, 0);

	scat.setSurfaceShape(testShape);

	//deform.deform(1, 0);

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
	ofPushMatrix();
	cam.begin();
	//cam.enableOrtho();
	cam.setPosition(0, 0, 400);


	ofRotate(ofGetFrameNum(), 1, 0, 0);
	testShape.show(ofPoint(0,0,0), ofVec3f(1,0,0));
	//scat.setSurface(testShape);
	scat.scatter();

	//testShape.object.drawNormals(false);
	//testShape.object.drawWireframeSet = true;

	//testShape.mapByLight(light.getPosition());

	cam.end();
	//testShape.vertPosToRgb();
	ofPopMatrix();
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
