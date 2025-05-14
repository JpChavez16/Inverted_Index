use rayon::prelude::*;
use std::collections::{HashMap, HashSet};
use std::sync::Mutex;
use std::io::{BufReader, BufRead};
use std::fs::File;
use std::time::Instant;

fn limpiar_palabra(palabra: &str) -> String {
    palabra.chars()
        .filter(|c| c.is_alphanumeric())
        .collect::<String>()
        .to_lowercase()
}

fn procesar_archivo(path: &str, doc_id: usize, indice: &Mutex<HashMap<String, HashSet<usize>>>) {
    let archivo = File::open(path).expect("No se pudo abrir el archivo");
    let lector = BufReader::new(archivo);

    for linea in lector.lines() {
        for palabra in linea.unwrap().split_whitespace() {
            let limpia = limpiar_palabra(palabra);
            if !limpia.is_empty() {
                let mut mapa = indice.lock().unwrap();
                mapa.entry(limpia).or_insert_with(HashSet::new).insert(doc_id);
            }
        }
    }
}

fn main() {
    let archivos: Vec<String> = (1..=10)
        .map(|i| format!("../../outputs_split/doc{}.txt", i))
        .collect();

    let indice_global = Mutex::new(HashMap::<String, HashSet<usize>>::new());

    let inicio = Instant::now();

    archivos.par_iter().enumerate().for_each(|(id, archivo)| {
        procesar_archivo(archivo, id + 1, &indice_global);
    });

    let duracion = inicio.elapsed();
    println!("Tiempo : {:.2?}", duracion);

    let indice = indice_global.lock().unwrap();
    let mut salida = File::create("indice_invertido_rust.txt").expect("No se pudo crear el archivo");

    for (id, (palabra, docs)) in indice.iter().enumerate() {
        use std::io::Write;
        write!(salida, "{} {} ", id + 1, palabra).unwrap();
        for doc_id in docs {
            write!(salida, "{},", doc_id).unwrap();
        }
        writeln!(salida).unwrap();
    }

    println!("Índice invertido generado correctamente en Rust.");
}

