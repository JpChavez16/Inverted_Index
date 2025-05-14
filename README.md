# 🔍 Índices Invertidos con Paralelismo: C++ (OpenMP) vs Rust (Rayon)

Este proyecto compara el rendimiento y la implementación de un **índice invertido** utilizando dos lenguajes modernos: **C++ con OpenMP** y **Rust con Rayon**. Ambos códigos procesan múltiples archivos de texto (simulados como documentos) para mapear palabras a sus respectivos archivos (documentos de origen).

---

## 📦 ¿Qué hace este proyecto?

- Procesa **10 archivos de texto** (~2GB en total).
- Limpia cada palabra (remueve caracteres especiales, convierte a minúsculas).
- Construye un **índice invertido** que asocia cada palabra con los documentos donde aparece.
- Mide el tiempo de ejecución para comparar eficiencia entre C++ y Rust.

---

## ⚙️ Tecnologías usadas

| Lenguaje | Librerías         | Paralelismo         | Formato de salida             |
|----------|-------------------|----------------------|-------------------------------|
| C++      | OpenMP            | `#pragma omp`        | `indice_invertido.txt`        |
| Rust     | rayon, std::sync  | `.par_iter()` + `Mutex` | `indice_invertido_rust.txt` |

---

## 🧪 Estructura del proyecto

├── cpp/
│   └── indice_invertido.cpp        # Código fuente en C++ con OpenMP
├── rust/
│   └── src/main.rs                 # Código fuente en Rust con Rayon
├── outputs_split/
│   ├── doc1.txt ... doc10.txt      # Archivos de entrada (simulan documentos)
├── indice_invertido.txt           # Salida C++
├── indice_invertido_rust.txt      # Salida Rust
└── README.md                      # Este documento



---

## ⌛ Tiempos de Ejecución (con 10 archivos)

| Implementación | Tiempo estimado |
|----------------|-----------------|
| C++ OpenMP     | ~10 minutos     |
| Rust Rayon     | < 1 minuto      |

> ⚠️ Las mediciones dependen del procesador y del sistema de archivos.

---

## 🧼 Limpieza y Preprocesamiento

Ambas versiones realizan una limpieza básica:

- Eliminar caracteres no alfanuméricos.
- Convertir todo a minúsculas.
- Tokenizar por espacios.

---

## 🧵 Paralelismo

- En **C++**, cada archivo es procesado en paralelo usando `#pragma omp parallel for`, cada uno generando un índice local que luego se fusiona.
- En **Rust**, los archivos se procesan con `par_iter` de `rayon`, y el índice global es protegido con `Mutex`.

---

## 🚀 Requisitos para compilar

### 🔧 C++
```bash
g++ -std=c++17 -fopenmp -O3 indice_invertido.cpp -o invertido
./invertido
