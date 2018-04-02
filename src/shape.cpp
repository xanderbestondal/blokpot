#include "shape.h"
#include <iostream>

#include <GL/gl.h>
#include <map>


//--------------------------------------------------------------
void shape::setup() {

	//loadObj("data/mappingfiles/3D.obj");
	cout << "STAAAAAAAAAAAAAAAAART" << endl;
	blendshapeLoaded = false;
	drawWireframeSet = false;
	drawNormalsSet = false;
}

void shape::resetMesh() {

	testMesh.clear();
	vertsBasePos.resize(0);
	vertsDeformedPos.resize(0);
	vertsDeformeFrom.resize(0);
}
ofMesh shape::loadObj(string filename){
    ifstream in(filename, ios::in);
    if (!in)
    {
        cerr << "Cannot open " << filename << endl;
        //exit(1);
    }

	resetMesh();
	
	loadedModel.loadModel(filename);
	testMesh = loadedModel.getMesh(0);
	
	//cout << testMesh.getNumVertices() << endl;

	vertsBasePos = testMesh.getVertices();
	vertsDeformedPos = testMesh.getVertices();
	vertsDeformeFrom = testMesh.getVertices();
	vertNormals = testMesh.getNormals();
	setNormals(testMesh);
	vertsDeformeFrom = vertsBasePos;
	vertsBaseNormals = testMesh.getNormals();

	return testMesh;
}
ofMesh shape::loadObjBlendshape(const char* filename) {
	ifstream in(filename, ios::in);
	if (!in)
	{
		cerr << "Cannot open " << filename << endl;
		//exit(1);
	}

	loadedModel.loadModel(filename);
	blendshapeMesh = loadedModel.getMesh(0);
	blendshapeLoaded = true;
	return blendshapeMesh;
}
void shape::circle(int steps, float radius, ofVec3f pos){
    float angle = 0;
    for (int r = 0; r < steps; r++){

        float x = sin(angle)*radius;
        float z = cos(angle)*radius;

        ofPoint newPos(x+pos.x,0+pos.y,z+pos.z);
        testMesh.addVertex(newPos);
        vertsBasePos.push_back(newPos);
        vertsDeformedPos.push_back(newPos);
        vertMap.push_back(0);

        testMesh.addTexCoord(ofVec2f(0,0));

        angle += TWO_PI / steps;

    }
}

ofMesh shape::plane(int widthSegments, int heightSegments, float width, float height) {

	resetMesh();

	widthSegments += 1;
	heightSegments += 1;

	tris = 0;
	ofVec3f pos(0, 0, 0);

	vertsTris.resize(heightSegments*widthSegments);
	vertNormals.resize(heightSegments*widthSegments);

	// set points
	for (int y = 0; y < heightSegments; y++) {
		for (int x = 0; x < widthSegments; x++) {
			pos.y = float(y) / heightSegments*height - height/2+ (height/ heightSegments)/2;
			pos.x = (float(x)/(widthSegments-1))*(width)-width/2;// - width / 2 + (width / widthSegments) / 2;

			ofPoint newPos(pos.x, pos.y, pos.z);
			testMesh.addVertex(newPos);

			vertsBasePos.push_back(newPos);
			vertsDeformedPos.push_back(newPos);
			vertMap.push_back(0);

			testMesh.addTexCoord(ofVec2f(0, 0));
		}
	}

	// set tris
	for (int i = 0; i < (heightSegments - 1)*widthSegments; i++) {

		if (!(i != 0 && (i + 1) % widthSegments == 0)) {

			makeTri(i, i + widthSegments, i + 1);
			makeTri(i + 1, i + widthSegments, i + 1 + widthSegments);
		}

	}

	//tex.loadScreenData(0, 0, 1, 1);
	setNormals(testMesh);
	vertsDeformeFrom = vertsBasePos;
	vertsBaseNormals = testMesh.getNormals();
	return testMesh;
	
}
ofMesh shape::cylinder(int steps,int heightSegments, float height, float radius,bool sphere, bool cone){

	resetMesh();

    float angle = 0;
    tris = 0;
    ofVec3f pos(0,0,0);

    //testMesh.setMode( OF_PRIMITIVE_LINES );

    vertsTris.resize((heightSegments+1)*steps);
    vertNormals.resize((heightSegments + 1)*steps);

	for (int i = 0; i < heightSegments + 1; i++) {
		float sphereRadius = radius;
		float circlePos = float(i) / heightSegments * radius;

		if (sphere) {
			sphereRadius = sin(float(i) / heightSegments * PI)*radius;
			circlePos = cos(float(i) / heightSegments * PI)*radius;
		}

		pos.y = -circlePos;
		circle(steps, sphereRadius, pos);
	}

    if(cone){
        for (int i = 0; i < heightSegments; i++){
            pos.y = float(i)/heightSegments*height;
            circle(steps,-float(heightSegments-1-i)/heightSegments*radius,pos);
        }
    }

    for (int i = 0; i < (heightSegments)*steps; i++){
        if(i!=0 && (i+1)%steps == 0){
            makeTri(i+steps, i,i+1-steps);
            makeTri(i+1-steps,i+1,i+steps);
        }else{
            makeTri(i+steps, i,i+1);
            makeTri(i+steps, i + 1,i+1+steps);
        }
    }
    //tex.loadScreenData(0, 0, 1, 1);
    setNormals(testMesh);
    vertsDeformeFrom = vertsBasePos;
    vertsBaseNormals = testMesh.getNormals();
    return testMesh;
}
void shape::setNormals( ofMesh &mesh ){
	//The number of the vertices
	int nV = mesh.getNumVertices();

	//The number of the triangles
	int nT = mesh.getNumIndices() / 3;

	vector<ofPoint> norm( nV );	//Array for the normals

	//Scan all the triangles. For each triangle add its
	//normal to norm's vectors of triangle's vertices
    polyNormals.resize(0);
	for (int t=0; t<nT; t++) {
		//Get indices of the triangle t

		int i1 = mesh.getIndex( 3 * t );
		int i2 = mesh.getIndex( 3 * t + 1 );
		int i3 = mesh.getIndex( 3 * t + 2 );

		//Get vertices of the triangle
		const ofPoint &v1 = mesh.getVertex( i1 );
		const ofPoint &v2 = mesh.getVertex( i2 );
		const ofPoint &v3 = mesh.getVertex( i3 );

		//Compute the triangle's normal
		ofPoint dir = ( (v2 - v1).crossed( v3 - v1 ) ).normalized();
		polyNormals.push_back(dir);

		//Accumulate it to norm array for i1, i2, i3
		norm[ i1 ] += dir;
		norm[ i2 ] += dir;
		norm[ i3 ] += dir;
	}

	//Normalize the normal's length
	for (int i=0; i<nV; i++){
		norm[i].normalize();
	}

	//Set the normals to mesh
	mesh.clearNormals();
	mesh.addNormals( norm );
 }
