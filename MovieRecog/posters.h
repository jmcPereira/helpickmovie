#ifndef POSTERS_H
#define POSTERS_H


#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include <vector>
#include <iostream>
#include <QDebug>
#include "avaliacao.h"
#include "lib/GeometricRecognizer.h"
#include "lib/PathWriter.h"
#include <cmath>


using namespace cv;
using namespace std;
using namespace DollarRecognizer;

Point p1,p2;
int pronto=0,definidos=0;

class Posters{

    Mat foto;
    Mat corte;
    int n_posters;
    vector<Mat> posters;
    //MultiStrokeGesture original;
    vector<KeyPoint> keypoints;


public:
    Posters(Mat f);
    void identifica_posters();
    Mat get_foto();
    void corta(Size sz);
    //MatND get_hist();
    Mat get_corte();
    //void set_arestas();
    //MultiStrokeGesture get_arestas();
    vector<KeyPoint> get_KeyPoints();
    void identifica();
    void identifica_posters2();
};

Posters::Posters(Mat f){
    foto=f.clone();
    n_posters=0;

    Mat resized;
    resize(foto,corte,Size(400,593),0,0,INTER_LINEAR);
    //imshow("Resized",resized);
    //imshow("Corte",corte);
    int minHessian = 400;
    SurfFeatureDetector detector( minHessian );

    detector.detect( corte, keypoints );

}



void Posters::identifica(){
    int scale = 2;
    int delta = 1;
    int ddepth = CV_16S;

    vector <Mat > parts;

    for(int i=0;i<foto.cols;i+=(foto.cols*0.2)){
        for (int j=0;j<foto.rows;j+=foto.rows*0.2){
            int x2=i+foto.cols*0.2,y2=j+foto.rows*0.2;
            if(x2>foto.cols)x2=foto.cols-1;
            if(y2>foto.rows)y2=foto.rows-1;
            Rect rectangulo(i,j,x2,y2);
            Mat cut=foto(rectangulo).clone();
            parts.push_back(cut.clone());
        }
    }

    for(int k=0;k<parts.size();k++){
        imshow("Parte",parts[k]);
        waitKey();
    }

    Mat foto_gray,grad_x,grad_y,gradient;
    GaussianBlur( foto,foto, Size(3,3), 0, 0, BORDER_DEFAULT );
    cvtColor(foto,foto_gray,CV_RGB2GRAY);
    Sobel( foto_gray,grad_x, ddepth, 1,0, 1, scale, delta, BORDER_DEFAULT );
    Sobel( foto_gray,grad_y, ddepth, 0,1, 1, scale, delta, BORDER_DEFAULT );
    convertScaleAbs( grad_x,grad_x );
    convertScaleAbs( grad_y,grad_y );
    addWeighted(grad_x,1,grad_y,1,1,gradient);
    threshold(gradient,gradient, 50, 255, CV_THRESH_BINARY | CV_THRESH_OTSU );
    imshow("sobel_X",gradient);
}

Mat Posters::get_foto(){
    return foto.clone();
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata){
     if  ( event == EVENT_LBUTTONDOWN ){
         p1.x=x;
         p1.y=y;
         definidos=1;
     }
     else if  ( event == EVENT_LBUTTONUP ){
         p2.x=x;
         p2.y=y;
     }
     else if  ( event == EVENT_RBUTTONDOWN )
     {
          if(definidos){

              cout << "Pontos definidos! " << endl;
              pronto=1;
              destroyAllWindows();

          }
          else cout << "Pontos ainda não definidos! "<< endl;
     }

}


void Posters::corta(Size sz){
    namedWindow("Foto",WINDOW_NORMAL);
    setMouseCallback("Foto",CallBackFunc,NULL);
    imshow("Foto",foto);
    while(pronto!=1)waitKey();
    Rect rectangulo(p1.x,p1.y,p2.x-p1.x,p2.y-p1.y);
    corte=foto(rectangulo).clone();
    Mat resized;
    resize(corte,corte,Size(400,593),0,0,INTER_LINEAR);
    //imshow("Resized",resized);
    imshow("Corte",corte);
    int minHessian = 400;
    SurfFeatureDetector detector( minHessian );

    detector.detect( corte, keypoints );
    waitKey();
}


