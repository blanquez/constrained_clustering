#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <cmath>
#include <algorithm>
#include <time.h>
using namespace std;

float calcular_f(vector<vector<float> >* cl_set, vector<int> c, vector<vector<int> > lista_const, int k, float lambda, float* desviacion, int* infeas){
    //Calculo de infesibility

    int infeasibility = 0;

    for(int i=0; i<lista_const.size();i++){
        if(c[lista_const[i][0]] == c[lista_const[i][1]] && lista_const[i][2] == -1) infeasibility++;
        if(c[lista_const[i][0]] != c[lista_const[i][1]] && lista_const[i][2] == 1) infeasibility++;
    }

    (*infeas) = infeasibility;
    
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
            centroides[i][j] /= contador[i];

    //Calculo de la distancia media intra-cluster

    vector<float> dist_intracl;
    for(int i=0; i<k; i++) dist_intracl.push_back(0);

    float aux_calc;
    for(int i=0; i<c.size();i++){
        aux_calc = 0;
        for(int j=0; j<centroides[0].size(); j++) aux_calc += pow(centroides[c[i]][j]-(*cl_set)[i][j],2);
        dist_intracl[c[i]] += pow(aux_calc,0.5);
    }
    for(int i=0; i<k; i++) dist_intracl[i] /= contador[i];

    //Calculo de la desviacion general

    float desv_general = 0;
    for(int i=0; i<k; i++) desv_general += dist_intracl[i];

    desv_general /= k;

    (*desviacion) = desv_general;

    return (desv_general + infeasibility * lambda);
}


