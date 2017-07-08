#include "aux/wrappers.hpp"
#include "aux/aux.hpp"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <cmath>
#include <string>
#include <complex>
#include <thread>

#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)

const int N = 2400;     // matrix dimension NxN
const int K = 2000;     // number of iterations
const int streamsize = 5;

// mandelbrot escape time algorithm: from opencv-docs
int mandelbrotAlgorithm(const std::complex<float> &z0, const int max) {
    std::complex<float> z = z0;
    for (int t = 0; t < max; t++) {
        if (z.real()*z.real() + z.imag()*z.imag() > 4.0f) return t;
        z = z*z + z0;
    }
    return max;
}

//! mandelbrot: modified from opencv-docs
cv::Vec3f mandelbrotFormula(const std::complex<float> &z0, const int maxIter=500) {
    int value = mandelbrotAlgorithm(z0, maxIter);
    if(maxIter - value == 0)
        return 0;
    cv::Vec3f color;
    color[1] = (sqrt(value / (float) maxIter) * 255);
    return color;
}

// it represents a subtask element for map computation: it will be
// the parameter for "op" function
struct Point {

    Point(const float x0, const float y0, cv::Vec3f& ch) :
        z(x0,y0), ch(ch) {}

    const std::complex<float> z;
    cv::Vec3f& ch;

};

// Mandelbrot Matrix, it embeds cv::Mat, transformation values
// and max iterations needed
template <typename T>
class MMatrix {
public:
    MMatrix() : mat() {}

    MMatrix(int rows, int cols) :
        mat(rows, cols, CV_32FC3) {

        // mandelbrot transformation
        x1 = -2.1f; x2 = 0.6f;
        y1 = -1.2f; y2 = 1.2f;
        scaleX = mat.cols / (x2 - x1);
        scaleY = mat.cols / (y2 - y1);

    }

    Point operator[](const int idx) {
        int i = idx / mat.cols;
        int j = idx % mat.cols;
        float x0 = j / scaleX + x1;
        float y0 = i / scaleY + y1;
        return Point(x0, y0, mat.at<cv::Vec3f>(i,j));
    }

    int size() {
        return mat.rows*mat.cols;
    }

    cv::Mat mat;
    float scaleX;
    float scaleY;
    float x1, x2;
    float y1, y2;
    int index;
};

// generates a stream of matrices having same size but that may need
// to be computed with different number of iterations
class matrixsource : public source<MMatrix<Point>> {

public:

    bool has_next() {
        return start < end;
    }

    MMatrix<Point>* next() {
        auto m = new MMatrix<Point>(N, N);
        m->index = ++start;
        return m;
    }

private:
    int start{0};
    int end{streamsize};
};

class mandelbrot : public seq_wrapper<MMatrix<Point>, MMatrix<Point>> {
public:
    MMatrix<Point> compute( MMatrix<Point>& mmat ) {
        for ( int i = 0; i < mmat.size(); i++ )
            op(mmat[i]);
        return mmat;
    }

    void op( Point&& p ) {
        p.ch = mandelbrotFormula(p.z, K);
    }

};

class colorize : public seq_wrapper<MMatrix<Point>, MMatrix<Point>> {
public:
    MMatrix<Point> compute( MMatrix<Point>& mmat ) {
        std::hash<std::string> hash_fn;
        for ( int i = 0; i < mmat.size(); i++ ) {
            Point p = mmat[i];
            float ratio = p.ch[1]/255;
            p.ch[0] = ratio*(hash_fn(std::to_string(mmat.index+1))%255);
            p.ch[1] = ratio*(hash_fn(std::to_string(mmat.index+2))%255);
            p.ch[2] = ratio*(hash_fn(std::to_string(mmat.index+3))%255);
        }
    return mmat;
    }
};

// drain wrapper
class matrix_drain : public drain<MMatrix<Point>> {
public:
    void process(MMatrix<Point>* res) {
        const std::string filename = STRINGIFY(PRE) "_mandelbrot_" + std::to_string(res->index) + ".png";
        std::cout << "saving " << filename << std::endl;
        cv::imwrite(filename, res->mat);
    }
};
