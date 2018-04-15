#include "shapeMapping.h"

void shapeMapping::setMeshBase(shapeBase* shp) {
	meshBase = shp;
	mesh = &meshBase->mesh;
}

void shapeMapping::mapPlannar(ofCamera cam, bool mirrorX) {

	(*mesh).clearTexCoords();
	for (int i = 0; i < (*mesh).getNumVertices(); i++) {
		//vertMap.push_back( ofNoise(vertsBasePos[i].x*frequency*scale[0],vertsBasePos[i].y*frequency*scale[1],vertsBasePos[i].z*frequency*scale[2]));

		ofVec2f uv = cam.worldToScreen((*meshBase).vertsDeformedPos[i]);
		if (mirrorX) {
			uv = ofPoint(1 - uv.x / (ofGetWidth()), uv.y / (ofGetHeight()));
		}
		else {
			uv = ofPoint(uv.x / (ofGetWidth()), uv.y / (ofGetHeight()));
		}

		if (uv.x < 0) { uv.x = .01; }
		if (uv.x > 1) { uv.x = .99; }
		if (uv.y < 0) { uv.y = .01; }
		if (uv.y > 1) { uv.y = .99; }

		(*mesh).addTexCoord(uv);
	}
}


void shapeMapping::mapByLight(ofVec3f point) {
	for (int i = 0; i < (*meshBase).polyNormals.size(); i++) {
		ofPoint mid = ((*mesh).getVertex((*mesh).getIndex(i * 3)) + (*mesh).getVertex((*mesh).getIndex(i * 3 + 1)) + (*mesh).getVertex((*mesh).getIndex(i * 3 + 2))) / 3;
		ofPoint lightDir(1, 0, 0);
		float dot((((*meshBase).polyNormals[i].dot(lightDir)) + 1) / 2);
		for (int t = 0; t < 3; t++) {
			(*mesh).setTexCoord((*mesh).getIndex(i * 3 + t), ofPoint(dot, ofRandom(1)));
		}
	}
}
void shapeMapping::noiseMap(float frequency = 1, float offset = 0, ofVec3f scale = ofVec3f(1,1,1), float yPos=0) {//, float zPos){
	vertMap.resize(0);
	for (int i = 0; i < (*mesh).getNumVertices(); i++) {
		vertMap.push_back(ofNoise((*meshBase).vertsBasePos[i].x*frequency*scale[0], (*meshBase).vertsBasePos[i].y*frequency*scale[1] + yPos, (*meshBase).vertsBasePos[i].z*frequency*scale[2]) + offset);
	}
}
void shapeMapping::displaceMap(ofImage dispImg, float offset) {
	vertMap.resize(0);
	displaceImg = dispImg;
	for (int i = 0; i < (*mesh).getNumVertices(); i++) {
		ofVec2f uv = (*mesh).getTexCoord(i);
		ofColor col = dispImg.getColor(uv.x*dispImg.getWidth(), uv.y*dispImg.getHeight());
		vertMap.push_back(col.getBrightness() / 255.0f + offset);
	}
}
void shapeMapping::gradientMap() {
	vertMap.resize(0);
	float height = 500;
	for (int i = 0; i < (*mesh).getNumVertices(); i++) {
		vertMap.push_back((*meshBase).vertsBasePos[i].y / height);
	}
}
void shapeMapping::setVertColor() {
	for (int i = 0; i < (*mesh).getNumVertices(); i++) {
		(*mesh).setColor(i, ofFloatColor(1 - vertMap[i], 0, 0));
	}
}