void copkm(vector<vector<float> >* cl_set, vector<vector<int> >* cl_set_const, int k, vector<int> seed){

    clock_t tiempo1, tiempo2;

    //Infeasibility en lista
    vector<vector<int> > lista_const;
    vector<int> aux_lista;
    for(int i=0; i<(*cl_set).size();i++){
        for(int j=i+1; j<(*cl_set).size();j++){
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

    tiempo1 = clock();
    
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
   vector<int> c;                   // Asignación instantánea de clusters
   vector<int> inf_actual;          // Infeasibility
   float minimo_cl, aux_calc, distancia=0;   // Minima infeasibility
   int minimo_ind, contador_fin=0;     // Indice de la minima infeasibility

    // Mientras cambien los clusters aplicamos el algoritmo
   do{
       //Control de clusters vacios
       bool condicion_vacio = true;
       if(!c_old.empty())
       for(int i=0;i<k;i++){
           if(count(c.begin(), c.end(), i) < 1){
               condicion_vacio = false;
               break;
           }
       }
       if(condicion_vacio) c_old = c;
       else{
           c = c_old;
           break;
       }
       //c.clear();
       //Introducir nodo en el mejor cluster
       for(int i=0; i<(*cl_set).size(); i++){
           int i2 = indices[i];
           inf_actual.clear();
           for(int j=0; j<k; j++) inf_actual.push_back(0);

            //Calcular infeasibility

            if(c_old.empty()){
                for(int j=0; j<c.size(); j++){
                    if((*cl_set_const)[i2][j] == -1 ) inf_actual[c[j]]++;
                    else if((*cl_set_const)[i2][j] == 1 ) for(int m=0; m<k; m++) if(m != c[j]) inf_actual[m]++;
                }
            }
            else{
                for(int j=0; j<k;j++){
                    c[i2] = j;
                    for(int l=0; l<lista_const.size();l++){
                        if(c[lista_const[l][0]] == c[lista_const[l][1]] && lista_const[l][2] == -1) inf_actual[j]++;
                        if(c[lista_const[l][0]] != c[lista_const[l][1]] && lista_const[l][2] == 1) inf_actual[j]++;
                    }
                }
            }

           /*for(int i=0; i<centroides.size(); i++){
                 cout << "Centroide " << i << ": ";
                for(int j=0; j<centroides[i].size(); j++) cout << centroides[i][j] << " ";
                cout << endl;
            }*/

           /*for(int j=0;j<inf_actual.size();j++){
               cout << "Inf " << j <<": " << inf_actual[j] << endl;
           }
           cout << endl;*/

            minimo_cl = inf_actual[0];
            minimo_ind=0;
            for(int j=0; j<centroides[0].size(); j++) distancia += pow(centroides[0][j]-(*cl_set)[i2][j],2);
            distancia = pow(distancia, 0.5);

           //Escoger cluster
           for(int j=1; j<k ; j++){
               aux_calc = 0;
               for(int l=0; l<centroides[0].size(); l++) aux_calc += pow(centroides[j][l]-(*cl_set)[i2][j],2);
               aux_calc = pow(aux_calc, 0.5);
               if((inf_actual[j] < minimo_cl) || ( (inf_actual[j] == minimo_cl) && (aux_calc < distancia) ) ){
                   minimo_cl = inf_actual[j];
                   minimo_ind = j;
                   distancia = aux_calc;
               }
           }
           
            if(c_old.empty()) c.push_back(minimo_ind);
            else c[i2] = minimo_ind;
       }

       //Reordeno por orden

        if(c_old.empty()){
            aux_baraja = c;
            for(int i=0; i<c.size(); i++) c[indices[i]] = aux_baraja[i];
        }

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

        /*for(int i=0; i<c.size(); i++) cout << c[i] << " ";
        cout << endl << endl;*/

   }while(c != c_old);

   tiempo2 = clock();

    float distancia_max = 0;
    for(int i=0; i<(*cl_set).size();i++){
        for(int j=i+1; j<(*cl_set).size();j++){
            aux_calc = 0;
            for(int l=0; l<(*cl_set)[0].size(); l++) aux_calc += pow((*cl_set)[c[i]][l]-(*cl_set)[i][l],2);
            aux_calc = pow(aux_calc, 0.5);
            if(aux_calc > distancia_max) distancia_max = (int)aux_calc + 1;
        }
    }

    float lambda = distancia_max / lista_const.size();

    float desviacion;
    int infeasibility;
    for(int i=0; i<c.size(); i++) cout << c[i] << " ";
        cout << endl << endl;
    cout << lambda << endl;
    float f = calcular_f(cl_set,c,lista_const,k,lambda,&desviacion,&infeasibility);

    cout << "f: " << f << endl;
    cout << "C/: " << desviacion << endl;
    cout << "inf: " << infeasibility << endl;
    cout << "tiempo: " << (float)tiempo2/CLOCKS_PER_SEC - (float)tiempo1/CLOCKS_PER_SEC;
    if(contador_fin == 5000) cout << "*";
    cout << endl;

   

   /*for(int i=0; i<centroides.size(); i++){
            cout << "Centroide " << i << ": ";
            for(int j=0; j<centroides[0].size(); j++) cout << centroides[i][j] << " ";
                cout << endl << endl;
        }

        for(int i=0; i<c.size(); i++) cout << c[i] <<" ";
        cout << endl << endl;*/

}

void busqueda_local(vector<vector<float> >* cl_set, vector<vector<int> >* cl_set_const, int k, vector<int> seed){

    time_t tiempo1, tiempo2;

    cout << "Creando solucion inicial aleatoria..." << endl;

    tiempo1 = clock();

    //Crear solución aleatoria
    
    vector<int> c;
    bool condicion;
    srand(seed[0]);
    do{
        c.clear();
        for(int i=0; i<(*cl_set).size(); i++) c.push_back(rand()%k);
        condicion = count(c.begin(), c.end(), 0) == 0;
        for(int i=1; i<k && !condicion; i++) condicion || count(c.begin(), c.end(), i) == 0;
    }while(condicion);

    /*for(int i=0; i<c.size(); i++){
            cout << c[i] << " ";
    }
    cout << endl;*/

    //Infeasibility en lista
    vector<vector<int> > lista_const;
    vector<int> aux_lista;
    for(int i=0; i<(*cl_set).size();i++){
        for(int j=i+1; j<(*cl_set).size();j++){
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

    //Calculo de lambda

    float distancia_max = 0, aux_calc;
    for(int i=0; i<(*cl_set).size();i++){
        for(int j=i+1; j<(*cl_set).size();j++){
            aux_calc = 0;
            for(int l=0; l<(*cl_set)[0].size(); l++) aux_calc += pow((*cl_set)[c[i]][l]-(*cl_set)[i][l],2);
            aux_calc = pow(aux_calc, 0.5);
            if(aux_calc > distancia_max) distancia_max = (int)aux_calc + 1;
        }
    }

    // Parámetro de relevancia
    int rel=8;

    float lambda = distancia_max*rel / lista_const.size();
    float desviacion, desv_temporal;
    int infeasibility, infe_temporal;

    //Calculo de f inicial

    float f_ini = calcular_f(cl_set,c,lista_const,k,lambda,&desviacion,&infeasibility);

    cout << "Aplicando algoritmo de busqueda local..." << endl;

    //Buscamos el primer mejor vecino hasta que no encontremos ninguno mejor

    float f_act = f_ini;
    int aux_func, contador = 0;
    vector<int> c_aux;

    cout << f_act << endl;

    do{
        f_ini = f_act;
        int aleatorio = rand();

        for(int i=0; i<c.size()*(k-1) && f_act>=f_ini ;i++){
            c_aux=c;
            aux_func = (i+aleatorio)%c.size();
            c_aux[aux_func] = (c_aux[aux_func] + (i/c.size()+1))%k;

            f_act = calcular_f(cl_set,c_aux,lista_const,k,lambda,&desv_temporal,&infe_temporal);
            contador++;
        }

        //cout << f_act << endl;

        if(f_act<f_ini){
            c = c_aux;
            infeasibility = infe_temporal;
            desviacion = desv_temporal;
        }

    }while(f_act<f_ini && contador < 100000);

    tiempo2 = clock();

    cout << "f final: " << f_ini << endl;
    cout << "c/: " << desviacion << endl;
    cout << "infeasibility: " << infeasibility << endl;
    cout << "tiempo: " << (float)tiempo2/CLOCKS_PER_SEC - (float)tiempo1/CLOCKS_PER_SEC << endl;
    
    for(int i=0;i<c.size();i++) cout << c[i] << " ";
    cout << endl;

}

int main(int argc, char* argv[]){

    //Lectura de datos de referencias por argumento o por teclado

    string ruta = "data/", restricciones = "data/";
    int k, algoritmo;
    vector<int> seed;

    if (argc == 6){
        ruta += argv[1];
        restricciones += argv[2];
        k = atoi(argv[3]);
        algoritmo = atoi(argv[4]);
        for(int i=0; i<k; i++) seed.push_back(atoi(argv[5])*(i+1));
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

    if(algoritmo==0) copkm(&cl_set, &cl_set_const, k, seed);
    else busqueda_local(&cl_set, &cl_set_const, k, seed);

    return 0;
}