void shape::makeTri(int vert1,int vert2,int vert3){

    testMesh.addIndex(vert1);
    testMesh.addIndex(vert2);
    testMesh.addIndex(vert3);
    vertsTris[vert1].push_back(tris);
    vertsTris[vert2].push_back(tris);
    vertsTris[vert3].push_back(tris);
//solidShading();
    tris += 1;
// TEMP NORMALS
    testMesh.addNormal(ofPoint(0,0,0));
}
void shape::solidShading(){

//    remove all verts and tris
//    rebuild like this:
//    vertsTemp = testMesh.getVertices();
    vector<ofVec3f> vertsTemp;

    for(int i = 0; i < testMesh.getNumIndices(); i+=3){
        vertsTemp.push_back(testMesh.getVertex(testMesh.getIndex(i)));
        vertsTemp.push_back(testMesh.getVertex(testMesh.getIndex(i+1)));
        vertsTemp.push_back(testMesh.getVertex(testMesh.getIndex(i+2)));
    }
    testMesh.clearVertices();
    testMesh.clearIndices();
    for(int i = 0; i < vertsTemp.size(); i++){
        testMesh.addVertex(vertsTemp[i]);
    }
    testMesh.setupIndicesAuto();
}
ofVec3f shape::makeTriNormal(ofVec3f p1,ofVec3f p2,ofVec3f p3){
    ofVec3f U = (p3 - p1);
    ofVec3f V = (p2 - p1);

    ofVec3f n(0,0,0);
    n.x = ( U.y * V.z) - ( U.z * V.y);
    n.y = ( U.z * V.x) - ( U.x * V.z);
    n.z = ( U.x * V.y) - ( U.y * V.x);

    //n = U.cross(V);
    n.normalize();
    return n;
}
void shape::drawNormals(bool faces){

    float len = 10;
	ofPushStyle();
    if(faces){
		scatterMeshes.resize(polyNormals.size());
        for(int i = 0; i < polyNormals.size(); i++){
            ofPoint mid = (testMesh.getVertex(testMesh.getIndex(i*3))+testMesh.getVertex(testMesh.getIndex(i*3+1))+testMesh.getVertex(testMesh.getIndex(i*3+2)))/3;
            ofSetColor(0,0,255);
            ofLine(mid.x,mid.y,mid.z,mid.x+polyNormals[i].x*len,mid.y+polyNormals[i].y*len,mid.z+polyNormals[i].z*len);
			
			//scatterMeshes.push_back(ofMesh::cone(100.0, 200.0));
        }
    }else{
        for(int i = 0; i < vertNormals.size(); i++){
            ofSetColor(255,255,255);
            ofLine(testMesh.getVertex(i).x,testMesh.getVertex(i).y,testMesh.getVertex(i).z,testMesh.getVertex(i).x+testMesh.getNormal(i).x*len,testMesh.getVertex(i).y+testMesh.getNormal(i).y*len,testMesh.getVertex(i).z+testMesh.getNormal(i).z*len);
        }
    }
	ofPopStyle();
}
void shape::mapPlannar(ofCamera cam, bool mirrorX){
    testMesh.clearTexCoords();
    for(int i = 0; i < testMesh.getNumVertices(); i++){
        //vertMap.push_back( ofNoise(vertsBasePos[i].x*frequency*scale[0],vertsBasePos[i].y*frequency*scale[1],vertsBasePos[i].z*frequency*scale[2]));

	    ofVec2f uv = cam.worldToScreen(vertsDeformedPos[i]);
		if (mirrorX) {
			uv = ofPoint(1 - uv.x / (ofGetWidth()), uv.y / (ofGetHeight()));
		}else{
			uv = ofPoint(uv.x / (ofGetWidth()), uv.y / (ofGetHeight()));
		}

		if (uv.x < 0) { uv.x = .01; }
		if (uv.x > 1) { uv.x = .99; }
		if (uv.y < 0) { uv.y = .01; }
		if (uv.y > 1) { uv.y = .99; }

        testMesh.addTexCoord(uv);
    }
}

