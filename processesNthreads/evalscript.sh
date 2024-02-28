#!/bin/bash

# Define el rango de números
rangos=(10 100 1000 2000 4000 6000 8000)

# Archivo CSV para almacenar los tiempos
archivo_csv="tiempos.csv"

# Cabecera del archivo CSV
echo "Valor,Tiempo" > "$archivo_csv"

# Contador de repeticiones
repeticiones=0

# Repetir el bucle al menos tres veces
while [ $repeticiones -lt 3 ]; do
    # Iterar sobre cada valor en el rango definido
    for valor in "${rangos[@]}"; do
        # Ejecutar el programa con el valor actual y guardar la salida en una variable
        tiempo=$(./exdir/processx "$valor" | grep -o '[0-9.]\+')
        # Agregar el tiempo al archivo CSV junto con el valor actual
        echo "$valor,$tiempo" >> "$archivo_csv"
    done
    # Incrementar el contador de repeticiones
    ((repeticiones++))
done
