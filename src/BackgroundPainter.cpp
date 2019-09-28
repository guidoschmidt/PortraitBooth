#include "BackgroundPainter.hpp"

BackgroundPainter::BackgroundPainter() {
  _center = glm::vec2(ofGetWidth() / 2, ofGetHeight() / 2);
  // Background colors
  _backgroundTo.setHex(0x0a3948, 255);
  _backgroundFrom = _backgroundTo;
  float brightness = _backgroundFrom.getBrightness();
  _backgroundFrom.setBrightness(brightness / 1.5);
  // Background strokes
  int w = ofGetWidth();
  int h = ofGetHeight();
  for (int x = 0; x < w; x += 10) {
    for (int y = 0; y < h; y += 10) {
      glm::vec2 offset = glm::vec2(ofRandom(-10, 10), ofRandom(-10, 10));
      glm::vec2 position(x, y);
      position += offset;
      glm::vec2 size(ofRandom(-120, 120), ofRandom(-600, 600));
      _backgroundStrokes.push_back(glm::vec4(position, size));
      _backgroundStrokeAngles.push_back(ofRandom(-9, 9));
    }
  }

  // Shuffle strokes
  auto rng = std::default_random_engine {};
  std::shuffle(std::begin(_backgroundStrokes), std::end(_backgroundStrokes), rng);
}

void BackgroundPainter::selectBackgroundColors() {
  int backgroundIndex = ofRandom(0, BackgroundColorOptions.size());
  int hexColorSelection = BackgroundColorOptions[backgroundIndex];
  _backgroundFrom.setHex(hexColorSelection, 255);
  float brightness = _backgroundFrom.getBrightness();
  _backgroundTo.setHex(hexColorSelection, 100);
  _backgroundTo.setBrightness(brightness / 1.5);
}

void BackgroundPainter::paint() {
  // Paint: gradient
  ofBackgroundGradient(_backgroundTo, _backgroundFrom, OF_GRADIENT_CIRCULAR);
  // Paint: strokes
  ofSetRectMode(ofRectMode::OF_RECTMODE_CENTER);
  for (int i = 0; i < _backgroundStrokes.size(); i++) {
    glm::vec4 rectV = _backgroundStrokes[i];
    float angle = _backgroundStrokeAngles[i];
    glm::vec2 pos = glm::vec2(rectV.x, rectV.y);
    float perl = glm::perlin(glm::vec2(pos.x / ofGetWidth(), pos.y / ofGetHeight()));
    float distanceToBorder = glm::distance(pos, _center);
    distanceToBorder = 1 - (distanceToBorder / ofGetHeight() / 2);
    ofColor c = _backgroundFrom;
    c.setBrightness(c.getBrightness() + (rectV.z / 120 * 3));
    ofPushMatrix();
    ofSetColor(c);
    ofTranslate(pos);
    ofRotateDeg(angle);
    if (perl < 0.0) {
      ofDrawRectangle(glm::vec2(0, 0), rectV.z, rectV.w);
    } else {
      ofDrawCircle(glm::vec2(0, 0), rectV.z);
    }
    ofPopMatrix();
  }
}
