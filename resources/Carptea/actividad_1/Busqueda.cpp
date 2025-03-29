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
bool busquedaSecuencial(const std::vector<int>& vec, int target) {
	for (int num : vec) {
		if (num == target) {
			return true; // Si se encuentra el elemento, devuelve true
		}
	}
	return false; // Si no se encuentra el elemento, devuelve false
}

int main() {
	
	std::ofstream archivo("resultados$$$.csv");
	if (!archivo.is_open()) {
		std::cerr << "Error al abrir el archivo." << std::endl;
		return 1;
	}

	// Inicializa el generador de números aleatorios
	std::random_device rd;  //inicializa la semilla 
	std::mt19937 gen(rd());   //generador de números aleatorio basado en el algoritmo Mersenne Twister 19937 con la semilla rd()

	archivo << std::fixed << std::setprecision(6); // Fijar precisión decimal
	archivo << "Tamano del Vector;Mejor Tiempo (us);Peor Tiempo (us);Tiempo Medio (us)" << std::endl;

	for (int n = TALLA_INICIAL; n <= TALLA_FINAL; n += TALLA_INCREMENTO) {
		std::vector<int> vectorAleatorio = generarVectorAleatorio(n, gen);

		double tiempo_total = 0.0;
		double mejor_tiempo = std::numeric_limits<double>::max(); // Se inicializa con el valor más grande posible (DBL_MAX en <cfloat>).
		double peor_tiempo = 0.0;

		std::uniform_int_distribution<int> distrib(1, n); // Distribución uniforme entre 1 y n

		// Realiza 10 búsquedas aleatorias y mide el tiempo
		for (int i = 0; i < N_BUSQUEDAS; ++i) {
			int valorAleatorio = distrib(gen); //Genera el número aleatorio entre 1 y n

			auto inicio = std::chrono::high_resolution_clock::now();
			bool encontrado = busquedaSecuencial(vectorAleatorio, valorAleatorio);
			auto fin = std::chrono::high_resolution_clock::now();

			double tiempo_busqueda = std::chrono::duration<double, std::micro>(fin - inicio).count();  //Calcular el coste en microsegundos
			tiempo_total += tiempo_busqueda;

			mejor_tiempo = std::min(mejor_tiempo, tiempo_busqueda);
			peor_tiempo = std::max(peor_tiempo, tiempo_busqueda);

			std::cout << "mejor: " << mejor_tiempo << "\npeor: " << peor_tiempo << "\nmedio: " << tiempo_busqueda << '\n';
		}

		// Calcula y guarda el tiempo medio, mejor y peor tiempo de búsqueda en el archivo
		double tiempo_medio = tiempo_total / N_BUSQUEDAS;
		archivo << n << ";" << mejor_tiempo << ";" << peor_tiempo << ";" << tiempo_medio << std::endl;
	}

	archivo.close();
	std::cout << "Los resultados se han guardado en el archivo 'resultados.csv'." << std::endl;

	return 0;
}