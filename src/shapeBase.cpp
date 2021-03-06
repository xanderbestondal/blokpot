#include "shapeBase.h"
#include <iostream>

#include <GL/gl.h>
#include <map>


//--------------------------------------------------------------
void shapeBase::setup() {

	//loadObj("data/mappingfiles/3D.obj");
	//cout << "STAAAAAAAAAAAAAAAAART" << endl;
	blendshapeLoaded = false;
	drawWireframeSet = false;
	drawNormalsSet = false;
}

void shapeBase::resetMesh() {

	mesh.clear();
	vertsBasePos.resize(0);
	vertsDeformedPos.resize(0);
	vertsDeformeFrom.resize(0);
}

void shapeBase::setNormals( ofMesh &mesh ){
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
void shapeBase::makeTri(int vert1,int vert2,int vert3){

    mesh.addIndex(vert1);
    mesh.addIndex(vert2);
    mesh.addIndex(vert3);
    vertsTris[vert1].push_back(tris);
    vertsTris[vert2].push_back(tris);
    vertsTris[vert3].push_back(tris);
//solidShading();
    tris += 1;
// TEMP NORMALS
    mesh.addNormal(ofPoint(0,0,0));
}
void shapeBase::solidShading(){

//    remove all verts and tris
//    rebuild like this:
//    vertsTemp = testMesh.getVertices();
    vector<ofVec3f> vertsTemp;

    for(int i = 0; i < mesh.getNumIndices(); i+=3){
        vertsTemp.push_back(mesh.getVertex(mesh.getIndex(i)));
        vertsTemp.push_back(mesh.getVertex(mesh.getIndex(i+1)));
        vertsTemp.push_back(mesh.getVertex(mesh.getIndex(i+2)));
    }
    mesh.clearVertices();
    mesh.clearIndices();
    for(int i = 0; i < vertsTemp.size(); i++){
        mesh.addVertex(vertsTemp[i]);
    }
    mesh.setupIndicesAuto();
}
ofVec3f shapeBase::makeTriNormal(ofVec3f p1,ofVec3f p2,ofVec3f p3){
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
void shapeBase::drawNormals(bool faces){

    float len = 10;
	ofPushStyle();
    if(faces){
        for(int i = 0; i < polyNormals.size(); i++){
            ofPoint mid = (mesh.getVertex(mesh.getIndex(i*3))+mesh.getVertex(mesh.getIndex(i*3+1))+mesh.getVertex(mesh.getIndex(i*3+2)))/3;
            ofSetColor(0,0,255);
            ofLine(mid.x,mid.y,mid.z,mid.x+polyNormals[i].x*len,mid.y+polyNormals[i].y*len,mid.z+polyNormals[i].z*len);
        }
    }else{
        for(int i = 0; i < vertNormals.size(); i++){
            ofSetColor(255,255,255);
            ofLine(mesh.getVertex(i).x,mesh.getVertex(i).y,mesh.getVertex(i).z,mesh.getVertex(i).x+mesh.getNormal(i).x*len,mesh.getVertex(i).y+mesh.getNormal(i).y*len,mesh.getVertex(i).z+mesh.getNormal(i).z*len);
        }
    }
	ofPopStyle();
}

void shapeBase::storeBoundingBox() {

	float Xmin = 0;
	float Xmax = 0;
	float Ymin = 0;
	float Ymax = 0;
	float Zmin = 0;
	float Zmax = 0;
	for (int i = 0; i < mesh.getNumVertices(); i++) {

		if (mesh.getVertex(i).x < Xmin)
			Xmin = mesh.getVertex(i).x;
		if (mesh.getVertex(i).x > Xmax)
			Xmax = mesh.getVertex(i).x;
	}

}
void shapeBase::vertPosToRgb() {
	//vertMap.resize(0);
	int width = 10;
	int Xpos = 0;
	int Ypos = 0;
	int drawWidth = sqrt(mesh.getNumVertices());
	for (int i = 0; i < mesh.getNumVertices(); i++) {
		ofPushStyle;
		ofFill();
		ofSetColor(mesh.getVertex(i).x , mesh.getVertex(i).y , mesh.getVertex(i).z);
		ofRect(Xpos, Ypos, 0, width,width);
		ofPopStyle;
			
		Xpos += width;
		if (i % drawWidth == 0) {
			Ypos += width;
			Xpos = 0;
		}
	}
}
void shapeBase::setTexture(ofImage texImg){
    img = texImg;
	//img.mirror(false, true);
}
ofImage shapeBase::getTexture() {
	return img;
}

void shapeBase::blendImg(ofImage img1, ofImage img2, ofImage mask) {
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
void shapeBase::loopTexture(int xOffset,int yOffset,float scale, bool blend){

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
void shapeBase::show(){

    setNormals(mesh);
    //solidShading();
	//if (drawNormalsSet) {
	//	drawNormals();
	//}
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

    mesh.draw();

    img.unbind();
    

	if (drawWireframeSet) {
		mesh.drawWireframe();
	}
    //img.
    //setVertColor();

}
//--------------------------------------------------------------
void shapeBase::draw(){

}
