#include "../include/Recognizer.h"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>

ColorRecognizer::ColorRecognizer(const Color &color)
    : targetColor(color) {}

ColorRecognizer::~ColorRecognizer() { return; }

void ColorRecognizer::init(const Color &color) {
  this->targetColor = color;
}

void ColorRecognizer::getMatFromColor(const Color &color, cv::Mat &hsvFrame,
                                      cv::Mat &mask) {
  cv::Scalar l1, l2, u1, u2;

  // Add new Cases for Adding new Colors.
  switch (color) {
  case Color::RED:
    l1 = cv::Scalar(0, 70, 50);
    u1 = cv::Scalar(10, 255, 255);
    l2 = cv::Scalar(170, 70, 50);
    u2 = cv::Scalar(180, 255, 255);
    break;
  default:
    break;
  }

  cv::Mat mask1, mask2;
  cv::inRange(hsvFrame, l1, u1, mask1);
  cv::inRange(hsvFrame, l2, u2, mask2);

  cv::bitwise_or(mask1, mask2, mask);
}

void ColorRecognizer::processFrame(const cv::Mat &frame, cv::Mat &output) {

  cv::Mat hsvFrame;
  this->convertToHSV(frame, hsvFrame);

  cv::Mat mask;
  this->applyThreshold(hsvFrame, mask);

  cv::Point colorCenter = this->findColorCenter(mask);
  std::cout << "Color region centroid found at (" << colorCenter.x << ", "
            << colorCenter.y << ")." << std::endl;

  output = frame.clone();

  if (colorCenter.x != -1 && colorCenter.y != -1) {
    cv::circle(output, colorCenter, 5, cv::Scalar(0, 0, 255), -1);
  } else {
    std::cout << "Color not found in the frame." << std::endl;
  }

  cv::imshow("Output with Circle", output);
  cv::imshow("MASK", mask);
}

double ColorRecognizer::getDistanceToCenter() const { return 0.0; }
