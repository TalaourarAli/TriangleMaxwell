

// TriangleMaxwell.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//
// TALAOURAR Ali
//
// Compile : cmake
// Execute : ./main.o


#include <iostream>
#include <memory>

#include <opencv2/opencv.hpp>
#include <algorithm>
#include <cmath>

//#define DEBUG

using namespace std;
using namespace cv;


Point2d L, M, S;

void setTringleValues(Mat& mat) {
    L.x = 0;
    L.y = mat.rows * 0.86;
    M.x = mat.cols / 2;
    M.y = 0;
    S.x = mat.cols;
    S.y = mat.rows * 0.86;
}

void drawTriangleInImage(Mat& mat) {
    int thickness = 1;
    int lineType = LINE_8;
    line(mat,
        M,
        L,
        Scalar(100),
        thickness,
        lineType);
    line(mat,
        S,
        M,
        Scalar(100),
        thickness,
        lineType);
    line(mat,
        L,
        S,
        Scalar(100),
        thickness,
        lineType);
}


unique_ptr<Point2d> baryCentre(double a, double b, double c) {
    unique_ptr<Point2d> ret = make_unique<Point2d>();
    if ((a + b + c) == 0) *ret = { 0,0 }; //return Centre
    else {
        ret->x = ((a * (double)S.x) + (b * (double)M.x) + (c * (double)L.x)) / (a + b + c);  // R  G  B : a réordonner 
        ret->y = ((a * (double)S.y) + (b * (double)M.y) + (c * (double)L.y)) / (a + b + c);
    }
    return ret;
}


void writeLMSletters(Mat& img) {
    putText(img, "L(R)",
        L + Point2d(0, 10),
        FONT_ITALIC,
        0.5,
        Scalar(0)
    );        
    putText(img, "S(B)",
        S + Point2d(-20, 10),
        FONT_ITALIC,
        0.5,
        Scalar(0)
    );        
    putText(img, "M(G)",
        M + Point2d(0, 10),
        FONT_ITALIC,
        0.5,
        Scalar(0)
    );
}

void showImg(Mat& img, const char title[]) {

    setTringleValues(img);

    Mat test(img.rows + 1, img.cols + 1, CV_8U, Scalar(255));
    drawTriangleInImage(test);
    writeLMSletters(test);

    if (!img.data) {
        cout << "image not laoded" << endl;
        return;
    }
#ifdef DEBUG

    for (int i = 0; i < 100; i++) {
        Point2d v = baryCentre(255, i, i);
        test.at<uchar>((int)v.y, (int)v.x) = 0;
    }
#endif // DEBUG
    for (int i = 0; i < img.rows; i++)
        for (int j = 0; j < img.cols; j++) {
            auto v = baryCentre(img.at<Vec3b>(i, j)[0], img.at<Vec3b>(i, j)[1], img.at<Vec3b>(i, j)[2]);
#ifdef DEBUG
            cout << "v.x = " << v.x << " and v.y = " << v.y << endl << "img.size = " << img.size << endl;
#endif // DEBUG
            test.at<uchar>(ceil(v->y), ceil(v->x)) = 0;
        }
    if (test.data) imshow(title, test);
}

int main()
{
    Mat imgR = imread("../res/automne.jpg");
    Mat imgG = imread("../res/v.jpg");
    Mat imgB = imread("../res/m.jpg");

    showImg(imgR, "Rouge");
    showImg(imgG, "Vert");
    showImg(imgB, "Bleu");

    waitKey(0);
    return 0;
}

