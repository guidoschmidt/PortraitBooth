#pragma once

#include "Painter.h"
#include "ofxPoissonDiskSampling.h"

class PointillismPainter : public Painter
{
private:
	float _density;
	std::vector<ofVec2f> _samples;
	ofImage _image_Color;

public:
	void init();
	void paint();

	inline void setColorImage(ofImage img) { _image_Color = img;  }
};

