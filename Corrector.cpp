/***************************************
	UNIVERSIDAD NACIONAL AUTONOMA DE MEXICO
	FACULTAD DE ESTUDIOS SUPERIORES -ARAGON-

	Computadoras y programacion.
	Santiago Ayax Yañez Ariza 323263005

	Quiso decir: Programa principal de la aplicacion de la distancia de Levenstein.

**************************************/


#include "stdafx.h"
#include <string.h>
#include "corrector.h"
#include <wctype.h> //Para poder usar la funcion towlower, es mejor que ctype.h porque soporta mas caracteres (como acentos).
//Funciones publicas del proyecto
/***************************************
	DICCIONARIO: Esta funcion crea el diccionario completo
	char *	szNombre				:	Nombre del archivo de donde se sacaran las palabras del diccionario
	char	szPalabras[][TAMTOKEN]	:	Arreglo con las palabras completas del diccionario
	int		iEstadisticas[]			:	Arreglo con el numero de veces que aparecen las palabras en el diccionario
	int &	iNumElementos			:	Numero de elementos en el diccionario
**************************************/
void	Diccionario(char* szNombre, std::unordered_map<std::unordered_map<std::string szPalabras[][TAMTOKEN], int iEstadisticas[], int& iNumElementos)
{
	FILE* fp;
	fopen_s(&fp, szNombre, "r"); 

	if (fp != NULL) { 
		iNumElementos = 0; 
		int caracter; 
		char szPalabraTemporal[TAMTOKEN]; 
		int contador = 0; 
		while ((caracter = fgetc(fp)) != EOF) { 

			char caracterLetra = (char)caracter; 
			if (caracterLetra == '\n' || caracterLetra == '\r' || caracterLetra == '\t' || caracterLetra == ' ' || caracterLetra == ',' || caracterLetra == ';' || caracterLetra == '(' || caracterLetra == ')' || caracterLetra == '.') { //Separadores que NO se cuentan
				szPalabraTemporal[contador] = '\0'; 

				if (contador > 0) {
					bool PalabraEncontrada = false; 

					for (int i = 0; i < iNumElementos; i++) { 

						if (strcmp(szPalabras[i], szPalabraTemporal) == 0) { 
							PalabraEncontrada = true; 
							iEstadisticas[i]++; 
							break; 
						}

					}

					if (PalabraEncontrada == false) {
						if (iNumElementos < NUMPALABRAS) { 
							strcpy_s(szPalabras[iNumElementos], TAMTOKEN, szPalabraTemporal); 
							iEstadisticas[iNumElementos] = 1; 
							iNumElementos++; 
						}
					}

				}

				contador = 0; 
				szPalabraTemporal[contador] = '\0'; 

			}
			else {				
				caracter = towlower(caracter); 
				if (contador < TAMTOKEN - 1) {
					
					szPalabraTemporal[contador] = (char)caracter; 
					contador++; 
				}

			}

		}

		if (contador > 0) { 
			szPalabraTemporal[contador] = '\0';

			bool PalabraEncontrada = false; 

			for (int i = 0; i < iNumElementos; i++) { 

				if (strcmp(szPalabras[i], szPalabraTemporal) == 0) {
					PalabraEncontrada = true;
					iEstadisticas[i]++; 
					break;
				}

			}

			if (PalabraEncontrada == false) {
				if (iNumElementos < NUMPALABRAS) { 
					strcpy_s(szPalabras[iNumElementos], TAMTOKEN, szPalabraTemporal); 
					iEstadisticas[iNumElementos] = 1; 
					iNumElementos++; 
				}
			}

		}
        fclose(fp);
		bool bNoOrdenado = true; 

		for (int j = 0; j < iNumElementos && bNoOrdenado; j++) {
			bNoOrdenado = false; 

			for (int k = 0; k < iNumElementos - 1; k++) {

				if (strcmp(szPalabras[k], szPalabras[k + 1]) > 0) {
					
					char aux[TAMTOKEN]; 
					
					int auxEstadistica; 
					strcpy_s(aux, TAMTOKEN, szPalabras[k]);
					strcpy_s(szPalabras[k], TAMTOKEN, szPalabras[k + 1]); 
					strcpy_s(szPalabras[k + 1], TAMTOKEN, aux);
					auxEstadistica = iEstadisticas[k]; 
					iEstadisticas[k] = iEstadisticas[k + 1];
					iEstadisticas[k + 1] = auxEstadistica;

					bNoOrdenado = true;
				}

			}

		}

		fclose(fp); 

	}
	else {
		iNumElementos = 0; 
	}

}

