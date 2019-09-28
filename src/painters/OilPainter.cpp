#include "OilPainter.h"

void OilPainter::init() {
	ofEnableAntiAliasing();
	ofEnableSmoothing();
	float w = _image_Color.getWidth();
	float h = _image_Color.getHeight();
	_density = min<float>(w, h) * 0.002;
	_samples = ofxPoissonDiskSampling::sample2D(w, h, _density, false);
	_backgroundPainter.selectBackgroundColors();
	_perlinFactor = 12.0f;
}

void OilPainter::paint() {
	_backgroundPainter.paint();
	ofFill();
	for (ofVec2f v : _samples) {
		int x = v.x;
		int y = v.y;
		if (x > 0 && x < _image_Color.getWidth() &&
			y > 0 && y < _image_Color.getHeight()) {
			ofColor color = _image_Color.getColor(x, y);
			float r = 2 + (255 - color.getLightness()) / 255;
			float distance = glm::distance(glm::vec2(v.x, v.y), _faceDescriptor->getCenter());
			float relativeDistance = 1.0 - (distance / _image_Color.getHeight());
			float relativeDistanceInv = (distance / _image_Color.getHeight());
			relativeDistance = glm::pow(relativeDistance, 2);
			relativeDistanceInv = glm::pow(relativeDistanceInv, 2);
			color.setSaturation((color.getSaturation() * 3) * relativeDistance);
			ofSetColor(color);
			float perlAngle = glm::perlin(glm::vec2(x / (float)ofGetWidth() * _perlinFactor, 
													y / (float)ofGetHeight() * _perlinFactor));
			ofPushMatrix();
			ofTranslate(x, y);
			ofRotateDeg(perlAngle * 360.0);
			ofDrawRectangle(glm::vec2(0, 0), 5 + r * relativeDistanceInv * 2, 10 + r * 60 * relativeDistanceInv);
			ofPopMatrix();
		}
	}
}
