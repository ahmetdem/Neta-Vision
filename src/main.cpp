#include "../include/Recognizer.h"
#include <opencv2/core/mat.hpp>
#include <opencv2/opencv.hpp>

int main() {
  // Create a VideoCapture object to capture from camera
  cv::VideoCapture cap(0);

  // Check if the camera opened successfully
  if (!cap.isOpened()) {
    std::cerr << "Error: Unable to open camera" << std::endl;
    return -1;
  }

  Color targetColor = Color::RED;
  int tolerance = 30;
  ColorRecognizer colorRecognizer(targetColor, tolerance);

  while (true) {
    // Capture frame-by-frame
    cv::Mat frame;
    cap >> frame;

    // Check if the frame is empty (end of video)
    if (frame.empty()) {
      std::cerr << "Error: Unable to capture frame" << std::endl;
      break;
    }

    cv::Mat output;
    colorRecognizer.processFrame(frame, output);

    // Display the captured frame
    cv::imshow("Camera", frame);

    // Check for key press to exit
    if (cv::waitKey(1) == 27) { // Press ESC to exit
      break;
    }
  }

  // Release the VideoCapture object and close windows
  cap.release();
  cv::destroyAllWindows();

  return 0;
}
