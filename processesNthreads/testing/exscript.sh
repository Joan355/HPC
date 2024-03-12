#!/bin/bash

# Nombre del programa de origen
PROGRAMA="processes.c"

gcc -o1 $PROGRAMA -o "./optdir/exec/processx0"
gcc -o2 $PROGRAMA -o "./optdir/exec/processx1"
gcc -o3 $PROGRAMA -o "./optdir/exec/processx2"

rango=(500 1000)
# Nombre del archivo ejecutable


# Verificación de si la compilación fue exitosa

for ((i=0; i<3; i++)); do
    archivo="./optdir/times/tiempo${i}.csv"
    echo "Valor,Tiempo" > "$archivo"
    for rango in "${rangos[@]}"; do
        tiempo=$(./optdir/exec/processx${i} "$rango" | grep -o '[0-9.]\+')
        echo "$rango,$tiempo" >> "./optdir/times/tiempo${i}.csv"
    done 
done
