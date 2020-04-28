///////////////////////////////////////////////////////////
//  PRÁCTICA 1 - TÉCNICAS DE BÚSQUEDA LOCAL Y ALGORITMOS //
//               GREEDY                                  //
//           +                                           //
//                                                       //
//  PRÁCTICA 2 - TÉCNICAS BASADAS EN POBLACIONES         //
//                                                       //
//  Antonio José Blánquez Pérez 3ºCSI UGR                //
//  METAHEURÍSTICAS                                      //
//  Grupo 2 - Jueves                                     //
///////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <cmath>
#include <algorithm>
#include <time.h>
using namespace std;

float calcular_f(vector<vector<float> >* cl_set, vector<int> c, vector<vector<int> > lista_const, int k, float lambda, float* desviacion, int* infeas){

    //Control de clusters vacíos

    bool condicion_vacio = true;

    for(int i=0;i<k;i++){
        if(count(c.begin(), c.end(), i) < 1){
            condicion_vacio = false;
            break;
        }
    }

    //Calculo de infesibility

    int infeasibility = 0;

    if(condicion_vacio){
        for(int i=0; i<lista_const.size();i++){
            if(c[lista_const[i][0]] == c[lista_const[i][1]] && lista_const[i][2] == -1) infeasibility++;
            if(c[lista_const[i][0]] != c[lista_const[i][1]] && lista_const[i][2] == 1) infeasibility++;
        }
    }else{
        infeasibility = 9999;
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

    // Aleatorizacion de acceso a las soluciones
    
    vector<int> indices;
    for(int i=0; i < (*cl_set).size(); i++) indices.push_back(i);
    srand(seed[0]);
    random_shuffle(indices.begin(), indices.end());
    
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

   // Aplicamos el algoritmo

   cout << "Aplicando algoritmo Greedy COPKM..." << endl;

   vector<int> c_old;                        // Solucion anterior
   vector<int> aux_baraja;                   // Vector auxiliar para reordenar solucion
   vector<int> c;                            // Solucion actual
   vector<int> inf_actual;                   // Infeasibility
   float minimo_cl, aux_calc, distancia=0;   // Variables auxiliares para la eleccion de cluster
   int minimo_ind;                           // Indice de la minima infeasibility

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

       // Calculo del mejor cluster para cada objeto

       for(int i=0; i<(*cl_set).size(); i++){
           int i2 = indices[i];
           inf_actual.clear();
           for(int j=0; j<k; j++) inf_actual.push_back(0);

            //Calculo de infeasibility

            // v1 /////////////////////////////////////////

            /*for(int j=0; j<c.size(); j++){
                if((*cl_set_const)[i2][j] == -1 ) inf_actual[c[j]]++;
                else if((*cl_set_const)[i2][j] == 1 ) for(int m=0; m<k; m++) if(m != c[j]) inf_actual[m]++;
            }*/

            // v2 /////////////////////////////////////////

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

            ///////////////////////////////////////////////

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

           // v1 ///////////////////////////////////////////////

           //c.push_back(minimo_ind)

           // v2 ///////////////////////////////////////////////
           
            if(c_old.empty()) c.push_back(minimo_ind);
            else c[i2] = minimo_ind;

           /////////////////////////////////////////////////////

       }

       // Reordenación la primera solucion
       // A partir de la primera no es necesario ya que no se introduce,
       // si no que se accede aleatoriamente

        // v1 ////////////////////////////////////////////////

        /*aux_baraja = c;
        for(int i=0; i<c.size(); i++) c[indices[i]] = aux_baraja[i];*/

        // v2 ////////////////////////////////////////////////

        if(c_old.empty()){
            aux_baraja = c;
            for(int i=0; i<c.size(); i++) c[indices[i]] = aux_baraja[i];
        }

        //////////////////////////////////////////////////////

       //Ajuste de centroides

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

    float desviacion;
    int infeasibility;

   }while(c != c_old);

   tiempo2 = clock();

    // Calculo de lambda

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

    // Calculo de la funcion objetivo

    float desviacion;
    int infeasibility;

    float f = calcular_f(cl_set,c,lista_const,k,lambda,&desviacion,&infeasibility);

    cout << "f: " << f << endl;
    cout << "C/: " << desviacion << endl;
    cout << "inf: " << infeasibility << endl;
    cout << "tiempo: " << (float)tiempo2/CLOCKS_PER_SEC - (float)tiempo1/CLOCKS_PER_SEC << endl;

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
            for(int l=0; l<(*cl_set)[0].size(); l++) aux_calc += pow((*cl_set)[i][l]-(*cl_set)[j][l],2);
            aux_calc = pow(aux_calc, 0.5);
            if(aux_calc > distancia_max) distancia_max = (int)aux_calc + 1;
        }
    }

    //Calculo de f inicial

    int rel=6;      // Parámetro de relevancia
    float lambda = distancia_max*rel / lista_const.size();
    float desviacion, desv_temporal;
    int infeasibility, infe_temporal;

    float f_ini = calcular_f(cl_set,c,lista_const,k,lambda,&desviacion,&infeasibility);

    //Busqueda del primer mejor vecino, hasta que no se encuentre ninguno mejor

    cout << "Aplicando algoritmo de busqueda local..." << endl;

    float f_act = f_ini;
    int aux_func, contador = 1;
    vector<int> c_aux;

    do{
        f_ini = f_act;

        // v1 /////////////////////////////////////////////

        /*int aleatorio = rand();

        // Exploracion del vecindario
        for(int i=0; i<c.size()*(k-1) && f_act>=f_ini ;i++){
            c_aux=c;
            aux_func = (i+aleatorio)%c.size();
            c_aux[aux_func] = (c_aux[aux_func] + (i/c.size()+1))%k;

            f_act = calcular_f(cl_set,c_aux,lista_const,k,lambda,&desv_temporal,&infe_temporal);
            contador++;
        }*/

        // v2 /////////////////////////////////////////////

        // Generacion del vecindario
        vector<vector<int> > vecinos;
        vector<int> aux_vecinos;
        for(int i=0;i<c.size();i++){
            for(int j=1;j<k;j++){
                aux_vecinos.clear();
                aux_vecinos.push_back(i);
                aux_vecinos.push_back( (c[i] + j) % k);
                vecinos.push_back(aux_vecinos);
            }
        }

        random_shuffle(vecinos.begin(), vecinos.end());

        // Exploracion del vecindario
        for(int i=0; i<vecinos.size() && f_act>=f_ini ;i++){
            c_aux=c;
            c_aux[vecinos[i][0]] = vecinos[i][1];
            f_act = calcular_f(cl_set,c_aux,lista_const,k,lambda,&desv_temporal,&infe_temporal);
            contador++;
        }

        ///////////////////////////////////////////////////

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

}

