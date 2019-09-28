#pragma once

#include "ofMain.h"
#include "FaceDescriptor.hpp"
#include "Paintstroke.hpp"

class FacePainter_Drawing {
private:
  FaceDescriptor* _faceDescriptor;

  // Strokes
  Paintstroke _paintstrokeChin;
  Paintstroke _paintstrokeNoseV;
  Paintstroke _paintstrokeNoseH;
  Paintstroke _paintstrokeMouth;
  Paintstroke _paintstrokeEyebrowL;
  Paintstroke _paintstrokeEyebrowR;
  Paintstroke _paintstrokeEyeL;
  Paintstroke _paintstrokeEyeR;

  // Contours
  // std::vector<Paintstroke> _contours;

  // Centers
  glm::vec2 _eyeStrokesCenterL;
  glm::vec2 _eyeStrokesCenterR;
  glm::vec2 _noseStrokeCenter;

  // Methods: Paint preparation
  void prepareEyes();
  void prepareNose();
  void prepareMouth();
  void prepareChin();
  void prepareEyebrows();

  // Methods: Painting
  void paintEyes();
  void paintNose();
  void paintMouth();
  void paintChin();
  void paintEyebrows();

public:
  FacePainter_Drawing();

  void init();
  void paint();

  inline void setDescriptor(FaceDescriptor* fd) { _faceDescriptor = fd; }
};
