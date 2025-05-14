import os
import random


NUM_ARCHIVOS = 10
TAMANIO_ARCHIVO_BYTES = 2 * 1024 * 1024 * 1024 
ARCHIVO_BASE = "palabras_base.txt"


with open(ARCHIVO_BASE, "r", encoding="utf-8") as f:
    palabras = [line.strip() for line in f if line.strip()]


os.makedirs("outputs_split", exist_ok=True)


for i in range(NUM_ARCHIVOS):
    nombre_archivo = f"outputs_split/doc{i+1}.txt"
    with open(nombre_archivo, "w", encoding="utf-8") as salida:
        bytes_escritos = 0
        while bytes_escritos < TAMANIO_ARCHIVO_BYTES:
            linea = " ".join(random.choices(palabras, k=100)) + "\n"
            salida.write(linea)
            bytes_escritos += len(linea.encode("utf-8"))

    print(f"{nombre_archivo} generado ({bytes_escritos // (1024 * 1024)} MB)")
