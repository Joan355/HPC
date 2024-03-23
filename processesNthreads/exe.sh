#!/bin/bash

# Define el rango de números
rangos=(10 100 500 1000 1500 2000 2500 3000 3500 4000 4500 6000 7000 7500 8000)
#rangos=(10 100 500 1000)

# Archivo CSV para almacenar los tiempos


# Cabecera del archivo CSV
# Contador de repeticiones
repeticiones=0
rep_t=0
Ts=(8 16 32)
while [ $rep_t -lt 3 ]; do
    archivo_csv="tiempos${Ts[$rep_t]}.csv"
    echo "Valor,Tiempo" > "$archivo_csv"
    # Repetir el bucle al menos tres veces
    while [ $repeticiones -lt 3 ]; do
        # Iterar sobre cada valor en el rango definido
        for valor in "${rangos[@]}"; do
            # Ejecutar el programa con el valor actual y guardar la salida en una variable
            tiempo=$(./exdir/threads "$valor" "${Ts[$rep_t]}" | grep -o '[0-9.]\+')
            # Agregar el tiempo al archivo CSV junto con el valor actual
            echo "$valor,$tiempo" >> "$archivo_csv"
        done
        # Incrementar el contador de repeticiones
        ((repeticiones++))
    done
    repeticiones=0
    ((rep_t++))
done