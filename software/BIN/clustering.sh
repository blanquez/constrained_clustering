#!/bin/bash
if [ $# -eq 1 ]; then
    declare -a seeds=('798245613' '123456789' '25022020' '17042026' '459268694')
	for i in "${seeds[@]}"
	do
	echo -e "\n$i:\n"
        if [ $1 == "aggun" ]; then
		    ./clustering iris_set.dat iris_set_const_10.const 3 2 $i
		    ./clustering newthyroid_set.dat newthyroid_set_const_10.const 3 2 $i
		    ./clustering ecoli_set.dat ecoli_set_const_10.const 8 2 $i
		    ./clustering rand_set.dat rand_set_const_10.const 3 2 $i
		    echo
		    ./clustering iris_set.dat iris_set_const_20.const 3 2 $i
		    ./clustering newthyroid_set.dat newthyroid_set_const_10.const 3 2 $i
		    ./clustering ecoli_set.dat ecoli_set_const_20.const 8 2 $i
		    ./clustering rand_set.dat rand_set_const_20.const 3 2 $i
	
	elif [ $1 == "copkm" ]; then
            	    ./clustering iris_set.dat iris_set_const_10.const 3 0 $i
		    ./clustering newthyroid_set.dat newthyroid_set_const_10.const 3 0 $i
		    ./clustering ecoli_set.dat ecoli_set_const_10.const 8 0 $i
		    ./clustering rand_set.dat rand_set_const_10.const 3 0 $i
echo
		    ./clustering iris_set.dat iris_set_const_20.const 3 0 $i
		    ./clustering newthyroid_set.dat newthyroid_set_const_20.const 3 0 $i
		    ./clustering ecoli_set.dat ecoli_set_const_20.const 8 0 $i
		    ./clustering rand_set.dat rand_set_const_20.const 3 0 $i

	elif [ $1 == "bl" ]; then
            	    ./clustering iris_set.dat iris_set_const_10.const 3 1 $i
		    ./clustering newthyroid_set.dat newthyroid_set_const_10.const 3 1 $i
		    ./clustering ecoli_set.dat ecoli_set_const_10.const 8 1 $i
		    ./clustering rand_set.dat rand_set_const_10.const 3 1 $i
echo
		    ./clustering iris_set.dat iris_set_const_20.const 3 1 $i
		    ./clustering newthyroid_set.dat newthyroid_set_const_20.const 3 1 $i
		    ./clustering ecoli_set.dat ecoli_set_const_20.const 8 1 $i
		    ./clustering rand_set.dat rand_set_const_20.const 3 1 $i


        elif [ $1 == "aggsf" ]; then
            	    ./clustering iris_set.dat iris_set_const_10.const 3 3 $i
		    ./clustering newthyroid_set.dat newthyroid_set_const_10.const 3 3 $i
		    ./clustering ecoli_set.dat ecoli_set_const_10.const 8 3 $i
		    ./clustering rand_set.dat rand_set_const_10.const 3 3 $i
echo
		    ./clustering iris_set.dat iris_set_const_20.const 3 3 $i
		    ./clustering newthyroid_set.dat newthyroid_set_const_20.const 3 3 $i
		    ./clustering ecoli_set.dat ecoli_set_const_20.const 8 3 $i
		    ./clustering rand_set.dat rand_set_const_20.const 3 3 $i

	elif [ $1 == "ageun" ]; then
            	    ./clustering iris_set.dat iris_set_const_10.const 3 4 $i
		    ./clustering newthyroid_set.dat newthyroid_set_const_10.const 3 4 $i
		    ./clustering ecoli_set.dat ecoli_set_const_10.const 8 4 $i
		    ./clustering rand_set.dat rand_set_const_10.const 3 4 $i
echo
		    ./clustering iris_set.dat iris_set_const_20.const 3 4 $i
		    ./clustering newthyroid_set.dat newthyroid_set_const_20.const 3 4 $i
		    ./clustering ecoli_set.dat ecoli_set_const_20.const 8 4 $i
		    ./clustering rand_set.dat rand_set_const_20.const 3 4 $i

	elif [ $1 == "agesf" ]; then
            	    ./clustering iris_set.dat iris_set_const_10.const 3 5 $i
		    ./clustering newthyroid_set.dat newthyroid_set_const_10.const 3 5 $i
		    ./clustering ecoli_set.dat ecoli_set_const_10.const 8 5 $i
		    ./clustering rand_set.dat rand_set_const_10.const 3 5 $i
echo
		    ./clustering iris_set.dat iris_set_const_20.const 3 5 $i
		    ./clustering newthyroid_set.dat newthyroid_set_const_20.const 3 5 $i
		    ./clustering ecoli_set.dat ecoli_set_const_20.const 8 5 $i
		    ./clustering rand_set.dat rand_set_const_20.const 3 5 $i

	elif [ $1 == "am1" ]; then
            	    ./clustering iris_set.dat iris_set_const_10.const 3 6 $i
		    ./clustering newthyroid_set.dat newthyroid_set_const_10.const 3 6 $i
		    ./clustering ecoli_set.dat ecoli_set_const_10.const 8 6 $i
		    ./clustering rand_set.dat rand_set_const_10.const 3 6 $i
echo
		    ./clustering iris_set.dat iris_set_const_20.const 3 6 $i
		    ./clustering newthyroid_set.dat newthyroid_set_const_20.const 3 6 $i
		    ./clustering ecoli_set.dat ecoli_set_const_20.const 8 6 $i
		    ./clustering rand_set.dat rand_set_const_20.const 3 6 $i

	elif [ $1 == "am0.1" ]; then
            	    ./clustering iris_set.dat iris_set_const_10.const 3 7 $i
		    ./clustering newthyroid_set.dat newthyroid_set_const_10.const 3 7 $i
		    ./clustering ecoli_set.dat ecoli_set_const_10.const 8 7 $i
		    ./clustering rand_set.dat rand_set_const_10.const 3 7 $i
echo
		    ./clustering iris_set.dat iris_set_const_20.const 3 7 $i
		    ./clustering newthyroid_set.dat newthyroid_set_const_20.const 3 7 $i
		    ./clustering ecoli_set.dat ecoli_set_const_20.const 8 7 $i
		    ./clustering rand_set.dat rand_set_const_20.const 3 7 $i

	elif [ $1 == "am0.1mej" ]; then
            	    ./clustering iris_set.dat iris_set_const_10.const 3 8 $i
		    ./clustering newthyroid_set.dat newthyroid_set_const_10.const 3 10 $i
		    ./clustering ecoli_set.dat ecoli_set_const_10.const 8 8 $i
		    ./clustering rand_set.dat rand_set_const_10.const 3 8 $i
echo
		    ./clustering iris_set.dat iris_set_const_20.const 3 8 $i
		    ./clustering newthyroid_set.dat newthyroid_set_const_20.const 3 10 $i
		    ./clustering ecoli_set.dat ecoli_set_const_20.const 8 8 $i
		    ./clustering rand_set.dat rand_set_const_20.const 3 8 $i

	elif [ $1 == "bmb" ]; then
            	    ./clustering iris_set.dat iris_set_const_10.const 3 10 $i
		    ./clustering newthyroid_set.dat newthyroid_set_const_10.const 3 10 $i
		    ./clustering ecoli_set.dat ecoli_set_const_10.const 8 10 $i
		    ./clustering rand_set.dat rand_set_const_10.const 3 10 $i
echo
		    ./clustering iris_set.dat iris_set_const_20.const 3 10 $i
		    ./clustering newthyroid_set.dat newthyroid_set_const_20.const 3 10 $i
		    ./clustering ecoli_set.dat ecoli_set_const_20.const 8 10 $i
		    ./clustering rand_set.dat rand_set_const_20.const 3 10 $i

	elif [ $1 == "ils" ]; then
            	    ./clustering iris_set.dat iris_set_const_10.const 3 11 $i
		    ./clustering newthyroid_set.dat newthyroid_set_const_10.const 3 11 $i
		    ./clustering ecoli_set.dat ecoli_set_const_10.const 8 11 $i
		    ./clustering rand_set.dat rand_set_const_10.const 3 11 $i
echo
		    ./clustering iris_set.dat iris_set_const_20.const 3 11 $i
		    ./clustering newthyroid_set.dat newthyroid_set_const_20.const 3 11 $i
		    ./clustering ecoli_set.dat ecoli_set_const_20.const 8 11 $i
		    ./clustering rand_set.dat rand_set_const_20.const 3 11 $i

	elif [ $1 == "es" ]; then
            	    ./clustering iris_set.dat iris_set_const_10.const 3 9 $i
		    ./clustering newthyroid_set.dat newthyroid_set_const_10.const 3 9 $i
		    ./clustering ecoli_set.dat ecoli_set_const_10.const 8 9 $i
		    ./clustering rand_set.dat rand_set_const_10.const 3 9 $i
echo
		    ./clustering iris_set.dat iris_set_const_20.const 3 9 $i
		    ./clustering newthyroid_set.dat newthyroid_set_const_20.const 3 9 $i
		    ./clustering ecoli_set.dat ecoli_set_const_20.const 8 9 $i
		    ./clustering rand_set.dat rand_set_const_20.const 3 9 $i

	elif [ $1 == "ilses" ]; then
            	    ./clustering iris_set.dat iris_set_const_10.const 3 12 $i
		    ./clustering newthyroid_set.dat newthyroid_set_const_10.const 3 12 $i
		    ./clustering ecoli_set.dat ecoli_set_const_10.const 8 12 $i
		    ./clustering rand_set.dat rand_set_const_10.const 3 12 $i
echo
		    ./clustering iris_set.dat iris_set_const_20.const 3 12 $i
		    ./clustering newthyroid_set.dat newthyroid_set_const_20.const 3 12 $i
		    ./clustering ecoli_set.dat ecoli_set_const_20.const 8 12 $i
		    ./clustering rand_set.dat rand_set_const_20.const 3 12 $i

        else
            echo "Algoritmo no valido"
        fi        
	done

else
    echo "Debes pasar como parametro el algoritmo a utilizar [bl/copkm]"
fi
