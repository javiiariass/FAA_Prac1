// practica1.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>   // Necesario para std::cout y std::cerr
#include <vector>     // Necesario para std::vector
#include <cstdlib>    // Necesario para std::rand y std::srand
#include <fstream>    // Necesario para std::ofstream (manejo de archivos)
#include <iomanip>    // Necesario para std::fixed y std::setprecision
#include <algorithm>  // Necesario para std::min, std::max
#include <chrono>     // Necesario para medir tiempos con std::chrono
#include <random>     // Necesario para std::mt19937 y std::shuffle
#include <limits>     // Necesario para std::numeric_limits

// ------------------------------- Constantes para varias las tallas -------------------------------

// Numero de busquedas
const int N_BUSQUEDAS = 10;
// Talla inicial
const int TALLA_INICIAL = 1000;
// Talla final
const int TALLA_FINAL = 10000;
// Incremento entre iteraciones de la talla
const int TALLA_INCREMENTO = 1000;


// ------------------------------- Implementaciones ------------------------------- 

// Función para generar un vector de n elementos aleatorios sin números repetidos
std::vector<int> generarVectorAleatorio(int n, std::mt19937& gen) {
	std::vector<int> vec(n);
	for (int i = 0; i < n; ++i) {
		vec[i] = i + 1; // Llena el vector con números del 1 al n
	}
	std::shuffle(vec.begin(), vec.end(), gen); // Mezcla los números aleatoriamente usando el algoritmo de Fisher-Yates
	return vec;
}

// Función para realizar una búsqueda secuencial en un vector
bool busquedaSecuencial(const std::vector<int>& vec, int target, int* operaciones) {
	int contAsignaciones, contComparaciones, contOE;
	contAsignaciones = contComparaciones = contOE = 0;
	bool aux = false;

	// Acceso a vector y asignacion a variable num
	for (int num : vec) {
		contAsignaciones++;
		contOE++;

		// Comparación por cada iteracion en bucle
		contComparaciones++;
		if (num == target) {
			aux = true; // Si se encuentra el elemento, devuelve true
			break;
		}
	}

	//return
	contOE++;

	// total de operaciones elementales
	contOE += (contComparaciones + contAsignaciones);

	*operaciones = contOE;

	return aux; // Si no se encuentra el elemento, devuelve false
}

int main() {

	// Primer elemento -> comparaciones
	// segundo elemento -> asignaciones
	// tercer elemento -> total oe
	int* contOp;

	

	std::ofstream archivo("resultados$$$.csv");
	if (!archivo.is_open()) {
		std::cerr << "Error al abrir el archivo." << std::endl;
		return 1;
	}

	// Inicializa el generador de números aleatorios
	std::random_device rd;  //inicializa la semilla 
	std::mt19937 gen(rd());   //generador de números aleatorio basado en el algoritmo Mersenne Twister 19937 con la semilla rd()
	archivo << std::fixed << std::setprecision(6); // Fijar precisión decimal
	archivo << "Tamano del Vector;Total operaciones Mejor Caso;Total operaciones Peor Caso; Total operaciones Caso Medio" << std::endl;

	for (int n = TALLA_INICIAL; n <= TALLA_FINAL; n += TALLA_INCREMENTO) {
		std::vector<int> vectorAleatorio = generarVectorAleatorio(n, gen);

		
		int OE_totales = 0;
		int OE_mejor = INT_MAX;
		int OE_peor = 0;

		std::uniform_int_distribution<int> distrib(1, n); // Distribución uniforme entre 1 y n

		// Realiza 10 búsquedas aleatorias y mide el tiempo
		for (int i = 0; i < N_BUSQUEDAS; ++i) {
			int valorAleatorio = distrib(gen); //Genera el número aleatorio entre 1 y n
			contOp = new int;
			bool encontrado = busquedaSecuencial(vectorAleatorio, valorAleatorio,contOp);
			OE_totales += *contOp;
			OE_mejor = std::min(OE_mejor, *contOp);
			OE_peor = std::max(OE_peor, *contOp);
			*contOp = 0;
		}
		delete[] contOp;
		
		// Calcula y guarda las OE medias, mejores y peores en el archivo
		int OE_medio = OE_totales / N_BUSQUEDAS;
		
		
		archivo << n << ";" << OE_mejor << ";" << OE_peor << ";" << OE_medio << std::endl;
	}

	archivo.close();
	std::cout << "Los resultados se han guardado en el archivo 'resultados.csv'." << std::endl;

	return 0;
}