/***************************************
	ListaCandidatas: Esta funcion recupera desde el diccionario las palabras validas y su peso
	Regresa las palabras ordenadas por su peso
	char	szPalabrasSugeridas[][TAMTOKEN],	//Lista de palabras clonadas
	int		iNumSugeridas,						//Lista de palabras clonadas
	char	szPalabras[][TAMTOKEN],				//Lista de palabras del diccionario
	int		iEstadisticas[],					//Lista de las frecuencias de las palabras
	int		iNumElementos,						//Numero de elementos en el diccionario
	char	szListaFinal[][TAMTOKEN],			//Lista final de palabras a sugerir
	int		iPeso[],							//Peso de las palabras en la lista final
	int &	iNumLista)							//Numero de elementos en la szListaFinal
**************************************/
void	ListaCandidatas(
	std::string	szPalabrasSugeridas[][TAMTOKEN],	//Lista de palabras clonadas
	int		iNumSugeridas,						//Lista de palabras clonadas
	std::string	szPalabras[][TAMTOKEN],				//Lista de palabras del diccionario
	int		iEstadisticas[],					//Lista de las frecuencias de las palabras
	int		iNumElementos,						//Numero de elementos en el diccionario
	std::string	szListaFinal[][TAMTOKEN],			//Lista final de palabras a sugerir
	int		iPeso[],							//Peso de las palabras en la lista final
	int& iNumLista)							//Numero de elementos en la szListaFinal
{
	
	iNumLista = 0; 

	if (iNumSugeridas == 0){
		return;
	}	
	
	for (int paso = 0; paso < iNumSugeridas; paso++) { 
		for (int c = 0; c < iNumElementos; c++) { 

			if (strcmp(szPalabrasSugeridas[paso], szPalabras[c]) == 0) { 
				strcpy_s(szListaFinal[iNumLista], TAMTOKEN, szPalabrasSugeridas[paso]);
				iPeso[iNumLista] = iEstadisticas[c];
				iNumLista++; 
				break;
			}

		}

	}

	bool bNoOrdenado = true; 

	for (int j = 0; j < iNumLista && bNoOrdenado; j++) {
		bNoOrdenado = false;

		for (int k = 0; k < iNumLista - 1; k++) {

			if (iPeso[k] < iPeso[k + 1]) { 
				
				char auxPalabrasFinales[TAMTOKEN];
				int auxiliarPeso;
				strcpy_s(auxPalabrasFinales, TAMTOKEN, szListaFinal[k]);  
				strcpy_s(szListaFinal[k], TAMTOKEN, szListaFinal[k + 1]);
				strcpy_s(szListaFinal[k + 1], TAMTOKEN, auxPalabrasFinales);
				auxiliarPeso = iPeso[k]; 
				iPeso[k] = iPeso[k + 1];
				iPeso[k + 1] = auxiliarPeso;

				bNoOrdenado = true;
			}

		}

	}
}