void genetico(vector<vector<float> >* cl_set, vector<vector<int> >* cl_set_const, int k, vector<int> seed, int tipo, int cruce){

    //Crear poblacion inicial aleatoria
    
    vector<int> c;
    vector<vector<int> > poblacion;
    bool condicion;
    srand(seed[0]);
    
    for(int j=0; j<50; j++){
        do{
            c.clear();
            for(int i=0; i<(*cl_set).size(); i++) c.push_back(rand()%k);
            condicion = count(c.begin(), c.end(), 0) == 0;
            for(int i=1; i<k && !condicion; i++) condicion || count(c.begin(), c.end(), i) == 0;
        }while(condicion);
        poblacion.push_back(c);
    }

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
            for(int l=0; l<(*cl_set)[0].size(); l++) aux_calc += pow((*cl_set)[i][l]-(*cl_set)[j][l],2);
            aux_calc = pow(aux_calc, 0.5);
            if(aux_calc > distancia_max) distancia_max = (int)aux_calc + 1;
        }
    }

    float lambda = distancia_max / lista_const.size();

    // Bucle principal

    int iteraciones = 0, infeasibility, indice, valor, aux, hijo1, hijo2, padre1, padre2, mutaciones = 0;
    float valorf, auxf, hijo1f, hijo2f, padre1f, padre2f;
    float desviacion;
    vector<int> indices, ind_cruces;
    vector<vector<int> > padres, hijos;

    time_t tiempo1, tiempo2;

    tiempo1 = clock();

    if(tipo == 0){              // Algoritmo genetico generacional
        if(cruce == 0){         // AGG - UN
            while(iteraciones<100000){

                // Eleccion de padres(poblacion.size())
                
                padres.clear();
                for(int i=0;i<poblacion.size();i++){
                    padre1 = rand()%poblacion.size();
                    padre2 = rand()%poblacion.size();
                    if(calcular_f(cl_set,poblacion[padre1],lista_const,k,lambda,&desviacion,&infeasibility) <= calcular_f(cl_set,poblacion[padre2],lista_const,k,lambda,&desviacion,&infeasibility))
                        padres.push_back(poblacion[padre1]);
                    else
                        padres.push_back(poblacion[padre2]);
                    iteraciones += 2;
                }

                // Cruces(Operador uniforme)

                hijos.clear();
                for(int i=1;i<padres.size()*0.7;i+=2){
                    hijos.push_back(padres[i-1]);
                    hijos.push_back(padres[i-1]);

                    for(int j=0;j<(padres[0].size()/2);j++){
                        valor = rand()%padres[0].size();
                        hijos[i-1][valor] = padres[i][valor];
                    }

                    for(int j=0;j<(padres[0].size()/2);j++){
                        valor = rand()%padres[0].size();
                        hijos[i][valor] = padres[i][valor];
                    }
                }

                
                // Mutacion

                mutaciones += hijos.size()*hijos[0].size();
                if(mutaciones >= 1000){
                    mutaciones = mutaciones % 1000;
                    indice = rand()%hijos.size();
                    valor = rand()%hijos[0].size();
                    hijos[indice][valor] = (hijos[indice][valor] + rand()%(k-1) + 1)%k;
                }

                // Reemplazamiento(con elitismo)

                valorf = calcular_f(cl_set,padres[0],lista_const,k,lambda,&desviacion,&infeasibility);
                indice = 0;
                iteraciones++;
                for(int i=1;i<poblacion.size();i++){
                    auxf = calcular_f(cl_set,poblacion[i],lista_const,k,lambda,&desviacion,&infeasibility);
                    iteraciones++;
                    if(auxf < valorf){
                        indice = i;
                        valorf = auxf;
                    }
                }
                c = poblacion[indice];
                poblacion.clear();
                poblacion.push_back(c);

                valorf = calcular_f(cl_set,hijos[0],lista_const,k,lambda,&desviacion,&infeasibility);
                indice = 0;
                iteraciones++;
                for(int i=1;i<hijos.size();i++){
                    auxf = calcular_f(cl_set,hijos[i],lista_const,k,lambda,&desviacion,&infeasibility);
                    iteraciones++;
                    if(auxf > valorf){
                        indice = i;
                        valorf = auxf;
                    }
                }
                hijos.erase(hijos.begin()+indice, hijos.begin()+indice+1);

                for(int i=0;i<hijos.size();i++) poblacion.push_back(hijos[i]);

                for(int i=padres.size()*0.7-1;i<padres.size();i++) poblacion.push_back(padres[i]);
            }
        }else{                  // AGG - SF
            while(iteraciones<100000){
                
                // Eleccion de padres(poblacion.size())

                padres.clear();
                for(int i=0;i<poblacion.size();i++){
                    padre1 = rand()%poblacion.size();
                    padre2 = rand()%poblacion.size();
                    if(calcular_f(cl_set,poblacion[padre1],lista_const,k,lambda,&desviacion,&infeasibility) <= calcular_f(cl_set,poblacion[padre2],lista_const,k,lambda,&desviacion,&infeasibility))
                        padres.push_back(poblacion[padre1]);
                    else
                        padres.push_back(poblacion[padre2]);
                    iteraciones += 2;
                }

                // Cruces(Operador de segmento finito)

                hijos.clear();
                for(int i=1;i<padres.size()*0.7;i+=2){
                    hijos.push_back(padres[i]);
                    hijos.push_back(padres[i]);

                    indice = rand()%padres[0].size();
                    valor = rand()%padres[0].size();

                    for(int j=indice;j<(indice+valor);j++) hijos[i-1][j%padres[0].size()] = padres[i-1][j%padres[0].size()];

                    indice = rand()%padres[0].size();
                    valor = rand()%padres[0].size();

                    for(int j=indice;j<(indice+valor);j++) hijos[i][j%padres[0].size()] = padres[i-1][j%padres[0].size()];
                
                    ind_cruces.clear();
                    for(int j=0;j<indice;j++) ind_cruces.push_back(j);
                    for(int j=indice+valor;j<padres[0].size();j++) ind_cruces.push_back(j);

                    random_shuffle(ind_cruces.begin(),ind_cruces.end());
                    for(int j=0;j<(ind_cruces.size()/2);j++){
                        valor = ind_cruces[j];
                        hijos[i-1][valor] = padres[i-1][valor];
                    }

                    random_shuffle(ind_cruces.begin(),ind_cruces.end());
                    for(int j=0;j<(ind_cruces.size()/2);j++){
                        valor = ind_cruces[j];
                        hijos[i][valor] = padres[i-1][valor];
                    }
                }

                // Mutacion

                mutaciones += hijos.size()*hijos[0].size();
                if(mutaciones >= 1000){
                    mutaciones = mutaciones % 1000;
                    indice = rand()%hijos.size();
                    valor = rand()%hijos[0].size();
                    hijos[indice][valor] = (hijos[indice][valor] + rand()%(k-1) + 1)%k;
                }

                // Reemplazamiento(con elitismo)

                valorf = calcular_f(cl_set,poblacion[0],lista_const,k,lambda,&desviacion,&infeasibility);
                indice = 0;
                iteraciones++;
                for(int i=1;i<padres.size();i++){
                    auxf = calcular_f(cl_set,poblacion[i],lista_const,k,lambda,&desviacion,&infeasibility);
                    iteraciones++;
                    if(auxf < valorf){
                        indice = i;
                        valorf = auxf;
                    }
                }
                c = poblacion[indice];
                poblacion.clear();
                poblacion.push_back(c);

                valorf = calcular_f(cl_set,hijos[0],lista_const,k,lambda,&desviacion,&infeasibility);
                indice = 0;
                iteraciones++;
                for(int i=1;i<hijos.size();i++){
                    auxf = calcular_f(cl_set,hijos[i],lista_const,k,lambda,&desviacion,&infeasibility);
                    iteraciones++;
                    if(aux > valor){
                        indice = i;
                        valorf = auxf;
                    }
                }
                hijos.erase(hijos.begin()+indice, hijos.begin()+indice+1);

                for(int i=0;i<hijos.size();i++) poblacion.push_back(hijos[i]);

                for(int i=padres.size()*0.7-1;i<padres.size();i++) poblacion.push_back(padres[i]);

            }
        }
    }else{                      // Algoritmo genetico estacionacionario
        if(cruce == 0){         // AGE - UN
           while(iteraciones<100000){

                // Eleccion de padres(Solo dos)

                padres.clear();
                for(int i=0;i<4;i++){
                    aux = rand()%poblacion.size();
                    padres.push_back(poblacion[aux]);
                    indices.push_back(aux);
                }

                if(calcular_f(cl_set,padres[0],lista_const,k,lambda,&desviacion,&infeasibility) <= calcular_f(cl_set,padres[1],lista_const,k,lambda,&desviacion,&infeasibility)){
                    padres.push_back(padres[0]);
                    indices.push_back(indices[0]);
                }
                else{
                    padres.push_back(padres[1]);
                    indices.push_back(indices[1]);
                }

                if(calcular_f(cl_set,padres[2],lista_const,k,lambda,&desviacion,&infeasibility) <= calcular_f(cl_set,padres[3],lista_const,k,lambda,&desviacion,&infeasibility)){
                    padres.push_back(padres[2]);
                    indices.push_back(indices[2]);
                }
                else{
                    padres.push_back(padres[3]);
                    indices.push_back(indices[3]);
                }
                iteraciones += 4;

                padres.erase(padres.begin(), padres.begin()+4);
                indices.erase(indices.begin(), indices.begin()+4);

                // Cruces(Operador uniforme)

                hijos.clear();
                hijos.push_back(padres[0]);
                hijos.push_back(padres[0]);

                ind_cruces.clear();
                for(int i=0;i<padres[0].size();i++) ind_cruces.push_back(i);

                random_shuffle(ind_cruces.begin(),ind_cruces.end());
                for(int i=0;i<(padres[0].size()/2);i++){
                    valor = ind_cruces[i];
                    hijos[0][valor] = padres[1][valor];
                }

                random_shuffle(ind_cruces.begin(),ind_cruces.end());
                for(int i=0;i<(padres[0].size()/2);i++){
                    valor = ind_cruces[i];
                    hijos[1][valor] = padres[1][valor];
                }

                // Mutacion

                mutaciones += 2*hijos[0].size();
                if(mutaciones >= 1000){
                    mutaciones = mutaciones % 1000;
                    indice = rand()%hijos.size();
                    valor = rand()%hijos[0].size();
                    hijos[indice][valor] = (hijos[indice][valor] + rand()%(k-1) + 1)%k;
                }
                
                // Reemplazamiento

                hijo1f = calcular_f(cl_set,hijos[0],lista_const,k,lambda,&desviacion,&infeasibility);
                hijo2f = calcular_f(cl_set,hijos[1],lista_const,k,lambda,&desviacion,&infeasibility);
                padre1f = calcular_f(cl_set,padres[0],lista_const,k,lambda,&desviacion,&infeasibility);
                padre2f = calcular_f(cl_set,padres[1],lista_const,k,lambda,&desviacion,&infeasibility);

                if((hijo1f < padre1f && hijo2f < padre2f) || (hijo1f < padre2f && hijo2f < padre1f)){
                    poblacion[indices[0]] = hijos[0];
                    poblacion[indices[1]] = hijos[1];
                }else if(hijo1f < padre1f) poblacion[indices[0]] = hijos[0];
                else if(hijo1f < padre2f) poblacion[indices[1]] = hijos[0];
                else if(hijo2f < padre1f) poblacion[indices[0]] = hijos[1];
                else if(hijo2f < padre2f) poblacion[indices[1]] = hijos[1];

                iteraciones += 4;

            }
        }else{                  // AGE - SF
            while(iteraciones<100000){
                
                // Eleccion de padres(Solo dos)

                padres.clear();
                for(int i=0;i<4;i++){
                    aux = rand()%poblacion.size();
                    padres.push_back(poblacion[aux]);
                    indices.push_back(aux);
                }

                if(calcular_f(cl_set,padres[0],lista_const,k,lambda,&desviacion,&infeasibility) <= calcular_f(cl_set,padres[1],lista_const,k,lambda,&desviacion,&infeasibility)){
                    padres.push_back(padres[0]);
                    indices.push_back(indices[0]);
                }
                else{
                    padres.push_back(padres[1]);
                    indices.push_back(indices[1]);
                }

                if(calcular_f(cl_set,padres[2],lista_const,k,lambda,&desviacion,&infeasibility) <= calcular_f(cl_set,padres[3],lista_const,k,lambda,&desviacion,&infeasibility)){
                    padres.push_back(padres[2]);
                    indices.push_back(indices[2]);
                }
                else{
                    padres.push_back(padres[3]);
                    indices.push_back(indices[3]);
                }
                iteraciones += 4;

                padres.erase(padres.begin(), padres.begin()+4);
                indices.erase(indices.begin(), indices.begin()+4);

                // Cruces(Operador de segmento finito)

                hijos.clear();
                hijos.push_back(padres[1]);
                hijos.push_back(padres[1]);

                indice = rand()%padres[0].size();
                valor = rand()%padres[0].size();

                for(int i=indice;i<(indice+valor);i++) hijos[0][i%padres[0].size()] = padres[0][i%padres[0].size()];

                indice = rand()%padres[0].size();
                valor = rand()%padres[0].size();

                for(int i=indice;i<(indice+valor);i++) hijos[1][i%padres[0].size()] = padres[0][i%padres[0].size()];

                ind_cruces.clear();
                    for(int j=0;j<indice;j++) ind_cruces.push_back(j);
                    for(int j=indice+valor;j<padres[0].size();j++) ind_cruces.push_back(j);

                    random_shuffle(ind_cruces.begin(),ind_cruces.end());
                    for(int j=0;j<(ind_cruces.size()/2);j++){
                        valor = ind_cruces[j];
                        hijos[0][valor] = padres[0][valor];
                    }

                    random_shuffle(ind_cruces.begin(),ind_cruces.end());
                    for(int j=0;j<(ind_cruces.size()/2);j++){
                        valor = ind_cruces[j];
                        hijos[1][valor] = padres[0][valor];
                    }

                // Mutacion

                mutaciones += 2*hijos[0].size();
                if(mutaciones >= 1000){
                    mutaciones = mutaciones % 1000;
                    indice = rand()%hijos.size();
                    valor = rand()%hijos[0].size();
                    hijos[indice][valor] = (hijos[indice][valor] + rand()%(k-1) + 1)%k;
                }

                // Reemplazamiento

                hijo1f = calcular_f(cl_set,hijos[0],lista_const,k,lambda,&desviacion,&infeasibility);
                hijo2f = calcular_f(cl_set,hijos[1],lista_const,k,lambda,&desviacion,&infeasibility);
                padre1f = calcular_f(cl_set,padres[0],lista_const,k,lambda,&desviacion,&infeasibility);
                padre2f = calcular_f(cl_set,padres[1],lista_const,k,lambda,&desviacion,&infeasibility);

                if((hijo1f < padre1f && hijo2f < padre2f) || (hijo1f < padre2f && hijo2f < padre1f)){
                    poblacion[indices[0]] = hijos[0];
                    poblacion[indices[1]] = hijos[1];
                }else if(hijo1f < padre1f) poblacion[indices[0]] = hijos[0];
                else if(hijo1f < padre2f) poblacion[indices[1]] = hijos[0];
                else if(hijo2f < padre1f) poblacion[indices[0]] = hijos[1];
                else if(hijo2f < padre2f) poblacion[indices[1]] = hijos[1];

                iteraciones += 4;

            }
        }
    }

    tiempo2 = clock();

    // Busqueda de la mejor solucion en la poblacion

    valorf = calcular_f(cl_set,poblacion[0],lista_const,k,lambda,&desviacion,&infeasibility);
    indice = 0;
    for(int i=1;i<poblacion.size();i++){
        auxf = calcular_f(cl_set,poblacion[i],lista_const,k,lambda,&desviacion,&infeasibility);
        if(auxf < valorf){
            indice = i;
            valorf = auxf;
        }
    }

    cout << desviacion << " ";
    cout << infeasibility << " ";
    cout << valorf << " ";
    cout << tiempo2 - tiempo1 << endl;
}

