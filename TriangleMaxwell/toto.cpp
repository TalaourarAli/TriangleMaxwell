

// TriangleMaxwell.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//
// TALAOURAR Ali
//
// Compile : cmake
// Execute : ./main.o

 
#include <iostream>

#include <opencv2/opencv.hpp>
#include <algorithm>

//#define DEBUG

using namespace std;
using namespace cv;


Point2d L, M, S;

void setTringleValues(Mat& mat) {
    L.y = 0;
    L.x = mat.rows * 0.86;
    M.y = mat.cols / 2;
    M.x = 0;
    S.y = mat.cols;
    S.x = mat.rows * 0.86;
}

void drawTriangleInImage(Mat& mat){
    int thickness = 1;
    int lineType = LINE_8;
    line( mat,
        M,
        L,
        Scalar( 100 ),
        thickness,
        lineType );
    line( mat,
        S,
        M,
        Scalar( 100 ),
        thickness,
        lineType );
    line( mat,
        L,
        S,
        Scalar( 100 ),
        thickness,
        lineType );
}


Point2d baryCentre(double a, double b, double c) {
    Point2d ret;
    if ((a + b + c) == 0) ret = { 0,0 }; //return Centre
    else {
        ret.x = ((a * (double)S.x) + (b * (double)M.x) + (c * (double)L.x)) / (a + b + c);
        ret.y = ((a * (double)S.y) + (b * (double)M.y) + (c * (double)L.y)) / (a + b + c);

    }
    return ret;
}


void showImg(Mat& img, const char title[]) {

    setTringleValues(img);

    Mat test(img.rows + 1, img.cols + 1, CV_8U, Scalar(255));
    drawTriangleInImage(test);

    if (!img.data) {
        cout << "image not laoded" << endl;
        return;
    }

    for (int i = 0; i < 100; i++){
        Point2d v = baryCentre(255, i, i);
        test.at<uchar>((int)v.x, (int)v.y) = 0;
    }
    for (int i = 0; i < img.rows; i++)
        for (int j = 0; j < img.cols; j++) {
            Point2d v = baryCentre(img.at<Vec3b>(i, j)[0], img.at<Vec3b>(i, j)[1], img.at<Vec3b>(i, j)[2]);

#ifdef DEBUG
            cout << "v.x = " << v.x << " and v.y = " << v.y << endl << "img.size = " << img.size << endl;
#endif // DEBUG
            
            test.at<uchar>((int)v.x, (int)v.y) = 0;
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

