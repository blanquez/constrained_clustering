#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <cmath>
#include <algorithm>
using namespace std;

void copkm(vector<vector<float> >* cl_set, vector<vector<int> >* cl_set_const, int k, vector<int> seed){
    
    // Barajar un vector de indices 0,1,...,cl_set.size() y luego explorar a partir de este vector

    vector<int> indices;
    for(int i=0; i < (*cl_set).size(); i++) indices.push_back(i);
    srand(seed[0]);
    random_shuffle(indices.begin(), indices.end());

    /*for(int j=0;j<indices.size();j++) cout << indices[j] << " ";
    cout << endl;*/
    
    // Creacion de centroides aleatorios
    
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
   
   /*for(int i=0; i<centroides.size(); i++){
       cout << "Centroide " << i << ": ";
       for(int j=0; j<centroides[i].size(); j++) cout << centroides[i][j] << " ";
       cout << endl;
   }*/

   // Aplicamos el algoritmo

   cout << "Aplicando algoritmo Greedy COPKM..." << endl;

   vector<int> c_old;               // Asignación de clusters
   vector<int> aux_baraja;
   vector<int> c;         // Asignación instantánea de clusters
   vector<int> inf_actual;      // Infeasibility
   float minimo_cl, aux_calc, distancia=0;   // Minima infeasibility
   int minimo_ind;     // Indice de la minima infeasibility

    // Mientras cambien los clusters aplicamos el algoritmo
   do{
       c_old = c;
       c.clear();
       //Introducir nodo en el mejor cluster
       for(int i=0; i<(*cl_set).size(); i++){
           inf_actual.clear();
           for(int j=0; j<k; j++) inf_actual.push_back(0);

            //Calcular infeasibility
           for(int j=0; j<c.size(); j++){
                if((*cl_set_const)[indices[i]][j] == -1 ) inf_actual[c[j]]++;
                else if((*cl_set_const)[indices[i]][j] == 1 ) for(int m=0; m<k; m++) if(m != c[j]) inf_actual[m]++;
           }

           /*for(int j=0;j<inf_actual.size();j++){
               cout << "Inf " << j <<": " << inf_actual[j] << endl;
           }
           cout << endl;*/

            minimo_cl = inf_actual[0];
            minimo_ind=0;
            for(int j=0; j<centroides[0].size(); j++) distancia += pow(centroides[0][j]-(*cl_set)[indices[i]][j],2);
            distancia = sqrt(distancia);

           //Escoger cluster
           for(int j=1; j<k ; j++){
               aux_calc = 0;
               for(int l=0; l<centroides[0].size(); l++) aux_calc += pow(centroides[j][l]-(*cl_set)[indices[i]][j],2);
               aux_calc = sqrt(aux_calc);
               if((inf_actual[j] < minimo_cl) || ( (inf_actual[j] == minimo_cl) && (aux_calc < distancia) ) ){
                   minimo_cl = inf_actual[j];
                   minimo_ind = j;
                   distancia = aux_calc;
               }
           }
           c.push_back(minimo_ind);
       }

       //Reordeno por orden

       aux_baraja = c;
       for(int i=0; i<c.size(); i++) c[indices[i]] = aux_baraja[i];

       //Ajustar centroides
       vector<float> contador;
       for(int i=0; i<centroides.size(); i++) contador.push_back(0);
       for(int i=0; i<c.size(); i++) contador[c[i]]++;

       for(int i=0; i<centroides.size(); i++)
            for(int j=0; j<centroides[0].size(); j++)
                centroides[i][j] = 0;

       for(int i=0; i<c.size(); i++)
           for(int j=0; j<centroides[0].size(); j++)
                centroides[c[i]][j] += (*cl_set)[i][j];

        for(int i=0; i<centroides.size(); i++)
           for(int j=0; j<centroides[0].size(); j++)
                centroides[i][j] /= contador[i];

        /*for(int i=0; i<centroides.size(); i++){
            cout << "Centroide " << i << ": ";
            for(int j=0; j<centroides[0].size(); j++) cout << centroides[i][j] << " ";
                cout << endl << endl;
        }

        for(int i=0; i<c.size(); i++) cout << c[i] <<" ";
        cout << endl << endl;*/

   }while(c != c_old);

   // Escribir en salida

   cout << "Escribiendo datos en salida..." << endl;

    ofstream f("data/centroides.out");
        for(int i=0; i<centroides.size(); i++){
            for(int j=0; j<centroides[0].size(); j++){
                f << centroides[i][j];
                if(j != centroides[0].size()-1) f << ",";
            }
            f << endl;
        }
        f.close();
        string ruta;
        for(int i=0; i<k; i++){
            ruta = "data/cluster" + to_string(i);
            ruta = ruta + ".out";
            f.open(ruta, ios_base::out);
            for(int j=0; j<c.size(); j++){
                if(c[j] == i){
                    for(int l=0; l<centroides[0].size(); l++){
                            f << (*cl_set)[j][l];
                            if(l != centroides[0].size()-1) f << ",";
                    }
                    f << endl;
                }
            }
            f.close();
        }

}

