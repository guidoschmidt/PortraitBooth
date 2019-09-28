#pragma once

#include <algorithm>
#include <random>
#include <vector>
#include "ofMain.h"

class BackgroundPainter {
private:
  std::vector<int> BackgroundColorOptions = {
      0x0a3948, 0x8c98c0, 0xc8d8e0, 0xf5c7c0, 0xe69e72, 0xa9572d,
      0xd5993e, 0xab8757, 0x7c6e6d, 0x736d5d, 0xdbdbd3
  };
  glm::vec2 _center;
  ofColor _backgroundFrom;
  ofColor _backgroundTo;
  std::vector<glm::vec4> _backgroundStrokes;
  std::vector<float> _backgroundStrokeAngles;

public:
  BackgroundPainter();

  void selectBackgroundColors();
  void paint();
  inline void darken() {
	  _backgroundFrom.setBrightness(30);
	  _backgroundTo.setBrightness(10);
  }
};
