
#include "shapeBase.h"
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"


class shape : public ofBaseApp {
	public:
		void setup();

		shapeBase object;

		ofxAssimpModelLoader loadedModel;
		ofMesh loadObj(string filename);
		ofMesh loadObjBlendshape(const char* filename);

		void circle(int steps, float radius, ofVec3f pos);
		ofMesh cylinder(int s, int heightSegments, float height, float radius, bool sphere, bool cone);
		ofMesh plane(int widthSegments, int heightSegments, float width, float height);

		void show(ofPoint translate, ofVec3f rotate = ofVec3f(0,0,0));

};