void busqueda_local(vector<vector<float> >* cl_set, vector<vector<int> >* cl_set_const, int k, vector<int> seed){

    cout << "Creando solucion inicial aleatoria..." << endl;

    //Crear solución aleatoria
    
    vector<int> c;
    bool condicion;
    srand(seed[0]);
    do{
        for(int i=0; i<(*cl_set).size(); i++) c.push_back(rand()%k);
        condicion = count(c.begin(), c.end(), 0) == 0;
        for(int i=1; i<k && !condicion; i++) condicion || count(c.begin(), c.end(), i) == 0;
    }while(condicion);

    /*for(int i=0; i<c.size(); i++){
            cout << c[i] << " ";
    }
    cout << endl;*/

    //Infleasibility en lista
    vector<vector<int> > lista_const;
    vector<int> aux_lista;
    for(int i=0; i<(*cl_set).size();i++){
        for(int j=0; j<(*cl_set).size();j++){
            if((*cl_set_const)[i][j] == 1){
                aux_lista.clear();
                aux_lista.push_back(i);
                aux_lista.push_back(j);
                aux_lista.push_back(1);
                lista_const.push_back(aux_lista);
            }
            else if((*cl_set_const)[i][j] == -1){
                aux_lista.clear();
                aux_lista.push_back(i);
                aux_lista.push_back(j);
                aux_lista.push_back(-1);
                lista_const.push_back(aux_lista);
            }
        }
    }

    //Calculo de infesibility

    int infleasibility = 0;
    for(int i=0; i<c.size(); i++){
        for(int j=i; j<c.size(); j++){
            for(int l=0; l<lista_const.size();l++){
                if(lista_const[l][0] == i && lista_const[l][1] == j){
                    if(c[i] == c[j] && lista_const[l][2] == -1) infleasibility++;
                    if(c[i] != c[j] && lista_const[l][2] == 1) infleasibility++;
                }
            }
        }
    }

    //Calculo de centroides

    vector<vector<float> > centroides;
    vector<float> aux_centro;
    for(int i=0; i<(*cl_set)[0].size(); i++) aux_centro.push_back(0);
    for(int i=0; i<k; i++) centroides.push_back(aux_centro);

    vector<float> contador;
    for(int i=0; i<centroides.size(); i++) contador.push_back(0);
    for(int i=0; i<c.size(); i++) contador[c[i]]++;

    for(int i=0; i<c.size(); i++)
        for(int j=0; j<centroides[0].size(); j++)
            centroides[c[i]][j] += (*cl_set)[i][j];

    for(int i=0; i<centroides.size(); i++)
        for(int j=0; j<centroides[0].size(); j++)
            centroides[i][j] = contador[i];

    //Calculo de la distancia media intra-cluster

    vector<float> dist_intracl;
    for(int i=0; i<k; i++) dist_intracl.push_back(0);

    float aux_calc;
    for(int i=0; i<c.size();i++){
        aux_calc = 0;
        for(int j=0; j<centroides[0].size(); j++) aux_calc += pow(centroides[c[i]][j]-(*cl_set)[i][j],2);
        dist_intracl[c[i]] = sqrt(aux_calc);
    }
    for(int i=0; i<k; i++) dist_intracl[i] /= contador[i];

    //Calculo de la desviacion general

    float desv_general = 0;
    for(int i=0; i<k; i++) desv_general += dist_intracl[i];

    desv_general /= k;

    //Calculo de lambda

    float distancia_max = 0;
    for(int i=0; i<(*cl_set).size();i++){
        for(int j=i; j<(*cl_set).size();j++){
            aux_calc = 0;
            for(int l=0; l<(*cl_set)[0].size(); l++) aux_calc += pow((*cl_set)[c[i]][l]-(*cl_set)[i][l],2);
            aux_calc = sqrt(aux_calc);
            if(aux_calc > distancia_max) distancia_max = (int)aux_calc + 1;
        }
    }


    float lambda = distancia_max / (*cl_set).size();

    //Calculo de f inicial

    float funcion = desv_general + infleasibility * lambda;

    //cout << funcion << endl;

    cout << "Aplicando algoritmo de busqueda local..." << endl;

    //Buscamos el primer mejor vecino hasta que no encontremos ninguno mejor



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
        //srand(seed[0]);
        //Hay que barajar conjuntamente el conjunto y las restricciones
        copkm(&cl_set, &cl_set_const, k, seed);
    }
    else busqueda_local(&cl_set, &cl_set_const, k, seed);

    return 0;
}