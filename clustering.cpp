#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
using namespace std;

void copkm(vector<vector<float> >* cl_set, vector<vector<int> >* cl_set_const, int k, int seed){
    
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
   srand(seed);
   for(int i=0; i<k; i++){
       aux_centro.clear();
       for(int j=0; j<minimo.size(); j++)aux_centro.push_back(rand() % (int)(maximo[j]-minimo[j]) + minimo[j]);
       centroides.push_back(aux_centro);
   }

   //REVISAR, CON LA MISMA SEED SALEN TODOS LOS CENTROIDES IGUALES
   
   /*for(int i=0; i<centroides.size(); i++){
       for(int j=0; j<centroides[i].size(); j++) cout << centroides[i][j] << " ";
       cout << endl;
   }*/

}

void busqueda_local(vector<vector<float> >* cl_set, vector<vector<int> >* cl_set_const, int k, int seed){

}

int main(int argc, char* argv[]){

    //Lectura de datos de referencias por argumento o por teclado

    string ruta = "data/", restricciones = "data/";
    int k, seed = 16, algoritmo;

    if (argc >= 5){
        ruta += argv[1];
        restricciones += argv[2];
        k = atoi(argv[3]);
        algoritmo = atoi(argv[4]);
        if(argc == 6) seed = atoi(argv[5]);
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
        cout << "Introduzca la semilla para la generación de números aleatorios: ";
        cin >> seed;
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

    if(algoritmo==0) copkm(&cl_set, &cl_set_const, k, seed);
    else busqueda_local(&cl_set, &cl_set_const, k, seed);

    return 0;
}