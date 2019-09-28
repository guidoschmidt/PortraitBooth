#include "PointillismPainter.h"

void PointillismPainter::init() {
	ofEnableAntiAliasing();
	ofEnableSmoothing();
	float w = _image_Color.getWidth();
	float h = _image_Color.getHeight();
	_density = min<float>(w, h) * 0.0015;
	_samples = ofxPoissonDiskSampling::sample2D(w, h, _density, false);
	ofSetCircleResolution(100);
}

void PointillismPainter::paint() {
	ofBackground(250);
	ofFill();
	ofColor dark = ofColor::fromHsb(0, 0, 0);
	for (ofVec2f v : _samples) {
		int x = v.x;
		int y = v.y;
		if (x > 0 && x < _image_Color.getWidth() &&
			y > 0 && y < _image_Color.getHeight()) {
			ofColor color = _image_Color.getColor(x, y);
			float r = 0.05 + (255 - color.getLightness()) / 255;
			float distance = glm::distance(glm::vec2(v.x, v.y), _faceDescriptor->getCenter());
			float relativeDistance = 1.0 - distance / _image_Color.getHeight();
			r *= relativeDistance;
			color.lerp(dark, relativeDistance);
			ofSetColor(color.getLightness());
			ofDrawCircle(v.x, v.y, r);
		}
	}
}
