#pragma once

#include "ofMain.h"
#include "FaceDescriptor.hpp"
#include "Paintstroke.hpp"

class FacePainter {
private:
  FaceDescriptor* _faceDescriptor;

  // Strokes
  Paintstroke _paintstrokeChin;
  Paintstroke _paintstrokeNoseV;
  Paintstroke _paintstrokeNoseH;
  Paintstroke _paintstrokeMouth;
  Paintstroke _paintstrokeEyebrowL;
  Paintstroke _paintstrokeEyebrowR;

  // Images
  ofImage _img_Color;
  ofImage _img_BW_a;
  ofImage _img_BW_b;
  // ofImage _img_OpticalFlow;

  // Contours
  // std::vector<Paintstroke> _contours;

  // Strokes
   std::vector<std::tuple<glm::vec3, ofColor>> _eyeStrokesL;
   std::vector<std::tuple<glm::vec3, ofColor>> _eyeStrokesR;
   std::vector<std::tuple<glm::vec3, ofColor>> _noseStrokes;
   std::vector<std::tuple<Paintstroke, ofColor>> _faceStrokes;
  // std::vector<std::tuple<glm::vec3, ofColor>> _mouthStrokes;

  // Centers
  glm::vec2 _eyeStrokesCenterL;
  glm::vec2 _eyeStrokesCenterR;
  glm::vec2 _noseStrokeCenter;

  // Methods: Paint preparation
  // void prepareStrokesFor(Region r);
  // void prepareStrokes();
  void prepareEyes();
  void prepareNose();
  void prepareMouth();
  void prepareChin();
  void prepareEyebrows();

  // Methods: Painting
  // void paintEyebrows();
  void paintEyes();
  void paintNose();
  void paintMouth();
  void paintChin();
  void paintEyebrows();

  void paintEyeArea(std::vector<std::tuple<glm::vec3, ofColor>> strokes, glm::vec2 center);

public:
  FacePainter();

  void init();
  void paint();

  inline void setDescriptor(FaceDescriptor* fd) { _faceDescriptor = fd; }
  inline void setColorImage(ofImage img) { _img_Color = img;  }
};
