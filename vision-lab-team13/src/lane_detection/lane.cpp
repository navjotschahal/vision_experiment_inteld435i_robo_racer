#include <opencv2/opencv.hpp>
#include <iostream>

void detectAndDrawLaneMarkers(const cv::Mat& inputImage, cv::Mat& outputImage) {
    // Convert the input image to HSV color space
    cv::Mat hsvImage;
    cv::cvtColor(inputImage, hsvImage, cv::COLOR_BGR2HSV);

    // Define the HSV range for yellow color (from Python code)
    cv::Scalar lowerYellow(20, 65, 100);  // Adjusted lower bound
    cv::Scalar upperYellow(30, 255, 255); // Adjusted upper bound

    // Apply Gaussian blur to reduce noise
    // cv::Mat blurredImage;
    // cv::GaussianBlur(hsvImage, blurredImage, cv::Size(5, 5), 0);

    // Threshold the HSV image to get only yellow colors
    cv::Mat mask;
    cv::inRange(hsvImage, lowerYellow, upperYellow, mask);

    // Perform morphological operations to clean up the mask
    // cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7)); // Larger kernel size
    // cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel);
    // cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);

    // Debug: Show the mask
    cv::imshow("Yellow Mask", mask);

    // Find contours in the mask
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Draw contours on the output image
    outputImage = inputImage.clone();
    for (const auto& contour : contours) {
        // Directly draw the contour without approximation
        cv::drawContours(outputImage, std::vector<std::vector<cv::Point>>{contour}, -1, cv::Scalar(0, 255, 0), 2);
    }

    // Debug: Show filtered contours
    cv::imshow("Filtered Contours", outputImage);
}

int main() {
    // Load the input image
    std::string imagePath = "../../resource/lane.png"; // Update the path to your image
    cv::Mat inputImage = cv::imread(imagePath);

    if (inputImage.empty()) {
        std::cerr << "Error: Could not load image at " << imagePath << std::endl;
        return -1;
    }

    // Detect and draw lane markers
    cv::Mat outputImage;
    detectAndDrawLaneMarkers(inputImage, outputImage);

    // Display the results
    cv::imshow("Input Image", inputImage);
    cv::imshow("Lane Detection", outputImage);

    // Wait for a key press and save the output image
    cv::waitKey(0);
    cv::imwrite("../../resource/lane_detection_result.png", outputImage);

    return 0;
}