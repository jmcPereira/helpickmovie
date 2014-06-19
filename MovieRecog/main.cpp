#include <iostream>
#include <stdio.h>
#include "filme.h"
#include "posters.h"

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/features2d.hpp"

using namespace std;
using namespace cv;
using namespace DollarRecognizer;




int main(int argc, char *argv[]){

    if(argc!=2){
        return 0;
    }

    vector <Filme> filmes;

    Mat capa=imread("noah.jpg");
    if(!capa.data)cout << "Erro ao Carregar Noah.jpg" << endl;
    Filme noah(capa,"Noah","http://www.imdb.com/title/tt1959490/?ref_=nv_sr_1");
    filmes.push_back(noah);
    //hobbit.MostraFilme();

    Mat capa2=imread("batman.jpg");
    if(!capa2.data)cout << "Erro ao Carregar Batman.jpg" << endl;
    Filme batman(capa2,"Batman","http://www.imdb.com/title/tt1345836/?ref_=nv_sr_2");
    filmes.push_back(batman);
    //batman.MostraFilme();

    Mat capa3=imread("transcendence.jpg");
    if(!capa3.data)cout << "Erro ao Carregar transcendence.jpg" << endl;
    Filme transcendence(capa3,"Transcendence","http://www.imdb.com/title/tt2209764/?ref_=nv_sr_2");
    filmes.push_back(transcendence);

    Mat capa4=imread("360.jpg");
    if(!capa4.data)cout << "Erro ao Carregar 360.jpg" << endl;
    Filme f360(capa4,"360","http://www.imdb.com/title/tt1680045/?ref_=nv_sr_1");
    filmes.push_back(f360);

    Mat capa5=imread("brave.jpg");
    if(!capa5.data)cout << "Erro ao Carregar Brave.jpg" << endl;
    Filme brave(capa5,"Brave","http://www.imdb.com/title/tt1217209/?ref_=nv_sr_2");
    filmes.push_back(brave);

    Mat capa6=imread("hercules.jpg");
    if(!capa6.data)cout << "Erro ao Carregar Hercules.jpg" << endl;
    Filme herc(capa6,"Hercules","http://www.imdb.com/title/tt1043726/?ref_=fn_al_tt_2");
    filmes.push_back(herc);

    Mat capa7=imread("marretas.jpg");
    if(!capa7.data)cout << "Erro ao Carregar Marretas.jpg" << endl;
    Filme marr(capa7,"Marretas","http://www.imdb.com/title/tt2281587/?ref_=nv_sr_1");
    filmes.push_back(marr);

    Mat capa8=imread("mercenaries2.jpg");
    if(!capa8.data)cout << "Erro ao Carregar Mercenaries2.jpg" << endl;
    Filme mercs2(capa8,"Mercenarios 2","http://www.imdb.com/title/tt1764651/?ref_=nv_sr_2");
    filmes.push_back(mercs2);

    Mat capa9=imread("oceu.jpg");
    if(!capa9.data)cout << "Erro ao Carregar Oceu.jpg" << endl;
    Filme oceu(capa9,"O Ceu Existe Mesmo","http://www.imdb.com/title/tt1929263/?ref_=nv_sr_1");
    filmes.push_back(oceu);

    Mat capa10=imread("sabotagem.jpg");
    if(!capa10.data)cout << "Erro ao Carregar Sabotagem.jpg" << endl;
    Filme sab(capa10,"Sabotagem","http://www.imdb.com/title/tt1742334/?ref_=nv_sr_1");
    filmes.push_back(sab);

    Mat capa11=imread("spiderman2.jpg");
    if(!capa11.data)cout << "Erro ao Carregar Spiderman2.jpg" << endl;
    Filme spider2(capa11,"The Amazing Spiderman 2","http://www.imdb.com/title/tt1872181/?ref_=nv_sr_1");
    filmes.push_back(spider2);

    Mat capa12=imread("residentevil.jpg");
    if(!capa12.data)cout << "Erro ao Carregar ResidentEvil.jpg" << endl;
    Filme residentevil(capa12,"Resident Evil","http://www.imdb.com/title/tt1855325/?ref_=nv_sr_1");
    filmes.push_back(residentevil);
//coco
    Mat foto=imread(argv[1]);
    if(!foto.data)cout << "Error loading picture." << endl;

    Posters p(foto);
    //p.identifica_posters();
    //p.corta(transcendence.get_size());
    int maximo_val=0;
    int indice_max=0;
    int contador=0;
    for(int k=0;k<filmes.size();k++){
        contador=filmes[k].n_good_matches(p.get_KeyPoints(),p.get_corte());
        if(contador>maximo_val){
            maximo_val=contador;
            indice_max=k;
        }
    }
    /*cout << "Melhor match encontrado : \n  --nº de matches: " << maximo_val << "\n  --Nome do Filme: " << filmes[indice_max].nome;
    filmes[indice_max].MostraFilme();*/
    cout << filmes[indice_max].getlink() ;

    waitKey(0);

    return 1;

}
