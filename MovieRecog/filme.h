#include <iostream>
#include <fstream>
#include <vector>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "lib/GeometricRecognizer.h"
#include "lib/PathWriter.h"


#define ALTU 700
#define COMP 450

using namespace std;
using namespace cv;
using namespace DollarRecognizer;

class Filme{
    Mat capa;

    //MatND hist_capa;
    //MultiStrokeGesture original;

    Size img_size;
    vector<KeyPoint> keypoints;
    Mat descriptors;
    string link;


public:
    string nome;
    Filme(Mat img, char* n, char *l);
    void MostraFilme();
    Size get_size();
    vector<KeyPoint> get_keypoints();
    Mat get_capa();
    int n_good_matches(vector <KeyPoint> foto, Mat corte);
    const char *getlink();
};

Filme::Filme(Mat img, char *n,char *l){
    capa=img.clone();
    string nova(n);
    string novolink(l);
    nome=nova;
    link=novolink;
    img_size=Size(capa.cols,capa.rows);
    int minHessian = 400;
    SurfFeatureDetector detector( minHessian );

    detector.detect( capa, keypoints );

}

const char* Filme::getlink(){
    return link.c_str();
}



void Filme::MostraFilme(){
    Mat copia;

    imshow(nome.c_str(),capa);
    waitKey();

}

Size Filme::get_size(){
    return img_size;
}

vector<KeyPoint> Filme:: get_keypoints(){
    return keypoints;
}

Mat Filme::get_capa(){
    return capa.clone();
}

int Filme::n_good_matches(vector <KeyPoint> foto,Mat corte){
    SurfDescriptorExtractor extractor;

      Mat descriptors_1, descriptors_2;
      Mat a=capa;
      Mat b=corte;
      vector<KeyPoint> vka=keypoints;
      vector<KeyPoint> vkb=foto;

      extractor.compute( a, vka, descriptors_1 );
      extractor.compute( b, vkb, descriptors_2 );
      //-- Step 3: Matching descriptor vectors using FLANN matcher
      FlannBasedMatcher matcher;
      std::vector< DMatch > matches;
      matcher.match( descriptors_1, descriptors_2, matches );

      double max_dist = 0; double min_dist = 100;

      //-- Quick calculation of max and min distances between keypoints
      for( int i = 0; i < descriptors_1.rows; i++ )
      { double dist = matches[i].distance;
        if( dist < min_dist ) min_dist = dist;
        if( dist > max_dist ) max_dist = dist;
      }

      //printf("-- Max dist : %f \n", max_dist );
      //printf("-- Min dist : %f \n", min_dist );

      //-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist,
      //-- or a small arbitary value ( 0.02 ) in the event that min_dist is very
      //-- small)
      //-- PS.- radiusMatch can also be used here.
      std::vector< DMatch > good_matches;

      for( int i = 0; i < descriptors_1.rows; i++ )
          {
          int pos1=matches[i].queryIdx;
          int pos2=matches[i].trainIdx;
          KeyPoint key1=vka[pos1];
        KeyPoint key2=vkb[pos2];
        Point2f ponto1=key1.pt;
        Point2f ponto2=key2.pt;

        double key1x=ponto1.x;
        double key1y=ponto1.y;

        double key2x=ponto2.x;
        double key2y=ponto2.y;




        int tdist=0;
        int raio=60;
        if((key1x +raio)>=key2x && (key1x-raio)<=key2x && (key1y +raio)>=key2y && (key1y-raio)<=key2y)tdist=1;

          if( matches[i].distance <= max(3*min_dist, 0.5) && tdist  )
        { good_matches.push_back( matches[i]); }
      }

      //cout << "CONTAGEM DE MATCHES: " << good_matches.size() << endl;
      //-- Draw only "good" matches
      /*Mat img_matches;
      drawMatches( a, vka, b,vkb,
                   good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
                   vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

      //-- Show detected matches
      imshow( nome, img_matches );
*/
    return good_matches.size();
}

