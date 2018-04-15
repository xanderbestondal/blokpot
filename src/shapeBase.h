#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"

class shapeBase : public ofBaseApp{

	public:
		void setup();
		void draw();
		shapeBase() {

			drawNormalsSet = false;
		}

        ofMesh mesh;
		ofMesh blendshapeMesh;
		bool blendshapeLoaded;

		ofImage img;
		ofImage grabbedImg;
        ofTextureData texData;


        int tris;


        vector<ofVec3f> vertsBasePos; // used to be able to alwys go back to the original shape
        vector<ofVec3f> vertsDeformedPos;
        vector<ofVec3f> vertsDeformeFrom;// used for multiple iterations of displacement


        vector<ofVec3f> vertsBaseNormals;
        vector<ofVec3f> normalsDeformeFrom;

        vector<ofVec3f> polyNormals;
        vector<ofVec3f> vertNormals;

        vector<vector<int> > vertsTris;


        void makeTri(int vert1,int vert2,int vert3);
        void show();

		void resetMesh();

        void setNormals( ofMesh &mesh );
        void solidShading();

        ofVec3f makeTriNormal(ofVec3f p1,ofVec3f p2,ofVec3f p3);

        ofCamera cam;

		void blendImg(ofImage img1, ofImage img2, ofImage mask = ofImage());

        void drawNormals(bool faces=false);
		bool drawNormalsSet;
		bool drawWireframeSet;

		void storeBoundingBox();
		void vertPosToRgb();

        void loopTexture(int xOffset=1,int yOffset=1,float scale=1,bool blend=false);
		void setTexture(ofImage texImg);
		ofImage getTexture();

};
