#include "avaliacao.h"
#include "cmath"



avaliacao::avaliacao(Mat imagem, string name, int div){

    nome=name;

    vector <Mat> canais;
    cvtColor(imagem,imagem,CV_BGR2HSV);
    split(imagem,canais);
    Mat imagem_red=canais[0].clone();

    Scalar valor;
    int v;

    for(int k=0;k-1<180/div;k++)valores.push_back(0);


    for(int i=0;i<imagem_red.rows;i++){
        for(int j=0;j<imagem_red.cols;j++){
            valor=imagem_red.at<uchar>(i,j);
            v=valor.val[0];

            v=v/div;
            valores[v]++;

        }
    }

    float pixels =imagem_red.cols*imagem_red.rows;


    for(int k=0;k-1<180/div;k++){
        valores[k]=(valores[k]/pixels)*100;
        cout << "VALOR " << k << ": " << valores[k] << endl;
    }

}

int avaliacao::compara(avaliacao vals){
    int dif=0;

    for(int i=0;i<vals.valores.size();i++){
        dif+=sqrt(pow((valores[i]-vals.valores[i]),2));
    }

    return dif;
}

