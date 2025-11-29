# ProyectoArbolesBinarios

PROYECTO: SISTEMA DE SUCESIÓN REAL BASADO EN ÁRBOLES BINARIOS
Autor: Santiago Leal — Cédula 32.741.894

DESCRIPCIÓN GENERAL
Este proyecto implementa un sistema de administración de un árbol genealógico real, donde cada miembro del linaje es representado mediante un nodo dentro de un árbol binario. A partir de un archivo CSV, el programa carga toda la información de la familia y determina automáticamente quién debe ocupar el trono según un conjunto de reglas de sucesión previamente establecidas. El objetivo principal es simular de forma estructurada la herencia del trono, considerando restricciones de parentesco, edad, género y disponibilidad de herederos dentro de las distintas ramas familiares.

FUNCIONALIDADES PRINCIPALES

Carga de datos desde archivo CSV
El programa lee un archivo con formato:
id, nombre, apellido, genero, edad, id_padre, esta_muerto, fue_rey, es_rey
A partir de esta información construye el árbol genealógico respetando el id del padre y la estructura de descendencia del primer rey.

Construcción del árbol genealógico
Cada persona se inserta en el árbol según su relación padre-hijo.
Dado que cada rey únicamente puede tener hasta dos descendientes, el árbol mantiene una estructura binaria: el hijo primogénito ocupa el lado izquierdo y el segundo hijo ocupa el lado derecho. Debido a esto, la construcción del árbol respeta el orden de nacimiento y permite navegar correctamente por las distintas ramas familiares.

Generación de la línea de sucesión
El programa muestra en pantalla la línea de sucesión tomando en cuenta solamente a los miembros vivos y elegibles.
Las reglas principales consideran:

Prioridad total a varones dentro de la rama del primogénito.

Exclusión automática de personas fallecidas o mayores de 70 años.

En caso de que todos los primogénitos estén muertos o sean inelegibles, se explora la rama del hermano, del tío y posteriormente se asciende hasta encontrar un ancestro con dos ramas válidas.

Cuando no existen varones disponibles, la corona pasa a la mujer mayor de 15 años más joven dentro de la rama más cercana al linaje primogénito.

Asignación automática de un nuevo rey
Si el rey actual muere o supera los 70 años, el sistema analiza las diferentes ramas familiares para determinar quién debe convertirse en el nuevo rey. El proceso incluye:

Hijos varones del rey.

Nietos dentro de la rama primogénita.

Hermanos y descendencia de los hermanos.

Tíos y descendencia de los tíos.

Mujeres en caso de ausencia total de varones.

Reglas especiales para sucesión posterior a una reina, en cuyo caso la corona vuelve a un varón en la línea establecida.

Modificación de datos
El usuario puede cambiar los datos de cualquier miembro del árbol con excepción del id y del id del padre, que se mantienen fijos para conservar la estructura del árbol.

ESTRUCTURA DEL PROYECTO
El proyecto se organiza en carpetas de la siguiente forma:

bin/
Contiene el archivo ejecutable y el archivo datos.csv utilizado para pruebas.

src/
Contiene los archivos fuente del proyecto (.cpp).

COMO COMPILAR
Para compilar el proyecto desde la consola, utilizar lo siguiente:

primero entramos en carpeta: cd ProyectoArbolesBinarios

luego
g++ -std=c++11 src/main.cpp src/reino.cpp -o bin/reino.exe

EJECUCIÓN DEL PROGRAMA
Una vez compilado, el programa se ejecuta con:

.\bin\reino.exe

El menú del programa permite:

Mostrar la línea de sucesión.

Seleccionar un nuevo rey de forma automática.

Modificar datos de un miembro.

Visualizar información completa.

Finalizar la ejecución.

OBJETIVO DEL TRABAJO
El propósito de este proyecto es aplicar los conceptos fundamentales de árboles binarios, organización jerárquica y manejo de datos para crear un simulador de sucesión monárquica. Se busca que el estudiante comprenda la lógica de navegación de árboles, el manejo de archivos CSV, el diseño modular en C++ y la implementación manual de estructuras sin uso de librerías avanzadas
