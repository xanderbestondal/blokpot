
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"

#include "shape.h"

class shapeScatter : public ofBaseApp {
	public:
		void setup();
		void scatter(bool faces = true);
		void setSurface(shape surf);

		shape scatterSurface;
		vector<shape> scatterMeshes;
};

