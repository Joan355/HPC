#!/bin/bash

# Define el rango de números
rangos=(10 100 500 1000 1500 2000 2500 3000 3500 4000 4500 6000 7000 7500 8000)
#rangos=(10 100 500 1000)

# Archivo CSV para almacenar los tiempos


# Cabecera del archivo CSV

Ps=(4 2)
# Contador de repeticiones
repeticiones=0
rep_p=0

while [ $rep_p -lt 2 ]; do
archivo_csv="tiempos${Ps[$rep_p]}.csv"
echo "Valor,Tiempo" > "$archivo_csv"
# Repetir el bucle al menos tres veces
    while [ $repeticiones -lt 3 ]; do
        # Iterar sobre cada valor en el rango definido
        for valor in "${rangos[@]}"; do
            # Ejecutar el programa con el valor actual y guardar la salida en una variable
            tiempo=$(./exdir/processx "$valor" "${Ps[$rep_p]}" | grep -o '[0-9.]\+')
            # Agregar el tiempo al archivo CSV junto con el valor actual
            echo "$valor,$tiempo" >> "$archivo_csv"
        done
        # Incrementar el contador de repeticiones
        ((repeticiones++))
    done
    repeticiones=0
    ((rep_p++))
done
