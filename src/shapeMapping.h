
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"

#include "shapeBase.h"

class shapeMapping : public ofBaseApp {
	public:
		void setMeshBase(shapeBase* shp);

		ofMesh* mesh;
		shapeBase* meshBase;

		vector<float> vertMap;
		ofImage displaceImg;

		void mapPlannar(ofCamera cam, bool mirrorX = false);
		void mapByLight(ofVec3f point);

		void noiseMap(float frequency, float offset, ofVec3f scale, float yPos);// , float zPos);
		void displaceMap(ofImage dispImg, float offset = 0);
		void gradientMap();

		void setVertColor();
};