int bl_suave(vector<int>* cromosoma, int k, int max_fallos, vector<vector<float> >* conjunto, vector<vector<int> > lista, float lambda){

    vector<int> indices, vecino;
    for(int i=0;i<(*cromosoma).size();i++) indices.push_back(i);
    random_shuffle(indices.begin(),indices.end());
    int fallos = 0, iters = 0, max_iters=(*cromosoma).size(), eval = 1, nada;
    float nada2, mejorf = calcular_f(conjunto,(*cromosoma),lista,k,lambda,&nada2,&nada), efe;
    bool mejora = true;

    while((mejora || fallos < max_fallos) && iters < max_iters){
        mejora = false;

        for(int i=1;i<k-1;i++){
            vecino = (*cromosoma);
            vecino[indices[iters]] = (vecino[indices[iters]] + i)%k;
            efe = calcular_f(conjunto,vecino,lista,k,lambda,&nada2,&nada);
            eval++;
            if(efe<mejorf){
                mejorf = efe;
                (*cromosoma) = vecino;
                mejora=true;
            }
        }

        if(!mejora) fallos++;

        iters++;
    }

    return eval;
}

void memetico(vector<vector<float> >* cl_set, vector<vector<int> >* cl_set_const, int k, vector<int> seed, int ratio, bool mejor){
    
    //Crear poblacion inicial aleatoria
    
    vector<int> c;
    vector<vector<int> > poblacion;
    bool condicion;
    srand(seed[0]);
    
    for(int j=0; j<10; j++){
        do{
            c.clear();
            for(int i=0; i<(*cl_set).size(); i++) c.push_back(rand()%k);
            condicion = count(c.begin(), c.end(), 0) == 0;
            for(int i=1; i<k && !condicion; i++) condicion || count(c.begin(), c.end(), i) == 0;
        }while(condicion);
        poblacion.push_back(c);
    }

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
            for(int l=0; l<(*cl_set)[0].size(); l++) aux_calc += pow((*cl_set)[i][l]-(*cl_set)[j][l],2);
            aux_calc = pow(aux_calc, 0.5);
            if(aux_calc > distancia_max) distancia_max = (int)aux_calc + 1;
        }
    }

    float lambda = distancia_max / lista_const.size();

    // Bucle principal

    int iteraciones = 0, infeasibility, indice, valor, aux, hijo1, hijo2, padre1, padre2, mutaciones = 0;
    float valorf, auxf, hijo1f, hijo2f, padre1f, padre2f;
    float desviacion;
    vector<int> indices, ind_cruces;
    vector<vector<int> > padres, hijos;

    time_t tiempo1, tiempo2;

    tiempo1 = clock();
    
    while(iteraciones<100000){

        // Eleccion de padres(poblacion.size())
                
        padres.clear();
        for(int i=0;i<poblacion.size();i++){
            padre1 = rand()%poblacion.size();
            padre2 = rand()%poblacion.size();
            if(calcular_f(cl_set,poblacion[padre1],lista_const,k,lambda,&desviacion,&infeasibility) <= calcular_f(cl_set,poblacion[padre2],lista_const,k,lambda,&desviacion,&infeasibility))
                padres.push_back(poblacion[padre1]);
            else
                padres.push_back(poblacion[padre2]);
            iteraciones += 2;
        }

        // Cruces(Operador uniforme)

        hijos.clear();
        for(int i=1;i<padres.size()*0.7;i+=2){
            hijos.push_back(padres[i-1]);
            hijos.push_back(padres[i-1]);

            for(int j=0;j<(padres[0].size()/2);j++){
                valor = rand()%padres[0].size();
                hijos[i-1][valor] = padres[i][valor];
            }

            for(int j=0;j<(padres[0].size()/2);j++){
                valor = rand()%padres[0].size();
                hijos[i][valor] = padres[i][valor];
            }
        }

                
        // Mutacion

        mutaciones += hijos.size()*hijos[0].size();
        if(mutaciones >= 1000){
            mutaciones = mutaciones % 1000;
            indice = rand()%hijos.size();
            valor = rand()%hijos[0].size();
            hijos[indice][valor] = (hijos[indice][valor] + rand()%(k-1) + 1)%k;
        }

        // Reemplazamiento(con elitismo)

        valorf = calcular_f(cl_set,padres[0],lista_const,k,lambda,&desviacion,&infeasibility);
        indice = 0;
        iteraciones++;
        for(int i=1;i<poblacion.size();i++){
            auxf = calcular_f(cl_set,poblacion[i],lista_const,k,lambda,&desviacion,&infeasibility);
            iteraciones++;
            if(auxf < valorf){
                indice = i;
                valorf = auxf;
            }
        }
        c = poblacion[indice];
        poblacion.clear();
        poblacion.push_back(c);

        valorf = calcular_f(cl_set,hijos[0],lista_const,k,lambda,&desviacion,&infeasibility);
        indice = 0;
        iteraciones++;
        for(int i=1;i<hijos.size();i++){
            auxf = calcular_f(cl_set,hijos[i],lista_const,k,lambda,&desviacion,&infeasibility);
            iteraciones++;
            if(auxf > valorf){
                indice = i;
                valorf = auxf;
            }
        }
        hijos.erase(hijos.begin()+indice, hijos.begin()+indice+1);

        for(int i=0;i<hijos.size();i++) poblacion.push_back(hijos[i]);

        for(int i=padres.size()*0.7-1;i<padres.size();i++) poblacion.push_back(padres[i]);

        // BLS

        if(mejor){
            valorf = calcular_f(cl_set,poblacion[0],lista_const,k,lambda,&desviacion,&infeasibility);
            indice = 0;
            iteraciones++;
            for(int i=1;i<poblacion.size();i++){
                auxf = calcular_f(cl_set,poblacion[i],lista_const,k,lambda,&desviacion,&infeasibility);
                iteraciones++;
                if(auxf < valorf){
                    indice = i;
                    valorf = auxf;
                }
            }

            iteraciones += bl_suave(&poblacion[indice],k,(int)(poblacion[0].size()*0.1),cl_set,lista_const,lambda);
        }else{
            indices.clear();
            for(int i=0;i<poblacion.size();i++) indices.push_back(i);
            random_shuffle(indices.begin(),indices.end());

            for(int i=0;i<poblacion.size()*ratio;i++) iteraciones += bl_suave(&poblacion[indices[i]],k,(int)(poblacion[0].size()*0.1),cl_set,lista_const,lambda);
        }
    }

    tiempo2 = clock();

    // Busqueda de la mejor solucion en la poblacion

    valorf = calcular_f(cl_set,poblacion[0],lista_const,k,lambda,&desviacion,&infeasibility);
    indice = 0;
    for(int i=1;i<poblacion.size();i++){
        auxf = calcular_f(cl_set,poblacion[i],lista_const,k,lambda,&desviacion,&infeasibility);
        if(auxf < valorf){
            indice = i;
            valorf = auxf;
        }
    }

    cout << desviacion << " ";
    cout << infeasibility << " ";
    cout << valorf << " ";
    cout << tiempo2 - tiempo1 << endl;
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
        cout << "Introduzca el algoritmo deseado: ";
        cin >> algoritmo;
        cout << "Introduzca el número de clusters que se crearán: ";
        cin >> k;
        cout << "Introduzca tantas semillas como clusters para la generación de números aleatorios: ";
        seed.push_back(0);
        cin >> seed[0];
        for(int i=0; i<k; i++) seed.push_back(seed[0] * (i+1));
    }

    if(k<1){
        cout << "El número de clusters debe ser positivo" << endl;
        return 0;
    }

    //cout << "Cargando datos..." << endl;

    //Calculo de dimensiones

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
    else if(algoritmo==1) busqueda_local(&cl_set, &cl_set_const, k, seed);
    else if(algoritmo==2) genetico(&cl_set, &cl_set_const, k, seed, 0, 0);
    else if(algoritmo==3) genetico(&cl_set, &cl_set_const, k, seed, 0, 1);
    else if(algoritmo==4) genetico(&cl_set, &cl_set_const, k, seed, 1, 0);
    else if(algoritmo==5) genetico(&cl_set, &cl_set_const, k, seed, 1, 1);
    else if(algoritmo==6) memetico(&cl_set, &cl_set_const, k, seed, 1, false);
    else if(algoritmo==7) memetico(&cl_set, &cl_set_const, k, seed, 0.1, false);
    else if(algoritmo==8) memetico(&cl_set, &cl_set_const, k, seed, 0.1, true);

    return 0;
}