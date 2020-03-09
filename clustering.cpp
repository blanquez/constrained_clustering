#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <cmath>
using namespace std;

void copkm(vector<vector<float> >* cl_set, vector<vector<int> >* cl_set_const, int k, vector<int> seed){
    
    //Creacion de centroides aleatorios
    
    cout << "Creando centroides iniciales..." << endl;

    vector<float> minimo;
    vector<float> maximo;
    for(int i=0; i < (*cl_set)[0].size(); i++){
        minimo.push_back((*cl_set)[0][i]);
        maximo.push_back((*cl_set)[0][i]);
    }
    for(int i=1; i < (*cl_set).size(); i++){
        for(int j=0; j < (*cl_set)[0].size(); j++){
            if(minimo[j] > (*cl_set)[i][j]) minimo[j] = (*cl_set)[i][j];
            if(maximo[j] < (*cl_set)[i][j]) maximo[j] = (*cl_set)[i][j];
        }
    }

   vector<vector<float> > centroides;
   vector<float> aux_centro;
   
  for(int i=0; i<k; i++){
       srand(seed[i]);
       aux_centro.clear();
       for(int j=0; j<minimo.size(); j++) aux_centro.push_back(rand() % (int)(maximo[j]-minimo[j]) + minimo[j]);
       centroides.push_back(aux_centro);
   }
   
   for(int i=0; i<centroides.size(); i++){
       cout << "Centroide " << i << ": ";
       for(int j=0; j<centroides[i].size(); j++) cout << centroides[i][j] << " ";
       cout << endl;
   }

   //Aplicamos el algoritmo

   cout << "Aplicando algoritmo Greedy COPKM..." << endl;

   vector<pair<int, int> > c;
   vector<pair<int, int> > nuevo_c;
   vector<int> inf_actual;
   pair<int,int> par;
   int minimo_cl, minimos_ind_sec, aux_calc;
   vector<int> minimos_ind;

    //Mientras cambien los clusters aplicamos el algoritmo
   do{
       nuevo_c = c;
       //Introducir nodo en el mejor cluster
       for(int i=0; i<(*cl_set).size(); i++){
           inf_actual.clear();
           for(int j=0; j<k; j++) inf_actual.push_back(0);

            //Calcular infeasibility
           for(int j=0; j<nuevo_c.size(); j++){
                if((*cl_set_const)[i][nuevo_c[j].first] == -1 ) inf_actual[nuevo_c[j].second]++;
                else if((*cl_set_const)[i][nuevo_c[j].first] == 1 ) for(int m=0; m<k; m++) if(m != nuevo_c[j].second) inf_actual[m]++;
           }

            minimo_cl = inf_actual[0];
            minimos_ind.push_back(0);
           //Escoger cluster
           for(int j=1; j<k ; j++){
               if(inf_actual[j] < minimo_cl){
                   minimos_ind.clear();
                   minimo_cl = inf_actual[j];
                   minimos_ind.push_back(j);
               }
               else if(inf_actual[j] == minimo_cl) minimos_ind.push_back(j);
           }
           
           //Si hay mas de un minimo, lo elegimos por distancia
           if(minimos_ind.size() == 1) par = make_pair(i,minimos_ind[0]);
           else{
               minimos_ind_sec = minimos_ind[0];
               for(int j=0; j<centroides[j].size(); j++) minimo_cl += pow((centroides[0][j]-(*cl_set)[minimos_ind_sec][j]), 2);
               minimo_cl = sqrt(minimo_cl);
               for(int j=1; j<minimos_ind.size(); j++){
                   aux_calc = 0;
                   for(int l=0; l<centroides[0].size(); l++) aux_calc += pow((centroides[j][l]-(*cl_set)[minimos_ind_sec][l]), 2);
                   aux_calc = sqrt(aux_calc);
                   if(aux_calc < minimo_cl){
                       minimo_cl = aux_calc;
                       minimos_ind_sec = j;
                   }
               }
               par = make_pair(i, minimos_ind[minimos_ind_sec]);
           }
           nuevo_c.push_back(par);
       }
       //Ajustar centroides
       for(int i=0; i<k; i++){

       }
   }while(c != nuevo_c);

}

void busqueda_local(vector<vector<float> >* cl_set, vector<vector<int> >* cl_set_const, int k, vector<int> seed){

}

int main(int argc, char* argv[]){

    //Lectura de datos de referencias por argumento o por teclado

    string ruta = "data/", restricciones = "data/";
    int k, algoritmo;
    vector<int> seed;

    if (argc >= 5){
        ruta += argv[1];
        restricciones += argv[2];
        k = atoi(argv[3]);
        algoritmo = atoi(argv[4]);
        if(argc > 5) for(int i=0; i<k; i++) seed.push_back(atoi(argv[i+5]));
        else for(int i=0; i<k; i++) seed.push_back(160*(i+1));
    }else{
        string aux_ruta;
        cout << "\nNo ha especificado el número correcto de argumentos: se le pedirán a continuación" << endl;
        cout << "Introduzca el nombre del archivo que contiene el dataframe: ";
        cin >> aux_ruta;
        ruta += aux_ruta;
        cout << "Introduzca el nombre del archivo que contiene el dataframe: ";
        cin >> aux_ruta;
        restricciones += aux_ruta;
        cout << "Introduzca el algoritmo deseado(0:Greedy COPKM, 1:Búsqueda local): ";
        cin >> algoritmo;
        cout << "Introduzca el número de clusters que se crearán: ";
        cin >> k;
        cout << "Introduzca tantas semillas como clusters para la generación de números aleatorios: ";
        for(int i=0; i<k; i++) cin >> seed[i];
    }

    if(k<1){
        cout << "El número de clusters debe ser positivo" << endl;
        return 0;
    }

    cout << "Cargando datos..." << endl;

    //Calculo de dimesiones

    ifstream f(ruta);
    if(!f){
        cout << "Error al abrir el archivo del dataframe" << endl;
        return 0;
    }

    int dimension = 0;
    float lectura_float;
    char basura = ',';

    while(basura == ','){
        dimension++;
        f >> lectura_float;
        f >> basura;
    }

    f.seekg(0);

    //Lectura del dataframe

    vector<vector<float> > cl_set;
    vector<float> tupla_cl;

    while (!f.eof()){
        tupla_cl.clear();
        for(int i=0; i<dimension; i++){
            f >> lectura_float;
            f.ignore();
            tupla_cl.push_back(lectura_float);
        }
        cl_set.push_back(tupla_cl);
    }

    f.close();

    //Lectura de restricciones

    vector<vector<int> > cl_set_const;
    vector<int> tupla_const;
    int lectura_int;

    f.open(restricciones);

    if(!f){
        cout << "Error al abrir el archivo de restricciones" << endl;
        return 0;
    }

    for(int i=0; i<cl_set.size(); i++){
        tupla_const.clear();
        for(int j=0; j<cl_set.size(); j++){
            f >> lectura_int;
            f.ignore();
            tupla_const.push_back(lectura_int);
        }
        cl_set_const.push_back(tupla_const);
    }

    f.close();

    //Seleccion de algoritmo

    if(algoritmo==0){
        srand(seed[0]);
        //Hay que barajar conjuntamente el conjunto y las restricciones
        copkm(&cl_set, &cl_set_const, k, seed);
    }
    else busqueda_local(&cl_set, &cl_set_const, k, seed);

    return 0;
}