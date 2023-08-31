# Horaris-FIB

## [English] Schedule generator for FIB-UPC 
Generates all possible schedules given a set of subjects. Works in Linux (only tested in Ubuntu).
### Good stuff:
-Automatically downloads and parses the document at FIB's API, simplifying input.

-Allows choosing which subjects appear always and what subjects can appear with them.

-Allows choosing which subgroups don't appear in schedules, which can be useful sometimes.

-Sorts the results, such that good schedules will be printed first. My own criteria are used here, but it's way better than not having them ordered.

### How to build and use
Requirements: being able to use a Makefile, or at least being able to compile C++11.

Having python3 installed (uses a python script to download data from https://api.fib.upc.edu).

1. In any console, navigate to the repository's directory and enter <tt>make</tt>. 
2. Then, type <tt>./main.exe < [input_file]</tt>, replacing these files with your actual files.
3. The program's output can be redirected (so it does not appear in the console) by adding <tt> > [output_file]</tt>. The whole command would be <tt>./main.exe < [input_file] > [output_file]</tt>.

### Input format

I recommend looking at <tt>input.txt</tt> and using it as a template.

## [Español] Generador de horarios sencillo para la FIB-UPC 
Genera todos los horarios posibles de un conjunto de asignaturas. Funciona en Linux (probado solo en Ubuntu).
### Utilidades: 

-Descarga automáticamente y parsea el documento oficial de horarios de la API de la FIB, cosa que simplifica mucho el proceso de input.

-Permite escoger qué asignaturas forman parte de los horarios, y se puede imponer que algunas aparezcan en todos.

-Permite escoger qué subgrupos no aparecen en los horarios, cosa que puede ser útil a veces.

-Ordena los horarios, de forma que en general los horarios buenos saldrán primero y los malos después. Se usa mi criterio personal, pero en general sirve muchísimo más que no ordenarlos.

### Cómo compilar y ejecutar
Requisitos: poder usar un Makefile (make) y poder compilar C++11.

Tener python3 instalado (se usa un script en python para descargar la información de los horarios de https://api.fib.upc.edu).

1. Desde una consola en el directorio del repositorio, escribir <tt>make</tt> 
2. Escribir <tt>./main.exe < [input_file]</tt>, sustituyendo los valores por los ficheros que tú tengas.
3. Se puede redirigir la salida (para que no aparezca en la consola) añadiendo <tt> > [output_file]</tt>, de forma que quedaría <tt>./main.exe < [input_file] > [output_file]</tt>.

### Formato de la entrada

Recomiendo mirar el fichero <tt>input.txt</tt> y usarlo como plantilla.