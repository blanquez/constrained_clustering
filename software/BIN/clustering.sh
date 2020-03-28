#!/bin/bash
if [ $# -eq 1 ]; then
    declare -a seeds=('798245613' '123456789' '25022020' '17042026' '459268694')
	for i in "${seeds[@]}"
	do
        if [ $1 == "bl" ]; then
            echo -e "\nIris 10% $i\n"
		    ./clustering iris_set.dat iris_set_const_10.const 3 1 $i
            echo -e "\nIris 20% $i\n"
		    ./clustering iris_set.dat iris_set_const_20.const 3 1 $i
            echo -e "\nEcoli 10% $i\n"
		    ./clustering ecoli_set.dat ecoli_set_const_10.const 8 1 $i
            echo -e "\nEcoli 20% $i\n"
		    ./clustering ecoli_set.dat ecoli_set_const_20.const 8 1 $i
            echo -e "\nRand 10% $i\n"
		    ./clustering rand_set.dat rand_set_const_10.const 3 1 $i
            echo -e "\nRand 20% $i\n"
		    ./clustering rand_set.dat rand_set_const_20.const 3 1 $i

        elif [ $1 == "copkm" ]; then
            echo -e "\nIris 10% $i\n"
		    ./clustering iris_set.dat iris_set_const_10.const 3 0 $i
            echo -e "\nIris 20% $i\n"
		    ./clustering iris_set.dat iris_set_const_20.const 3 0 $i
            echo -e "\nEcoli 10% $i\n"
		    ./clustering ecoli_set.dat ecoli_set_const_10.const 8 0 $i
            echo -e "\nEcoli 20% $i\n"
		    ./clustering ecoli_set.dat ecoli_set_const_20.const 8 0 $i
            echo -e "\nRand 10% $i\n"
		    ./clustering rand_set.dat rand_set_const_10.const 3 0 $i
            echo -e "\nRand 20% $i\n"
		    ./clustering rand_set.dat rand_set_const_20.const 3 0 $i

        else
            echo "Algoritmo no valido"
        fi        
	done

else
    echo "Debes pasar como parametro el algoritmo a utilizar [bl/copkm]"
fi
