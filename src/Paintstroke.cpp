#include "Paintstroke.hpp"

Paintstroke::Paintstroke() {
  _perlinFactor = 25;
  int accessoryCount = ofRandom(3, 5);
  for (int i = 0; i < accessoryCount; i++) {
    _accessoryLines.push_back(ofPolyline());
  }
}


void Paintstroke::renderPaint(ofPolyline& polyLine, float width) {
  ofSetColor(_color, 212);
  auto vertices = polyLine.getVertices();
  float strokeWidth = width;
  float baseStrokeWidth = 1;
  int count = 3 * polyLine.size();
  ofSetLineWidth(5);
  for (float p = 0; p < count; p += 0.1) {
    glm::vec2 point = polyLine.getPointAtPercent(p / count);
    float floatIndex = polyLine.getIndexAtPercent(p / count);
    float pathWidth = glm::sin((p / count) * glm::pi<float>());
    glm::vec2 normal = polyLine.getNormalAtIndexInterpolated(floatIndex);
    glm::vec2 normalAdj = baseStrokeWidth + normal * strokeWidth * pathWidth;
    ofDrawLine(point - normalAdj / 2, point + normalAdj / 2);
  }
}

void Paintstroke::addPoint(glm::vec2 p) {
  _baseLine.curveTo(p.x, p.y );
  _baseLine.simplify();
  for (int i = 0; i < _accessoryLines.size(); i++) {
    float xO = glm::perlin(
                   glm::vec2(float(_perlinFactor * (p.x / ofGetWidth()) + i),
                             float(_perlinFactor * (p.y / ofGetWidth()) + i))) *
               8.0f;
    float yO = glm::perlin(
                   glm::vec2(float(_perlinFactor * (p.x / ofGetWidth()) + i),
                             float(_perlinFactor * (p.y / ofGetWidth()) + i))) *
               8.0f;
    _accessoryLines[i].curveTo(p.x + xO, p.y + yO);
    _accessoryLines[i].simplify();
  }
}

void Paintstroke::draw(float width) {
  ofSetColor(_color, 255);
  renderPaint(_baseLine, width);
  for (auto pl : _accessoryLines) {
    ofSetColor(_color, 255);
    ofSetLineWidth(1);
    pl.draw();
  }
}


