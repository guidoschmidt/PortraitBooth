#pragma once

#include "Painter.h"
#include "ofxPoissonDiskSampling.h"
#include "BackgroundPainter.hpp"

class OilPainter : public Painter
{
private:
	ofImage _image_Color;
	float _density;
	std::vector<ofVec2f> _samples;
	BackgroundPainter _backgroundPainter;
	float _perlinFactor;

public:
	void init();
	void paint();

	inline void setColorImage(ofImage img) { _image_Color = img;  }
};