Mat Posters::get_corte(){
    return corte.clone();
}


vector<KeyPoint> Posters::get_KeyPoints(){

    return keypoints;

}

static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

/**
 * Helper function to display text in the center of a contour
 */
void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour)
{
    int fontface = cv::FONT_HERSHEY_SIMPLEX;
    double scale = 0.4;
    int thickness = 1;
    int baseline = 0;

    cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
    cv::Rect r = cv::boundingRect(contour);

    cv::Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
    cv::rectangle(im, pt + cv::Point(0, baseline), pt + cv::Point(text.width, -text.height), CV_RGB(255,255,255), CV_FILLED);
    cv::putText(im, label, pt, fontface, scale, CV_RGB(0,0,0), thickness, 8);
}


void Posters::identifica_posters2(){

    Mat src=foto.clone();
    for(int j=3;j<5;j+=2)medianBlur(src,src,j);
    cv::Mat gray;
        cv::cvtColor(src, gray, CV_BGR2GRAY);

        // Use Canny instead of threshold to catch squares with gradient shading
        cv::Mat bw;
        cv::Canny(gray, bw,0, 50, 3);

        // Find contours
        std::vector<std::vector<cv::Point> > contours;
        vector<Vec4i> hierarchy;
        cv::findContours(bw.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

        Mat drawing = Mat::zeros( bw.size(), CV_8UC3 );
          for( int i = 0; i< contours.size(); i++ )
             {
               Scalar color = Scalar( 0,0,255);
               if(contourArea(contours[i])>100)drawContours( drawing, contours, i, color, 2, 8,hierarchy, 0, Point() );

             }

          /// Show in a window
          namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
          imshow( "Contours", drawing );

        std::vector<cv::Point> approx;
        cv::Mat dst = src.clone();

        for (int i = 0; i < contours.size(); i++)
        {
            // Approximate contour with accuracy proportional
            // to the contour perimeter
            cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);

            // Skip small or non-convex objects
            if (std::fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx))
                continue;

            if (approx.size() == 3)
            {
                setLabel(dst, "TRI", contours[i]);    // Triangles
            }
            else if (approx.size() >= 4 && approx.size() <= 6)
            {
                // Number of vertices of polygonal curve
                int vtc = approx.size();

                // Get the cosines of all corners
                std::vector<double> cos;
                for (int j = 2; j < vtc+1; j++)
                    cos.push_back(angle(approx[j%vtc], approx[j-2], approx[j-1]));

                // Sort ascending the cosine values
                std::sort(cos.begin(), cos.end());

                // Get the lowest and the highest cosine
                double mincos = cos.front();
                double maxcos = cos.back();

                // Use the degrees obtained above and the number of vertices
                // to determine the shape of the contour
                if (vtc == 4 && mincos >= -0.1 && maxcos <= 0.3)
                    setLabel(dst, "RECT", contours[i]);
                else if (vtc == 5 && mincos >= -0.34 && maxcos <= -0.27)
                    setLabel(dst, "PENTA", contours[i]);
                else if (vtc == 6 && mincos >= -0.55 && maxcos <= -0.45)
                    setLabel(dst, "HEXA", contours[i]);
            }
            else
            {
                // Detect and label circles
                double area = cv::contourArea(contours[i]);
                cv::Rect r = cv::boundingRect(contours[i]);
                int radius = r.width / 2;

                if (std::abs(1 - ((double)r.width / r.height)) <= 0.2 &&
                    std::abs(1 - (area / (CV_PI * std::pow(radius, 2)))) <= 0.2)
                    setLabel(dst, "CIR", contours[i]);
            }
        }

        cv::imshow("src", src);
        cv::imshow("dst", dst);
        cv::waitKey(0);
}


#endif // POSTERS_H
