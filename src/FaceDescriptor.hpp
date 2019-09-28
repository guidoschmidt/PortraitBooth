#pragma once

#include "ofMain.h"

const int MarkerCountPerFace = 68;
const enum Region {
  CHIN,
  EYEBROW_L,
  EYEBROW_R,
  NOSE_V,
  NOSE_H,
  EYE_L,
  EYE_R,
  MOUTH
};

class FaceDescriptor {
private:
  std::vector<glm::vec2> _markers;
  glm::vec2              _center;

public:
  FaceDescriptor();

  std::tuple<int, int> getRangeForRegion(Region r);
  void calculateCenter();
  void drawDebug();

  inline void addMarkerPoint(glm::vec2 p) { _markers.push_back(p); }
  inline std::vector<glm::vec2>& getMarkers() { return _markers; }
  inline glm::vec2 getCenter() { return _center; }
  inline glm::vec2 getBottom() { return _markers[8]; }
  inline float getWidth() { return glm::abs(_markers[0].x - _markers[16].x); }
  glm::vec2 getNoseCenter();
};