/***************************************
	ClonaPalabras: toma una palabra y obtiene todas las combinaciones y permutaciones requeridas por el metodo
	char *	szPalabraLeida,						// Palabra a clonar
	char	szPalabrasSugeridas[][TAMTOKEN], 	//Lista de palabras clonadas
	int &	iNumSugeridas)						//Numero de elementos en la lista
**************************************/
void	ClonaPalabras(
	char* szPalabraLeida,						// Palabra a clonar
	char	szPalabrasSugeridas[][TAMTOKEN], 	//Lista de palabras clonadas
	int& iNumSugeridas)						//Numero de elementos en la lista
{
	iNumSugeridas = 0; 
	
	
	strcpy_s(szPalabrasSugeridas[iNumSugeridas], TAMTOKEN, szPalabraLeida); 
	iNumSugeridas++; 
	int longitud;
	longitud = strlen(szPalabraLeida); 

	for (int posicion = 0; posicion < longitud; posicion++) { 
		char sinCaracter[TAMTOKEN]; 
		int contador = 0; 
		int contadorDos = 0; 

		for (contador; contador < longitud; contador++) {

			if (!(posicion == contador)) { 
				sinCaracter[contadorDos] = szPalabraLeida[contador]; 
				
				contadorDos++; 
			}

		}

		sinCaracter[contadorDos] = '\0'; 
		
		
		strcpy_s(szPalabrasSugeridas[iNumSugeridas], TAMTOKEN, sinCaracter); 
		iNumSugeridas++; 

	}

	for (int i = 0; i < longitud - 1; i++) { 
		char PalabraRespaldo[TAMTOKEN]; 
		strcpy_s(PalabraRespaldo, TAMTOKEN, szPalabraLeida); 
		
		char auxCaracter;
		auxCaracter = PalabraRespaldo[i]; 
		PalabraRespaldo[i] = PalabraRespaldo[i + 1]; 
		PalabraRespaldo[i + 1] = auxCaracter;
		strcpy_s(szPalabrasSugeridas[iNumSugeridas], TAMTOKEN, PalabraRespaldo); 
		iNumSugeridas++;
	}

	
	char abecedario[] = "abcdefghijklmnñopqrstuvwxyzáéíóú"; 
	int longitud_abecedario = strlen(abecedario); 

	for (int j = 0; j < longitud; j++) { 

		for (int k = 0; k < longitud_abecedario; k++) { 
			char PalabraRespaldoDos[TAMTOKEN]; 
			strcpy_s(PalabraRespaldoDos, TAMTOKEN, szPalabraLeida); 
			PalabraRespaldoDos[j] = abecedario[k]; 
			strcpy_s(szPalabrasSugeridas[iNumSugeridas], TAMTOKEN, PalabraRespaldoDos);
			iNumSugeridas++; 
		}

	}

	
	for (int a = 0; a <= longitud; a++) { 

		for (int b = 0; b < longitud_abecedario; b++) { 
			char insertarPalabra[TAMTOKEN]; 
			for (int c = 0; c < longitud + 1; c++) { 

				if (c == a) { 
					insertarPalabra[c] = abecedario[b]; 
				}
				else if (c > a) { 
					insertarPalabra[c] = szPalabraLeida[c - 1]; 
					
				}
				else { 
					insertarPalabra[c] = szPalabraLeida[c]; 
				}

			}

			insertarPalabra[longitud + 1] = '\0'; 
			strcpy_s(szPalabrasSugeridas[iNumSugeridas], TAMTOKEN, insertarPalabra); 
			iNumSugeridas++; 
		}

	}
	bool bNoOrdenado = true; 

	for (int j = 0; j < iNumSugeridas && bNoOrdenado; j++) {
		bNoOrdenado = false; 

		for (int k = 0; k < iNumSugeridas - 1; k++) {

			if (strcmp(szPalabrasSugeridas[k], szPalabrasSugeridas[k + 1]) > 0) {
				
				char auxPalabrasSugeridas[TAMTOKEN];
				strcpy_s(auxPalabrasSugeridas, TAMTOKEN, szPalabrasSugeridas[k]); 
				strcpy_s(szPalabrasSugeridas[k], TAMTOKEN, szPalabrasSugeridas[k + 1]);
				strcpy_s(szPalabrasSugeridas[k + 1], TAMTOKEN, auxPalabrasSugeridas);

				bNoOrdenado = true;
			}

		}

	}
}
