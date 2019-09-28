#include "FaceDescriptor.hpp"

FaceDescriptor::FaceDescriptor() {
  _markers = {};
  _center = glm::vec2(0, 0);
}

void FaceDescriptor::calculateCenter() {
  if (_markers.size() == 0) return;
  int xSum = 0;
  int ySum = 0;
  for (auto marker : _markers) {
    xSum += marker.x;
    ySum += marker.y;
  }
  int xMean = xSum / _markers.size();
  int yMean = ySum / _markers.size();
  _center = glm::vec2(xMean, yMean);
}

std::tuple<int, int> FaceDescriptor::getRangeForRegion(Region r) {
  switch(r) {
  case Region::CHIN:
    return std::make_tuple(0, 16);
  case Region::EYEBROW_L:
    return std::make_tuple(17, 22);
  case Region::EYEBROW_R:
    return std::make_tuple(22, 27);
  case Region::NOSE_V:
    return std::make_tuple(27, 31);
  case Region::NOSE_H:
    return std::make_tuple(31, 36);
  case Region::EYE_L:
    return std::make_tuple(36, 42);
  case Region::EYE_R:
    return std::make_tuple(42, 48);
  case Region::MOUTH:
    return std::make_tuple(48, 67);
  }
}

void FaceDescriptor::drawDebug() {
  for (auto marker : _markers) {
    ofSetColor(100, 255, 255);
    ofDrawEllipse(marker, 3, 3);
  }
}

glm::vec2 FaceDescriptor::getNoseCenter() {
    glm::vec2 p1 = _markers[39];
    glm::vec2 p2 = _markers[42];
    glm::vec2 p3 = _markers[33];
    int xSumNoseCenter = p1.x + p2.x + p3.x;
    int ySumNoseCenter = p1.y + p2.y + p3.y;
    return glm::vec2(xSumNoseCenter / 3, ySumNoseCenter / 3);
}
