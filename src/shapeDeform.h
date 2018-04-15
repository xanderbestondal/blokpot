
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"

#include "shapeBase.h"

class shapeDeform : public ofBaseApp {
	public:
		void setMeshBase(shapeBase* shp);
		void setMap(vector<float> map);

		shapeBase* meshBase;
		ofMesh* mesh;

		int iterations;
		//int deformStrength;
		float deformMultiplier = 10;

		vector<ofVec3f> vertsBasePos; // used to be able to alwys go back to the original shape
		vector<ofVec3f> vertsDeformedPos;
		vector<ofVec3f> vertsDeformeFrom;// used for multiple iterations of displacement

		vector<ofVec3f> vertsBlendshapePos;

		vector<ofVec3f> vertsBaseNormals;
		vector<ofVec3f> normalsDeformeFrom;

		vector<float> vertMap;

		ofMesh blendshapeMesh;
		bool blendshapeLoaded;

		ofMesh solidDeform(int iterations);
		ofMesh deform(int iterations, float Yfavor);
		void blenshapeDeform(float strength);

};

