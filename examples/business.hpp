/*
 * This business logic is a port of the ffvideo.cpp
 * example you can find in fastflow project, under
 * "examples/OpenCVVideo/" directory
 *
 */

#ifndef videofilter_business_hpp
#define videofilter_business_hpp

#include "aux/wrappers.hpp"
#include "aux/aux.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

// file name of video source
const std::string filename = "prova.mp4";
// shows the video output, or not
const bool showvideo  = true;

class gen : public source<Mat> {
public:
    vsource() : vsource("prova.mp4") {}

    vsource(const std::string& filename) : capture(filename) {
        if (!capture.isOpened()) {
            std::cerr << "error: no video open" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    bool has_next() {
        frame = new Mat();
        return capture.read(*frame);
    }

    // ALWAYS call has_next() method before this
    Mat* next() {
        return frame;
    }

protected:
    VideoCapture capture;   // class for video capturing
    Mat *frame;             // frame pointer for video frames
};

class stop : public drain<Mat> {
public:
    void process(Mat* frame) {
        if (showvideo) {
            namedWindow("Filtered video", WINDOW_NORMAL);
            imshow("Filtered video", *frame);
            waitKey(30);    // accomodate visualization
        }
         delete frame;
    }
};

class stage1 : public seq_wrapper<Mat, Mat> {
public:
    Mat compute(Mat& in_frame) {
        Mat out_frame;
        GaussianBlur(in_frame, out_frame, Size(0, 0), 3);
        addWeighted(in_frame, 1.5, out_frame, -0.5, 0, in_frame);
        return out_frame;
    }
};

class stage2 : public seq_wrapper<Mat, Mat> {
public:
    Mat compute(Mat& in_frame) {
        Mat out_frame;
        Sobel(in_frame, out_frame, -1, 1, 0, 3 );
        return out_frame;
    }
};


#endif
