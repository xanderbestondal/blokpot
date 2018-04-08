#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"

class shapeBase : public ofBaseApp{

	public:
		void setup();
		void draw();
		shapeBase() {

			drawNormalsSet = false;
			deformMultiplier = 2;
		}

        ofMesh mesh;
		ofMesh blendshapeMesh;
		bool blendshapeLoaded;

		ofImage img;
		ofImage grabbedImg;
        ofImage displaceImg;
        ofTextureData texData;


        //int segment_count;
        int steps;
        int segments;
        int tris;
        int iterations;
		//int deformStrength;
		float deformMultiplier;

        vector<ofVec3f> vertsBasePos; // used to be able to alwys go back to the original shape
        vector<ofVec3f> vertsDeformedPos;
        vector<ofVec3f> vertsDeformeFrom;// used for multiple iterations of displacement

		vector<ofVec3f> vertsBlendshapePos;

        vector<ofVec3f> vertsBaseNormals;
        vector<ofVec3f> normalsDeformeFrom;

        vector<ofVec3f> polyNormals;
        vector<ofVec3f> vertNormals;
        vector<float> vertMap;

        vector<vector<int> > vertsTris;

        vector<ofIndexType > originalIndices;// used to store verts before division
        vector<ofVec3f > dividedIndices; // used to store ALL verts after division
        //vector<ofVec3f > dividedIndices; // used to store only new verts after division

        vector<ofVec2f > dividedList; // used to store vert pairs that are divided
        vector<int > dividedVertList; // used together with dividedList to find vert number that was already divided


        void makeTri(int vert1,int vert2,int vert3);
        void show();

		void resetMesh();

        void setNormals( ofMesh &mesh );
        void solidShading();

        ofVec3f makeTriNormal(ofVec3f p1,ofVec3f p2,ofVec3f p3);
        void setVertColor();

        ofCamera cam;

		void noiseMap(float frequency, float offset, ofVec3f scale, float yPos);// , float zPos);
        void displaceMap(ofImage dispImg, float offset = 0);
        void gradientMap();
		void blendImg(ofImage img1, ofImage img2, ofImage mask = ofImage());

        void solidDeform(int iterations);
        void deform(int iterations, float Yfavor);
		void blenshapeDeform(float strength);
        void drawNormals(bool faces=false);
		bool drawNormalsSet;
		bool drawWireframeSet;

		void storeBoundingBox();
		void vertPosToRgb();

        void mapPlannar(ofCamera cam, bool mirrorX = false);
        void mapByLight(ofVec3f point);
        void loopTexture(int xOffset,int yOffset,float scale,bool blend=false);
		void setTexture(ofImage texImg);
		ofImage getTexture();

};
