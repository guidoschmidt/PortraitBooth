#pragma once

#include <vector>
#include "ofMain.h"

class Paintstroke {
private:
  ofColor _color;
  float _perlinFactor;
  ofPolyline _baseLine;
  std::vector<ofPolyline> _accessoryLines;

  void renderPaint(ofPolyline& polyLine, float width = 2);

public:
  Paintstroke();

  void addPoint(glm::vec2 p);
  void draw(float width = 10.0);

  inline float getArea() { return _baseLine.getArea(); }
  inline glm::vec2 getCenter() { return _baseLine.getCentroid2D(); }
  inline void setColor(ofColor color) { _color = color; }
};
