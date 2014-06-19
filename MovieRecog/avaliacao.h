#ifndef AVALIACAO_H
#define AVALIACAO_H

#include <vector>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

class avaliacao{


    string nome;

public:
    vector <int> valores;
    avaliacao(Mat imagem, string name, int div);
    int compara(avaliacao vals);
};

#endif // AVALIACAO_H