void shape::storeBoundingBox() {

	float Xmin = 0;
	float Xmax = 0;
	float Ymin = 0;
	float Ymax = 0;
	float Zmin = 0;
	float Zmax = 0;
	for (int i = 0; i < testMesh.getNumVertices(); i++) {

		if (testMesh.getVertex(i).x < Xmin)
			Xmin = testMesh.getVertex(i).x;
		if (testMesh.getVertex(i).x > Xmax)
			Xmax = testMesh.getVertex(i).x;
	}

}
void shape::vertPosToRgb() {
	vertMap.resize(0);
	int width = 10;
	int Xpos = 0;
	int Ypos = 0;
	int drawWidth = sqrt(testMesh.getNumVertices());
	for (int i = 0; i < testMesh.getNumVertices(); i++) {
		ofPushStyle;
		ofFill();
		ofSetColor(testMesh.getVertex(i).x , testMesh.getVertex(i).y , testMesh.getVertex(i).z);
		ofRect(Xpos, Ypos, 0, width,width);
		ofPopStyle;
			
		Xpos += width;
		if (i % drawWidth == 0) {
			Ypos += width;
			Xpos = 0;
		}
	}
}
void shape::mapByLight(ofVec3f point){
    for(int i = 0; i < polyNormals.size(); i++){
        ofPoint mid = (testMesh.getVertex(testMesh.getIndex(i*3))+testMesh.getVertex(testMesh.getIndex(i*3+1))+testMesh.getVertex(testMesh.getIndex(i*3+2)))/3;
        ofPoint lightDir(1,0,0);
        float dot(((polyNormals[i].dot(lightDir))+1)/2);
        for(int t = 0; t < 3; t++){
            testMesh.setTexCoord(testMesh.getIndex(i*3+t),ofPoint(dot,ofRandom(1)));
        }
    }
}
void shape::noiseMap(float frequency,float offset,ofVec3f scale, float yPos){//, float zPos){
    vertMap.resize(0);
    for(int i = 0; i < testMesh.getNumVertices(); i++){
        vertMap.push_back( ofNoise(vertsBasePos[i].x*frequency*scale[0],vertsBasePos[i].y*frequency*scale[1]+yPos,vertsBasePos[i].z*frequency*scale[2]) + offset);
    }
}
void shape::displaceMap(ofImage dispImg, float offset) {
	vertMap.resize(0);
	displaceImg = dispImg;
	for (int i = 0; i < testMesh.getNumVertices(); i++) {
		ofVec2f uv = testMesh.getTexCoord(i);
		ofColor col = dispImg.getColor(uv.x*dispImg.getWidth(), uv.y*dispImg.getHeight());
		vertMap.push_back(col.getBrightness() / 255.0f + offset);
	}
}
void shape::gradientMap(){
    vertMap.resize(0);
    float height = 500;
    for(int i = 0; i < testMesh.getNumVertices(); i++){
        vertMap.push_back( vertsBasePos[i].y/height);
    }
}
void shape::solidDeform(int iterations){
    for(int i = 0; i < testMesh.getNumVertices(); i+=3){
        ofVec3f deformedPos(0,ofRandom(30),0);
        testMesh.setVertex(i,(testMesh.getVertex(i)+deformedPos));
        testMesh.setVertex(i+1,(testMesh.getVertex(i+1)+deformedPos));
        testMesh.setVertex(i+2,(testMesh.getVertex(i+2)+deformedPos));
    }
}
void shape::deform(int iterations, float Yfavor){
    vertsDeformeFrom = vertsBasePos;
    normalsDeformeFrom = vertsBaseNormals;
    for(int iter = 0; iter < iterations; iter++){
        vertsDeformedPos.resize(0);
        for(int i = 0; i < testMesh.getNumVertices(); i++){
			float yNormal = 1.0;
			if (Yfavor != 0) {
				yNormal = normalsDeformeFrom[i].y * Yfavor;
			}
			ofPoint melt = ofPoint(0, Yfavor*(deformMultiplier/2), 0)*(vertMap[i]);
            ofVec3f deformedPos((vertMap[i]-.5) *deformMultiplier * normalsDeformeFrom[i]+ melt + vertsDeformeFrom[i] + normalsDeformeFrom[i]*deformMultiplier + melt);
            testMesh.setVertex(i,deformedPos);
            vertsDeformedPos.push_back(deformedPos);
        }
        vertsDeformeFrom = vertsDeformedPos;
        setNormals(testMesh);
        normalsDeformeFrom = testMesh.getNormals();
    }
}
void shape::blenshapeDeform(float strength) {
	if (blendshapeLoaded) {
		for (int i = 0; i < testMesh.getNumVertices(); i++) {

			ofVec3f deformedPos(vertsBasePos[i] * (1 - strength) + blendshapeMesh.getVertex(i) * strength);
			testMesh.setVertex(i, deformedPos);
		}
		setNormals(testMesh);
	}

}
void shape::setTexture(ofImage texImg){
    img = texImg;
	//img.mirror(false, true);
}
ofImage shape::getTexture() {
	return img;
}
void shape::setVertColor(){
    for(int i = 0; i < testMesh.getNumVertices(); i++){
        testMesh.setColor(i,ofFloatColor(1-vertMap[i],0,0));
    }
}

