#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <sstream>
#include <cctype>
#include <omp.h>
#include <chrono>

using namespace std;

const int BUFFER_SIZE = 64 * 1024;

string limpiar_palabra(const string& palabra) {
    string limpia;
    for (char c : palabra) {
        if (isalnum(c)) limpia += tolower(c);
    }
    return limpia;
}

void procesar_documento(const string& filename, int doc_id,
                        unordered_map<string, unordered_set<int>>& indice_local) {
    ifstream archivo(filename, ios::binary);
    if (!archivo.is_open()) return;

    char buffer[BUFFER_SIZE];
    string contenido, palabra;

    while (archivo.read(buffer, sizeof(buffer)) || archivo.gcount()) {
        contenido.append(buffer, archivo.gcount());

        size_t pos = 0;
        while ((pos = contenido.find_first_of(" \n\t")) != string::npos) {
            palabra = limpiar_palabra(contenido.substr(0, pos));
            if (!palabra.empty()) {
                indice_local[palabra].insert(doc_id);
            }
            contenido.erase(0, pos + 1);
        }
    }
}

unordered_map<string, unordered_set<int>> combinar_indices(
        const vector<unordered_map<string, unordered_set<int>>>& indices) {
    unordered_map<string, unordered_set<int>> resultado;
    for (const auto& indice : indices) {
        for (const auto& par : indice) {
            resultado[par.first].insert(par.second.begin(), par.second.end());
        }
    }
    return resultado;
}

int main() {
    vector<string> documentos = {
        "outputs_split/doc1.txt", "outputs_split/doc2.txt",
        "outputs_split/doc3.txt", "outputs_split/doc4.txt",
        "outputs_split/doc5.txt", "outputs_split/doc6.txt",
        "outputs_split/doc7.txt", "outputs_split/doc8.txt",
        "outputs_split/doc9.txt", "outputs_split/doc10.txt"
    };

    int NUM_THREADS = omp_get_max_threads();
    vector<unordered_map<string, unordered_set<int>>> indices_locales(documentos.size());

    auto inicio = chrono::high_resolution_clock::now();

    #pragma omp parallel for num_threads(NUM_THREADS)
    for (int i = 0; i < documentos.size(); ++i) {
        procesar_documento(documentos[i], i + 1, indices_locales[i]);
    }

    auto fin = chrono::high_resolution_clock::now();
    auto duracion = chrono::duration_cast<chrono::seconds>(fin - inicio).count();
    cout << "Tiempo de procesamiento: " << duracion << " segundos\n";

    auto indice_global = combinar_indices(indices_locales);

    ofstream salida("indice_invertido.txt");
    int id = 1;
    for (const auto& par : indice_global) {
        salida << id++ << " " << par.first << " ";
        for (int doc_id : par.second) {
            salida << doc_id << ",";
        }
        salida << "\n";
    }

    cout << "Índice invertido generado correctamente." << endl;
    return 0;
}
