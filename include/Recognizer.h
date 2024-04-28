#ifndef COLOR_RECOGNIZER_H
#define COLOR_RECOGNIZER_H

#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/opencv.hpp>

enum Color {
  RED,
  BLUE,
  YELLOW,
};

class ColorRecognizer {
public:
  /**
   * @brief Constructor for the ColorRecognizer class.
   *
   * Initializes the ColorRecognizer object with specified parameters.
   *
   * @param color The color to recognize (in BGR format).
   * @param tolerance The tolerance level for color matching.
   */
  ColorRecognizer(const Color &color);

  /**
   * @brief Destructor for the ColorRecognizer class.
   *
   * Frees any resources allocated by the ColorRecognizer object.
   */
  ~ColorRecognizer();

  /**
   * @brief Generates a binary mask based on the specified color range.
   *
   * This function generates a binary mask highlighting regions in an HSV frame
   * that match the specified color range.
   *
   * @param color The target color to generate the mask for.
   * @param hsvFrame The input HSV frame.
   * @param mask The output binary mask highlighting regions matching the target
   * color.
   */
  void getMatFromColor(const Color &color, cv::Mat &hsvFrame, cv::Mat &mask);

  /**
   * @brief Initialize the color recognizer with the specified parameters.
   *
   * @param color The color to recognize (in BGR format).
   * @param tolerance The tolerance level for color matching.
   */
  void init(const Color &color);

  /**
   * @brief Process the input frame and identify regions matching the specified
   * color.
   *
   * @param frame The input frame in which to search for the specified color.
   * @param[out] output The output frame highlighting regions matching the
   * specified color.
   */
  void processFrame(const cv::Mat &frame, cv::Mat &output);

  /**
   * @brief Calculates the distance between the center of the screen and the
   * center of the region matching the target color.
   *
   * @return The distance between the center of the screen and the center of the
   * region matching the target color.
   */
  double getDistanceToCenter() const;

private:
  Color targetColor;

  /**
   * @brief Convert the input frame to the HSV color space.
   *
   * @param frame The input frame to convert.
   * @param[out] hsvFrame The output frame in the HSV color space.
   */
  void convertToHSV(const cv::Mat &frame, cv::Mat &hsvFrame) {

    // Check if the input frame has the correct number of channels (3)
    if (frame.channels() != 3) {
      CV_Error(cv::Error::StsBadArg, "Input frame must have 3 channels (BGR)");
    }

    // Convert the BGR frame to HSV color space
    cv::cvtColor(frame, hsvFrame, cv::COLOR_BGR2HSV);
  }

  /**
   * @brief Apply color thresholding to identify regions matching the target
   * color.
   *
   * @param hsvFrame The input frame in the HSV color space.
   * @param[out] mask The binary mask highlighting regions matching the target
   * color.
   */
  void applyThreshold(cv::Mat &hsvFrame, cv::Mat &mask) {
    this->getMatFromColor(this->targetColor, hsvFrame, mask);
  }

  /**
   * @brief finds the centroid of the region matching the target color.
   *
   * @param mask the binary mask highlighting regions matching the target color.
   * @return the centroid of the region matching the target color.
   */
  cv::Point findColorCenter(const cv::Mat &mask) {
    // Find contours in the binary mask
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);

    // If no contours are found, return (-1, -1) indicating no center
    if (contours.empty()) {
      return cv::Point(-1, -1);
    }

    // Find the largest contour (assuming it represents the main color region)
    double maxArea = 0;
    int maxAreaIdx = -1;
    for (size_t i = 0; i < contours.size(); ++i) {
      double area = cv::contourArea(contours[i]);
      if (area > maxArea) {
        maxArea = area;
        maxAreaIdx = static_cast<int>(i);
      }
    }

    // Calculate the moments only if a valid contour was found
    if (maxAreaIdx != -1) {
      cv::Moments moments = cv::moments(contours[maxAreaIdx]);
      if (moments.m00 != 0) {
        return cv::Point(moments.m10 / moments.m00, moments.m01 / moments.m00);
      }
    }

    // Return (-1, -1) if no valid centroid can be calculated
    return cv::Point(-1, -1);
  }
};

#endif // COLOR_RECOGNIZER_H