void shape::blendImg(ofImage img1, ofImage img2, ofImage mask) {
	//int pixels = (img.getHeight()*img.getWidth())
	//ofColor green(0, 255, 0);
	//img.loadImage("img_02.jpg");
	ofPixelsRef pixels = img1.getPixelsRef();
	cout << mask.getWidth() << endl;
	for (int i = 0; i < pixels.size(); i++) {

		pixels.setColor(i, img1.getColor(i)*.3 + img2.getColor(i)*.7);
		//pixels.setColor(i, pixels.getColor(i)*mask.getColor(i).getBrightness() + img2.getColor(i)*(1.0f - mask.getColor(i).getBrightness()));
	}


	img.setFromPixels(pixels);
}
void shape::loopTexture(int xOffset,int yOffset,float scale, bool blend){

    //int offset = 10;


	//grabbedImg.crop((grabbedImg.getWidth() - img.getWidth()) / 2, );
	/*if (blend) {
		grabbedImg.grabScreen(0 + xOffset + scale, 0 + yOffset + scale, ofGetWidth() - scale * 2, ofGetHeight() - scale * 2);
		//grabbedImg.crop(60, 60, 1000, 1000);
		
		blendImg(img, grabbedImg, displaceImg);
	}
	else {*/
	img.grabScreen(0 + xOffset + scale, 0 + yOffset + scale, ofGetWidth() - scale * 2, ofGetHeight() - scale * 2);
		//img.grabScreen(ofGetWidth() / 2 - scale, ofGetHeight() / 2 - scale, ofGetWidth() / 2 + scale, ofGetHeight() / 2 + scale);
	//}
	//img = grabbedImg;
	
	//img.resize(resizeX, resizeY);
}
void shape::show(){

    setNormals(testMesh);
    //solidShading();
	if (drawNormalsSet) {
		drawNormals();
	}
    //drawNormals(true);

    //ofTranslate(-20,0,0);
    //img.getTextureReference().bind();

    //ofSetColor( 0, 128, 0 );
    //mapPlannar();
//    ofColor green(0, 255, 0);
//    img.setColor(green);

//    img.loadImage("img_01.png");
//    changeColor();
    //img.rotate90(5);
    img.bind();

    testMesh.draw();

    img.unbind();
    

	if (drawWireframeSet) {
		testMesh.drawWireframe();
	}
    //img.
    //setVertColor();

}
//--------------------------------------------------------------
void shape::draw(){

}
