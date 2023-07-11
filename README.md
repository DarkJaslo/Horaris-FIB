# Horaris-FIB

## [English] Schedule generator for FIB-UPC 
Generates all possible schedules given a set of subjects.
### Good stuff:
-Parses the document at FIB's API, simplifying input.

-Allows choosing what subjects appear always and what subjects can appear with them. 

-Sorts the results, such that good schedules will be printed first. My own criteria are used here, but it's way better than not having them ordered.

### Issues:
-The application doesn't automatically download the file from the FIB's API yet. The user will have to get a copy manually (fortunately, it just has to be done once, and this can be easily done with a few clicks, ctrl+A, ctrl+C and ctrl+V).

### How to build and use
Requirements: being able to use a Makefile, or at least being able to compile C++11.

1. In any console, navigate to the repository's directory and enter <tt>make</tt>. 
2. You can just execute <tt>./main.exe</tt> with no parameters or input, and it will output its usage.
3. Then, type <tt>./main.exe [FIB_DATA_file] < [input_file]</tt>, replacing these files with your actual files.
4. The program's output can be redirected (so it does not appear in the console) by adding <tt> > [output_file]</tt>. The whole command would be <tt>./main.exe [FIB_DATA_file] < [input_file] > [output_file]</tt>.

### How to obtain the FIB's data (for FIB students)

1. Enter https://api.fib.upc.edu/v2/quadrimestres/actual/
2. Log in if needed.
3. Field <tt>"id"</tt> is the current semester.
4. Enter the link in field <tt>"classes"</tt>.
5. This is the file this program can read. You can easily copy it to a local file with CTRL+A and copy. You can leave the file without removing anything, because the program ignores all lines until <tt>"count":</tt>.

### Input format

I recommend looking at <tt>input.inp</tt> and using it as a template.

## [Español] Generador de horarios sencillo para la FIB-UPC 
Genera todos los horarios posibles de un conjunto de asignaturas.
### Utilidades: 

-Parsea el documento oficial de horarios de la API de la FIB, cosa que simplifica mucho el proceso de input.

-Permite escoger qué asignaturas forman parte de los horarios, y se puede imponer que algunas aparezcan en todos.

-Ordena los horarios, de forma que en general los horarios buenos saldrán primero y los malos después. Se usa mi criterio personal, pero en general sirve muchísimo más que no ordenarlos.
### Limitaciones: 
-La aplicación aún no descarga automáticamente los datos de la API de la FIB. Tú tendrás que descargar manualmente una copia (por suerte, eso solo hace falta una vez por cuatrimestre, y se puede hacer fácilmente con unos cuantos clicks, ctrl+A, ctrl+C y ctrl+V).

### Cómo compilar y ejecutar
Requisitos: poder usar un Makefile (make) y poder compilar C++11.

1. Desde una consola en el directorio del repositorio, escribir <tt>make</tt> 
2. Se puede ejecutar <tt>./main.exe</tt> directamente, éste dirá por consola cómo debe ser usado.
3. Escribir <tt>./main.exe [FIB_DATA_file] < [input_file]</tt>, sustituyendo los valores por los ficheros que tú tengas.
4. Se puede redirigir la salida (para que no aparezca en la consola) añadiendo <tt> > [output_file]</tt>, de forma que quedaría <tt>./main.exe [FIB_DATA_file] < [input_file] > [output_file]</tt>.

### Cómo obtener los datos de la FIB (para estudiantes)

1. Entra en https://api.fib.upc.edu/v2/quadrimestres/actual/
2. Inicia sesión si se pide.
3. El campo <tt>"id"</tt> es el cuatrimestre actual.
4. Entra en el enlace del campo <tt>"classes"</tt>.
5. Este es el fichero que el programa puede leer. Puedes copiarlo fácilmente en un fichero local con CTRL+A y copia. Puedes dejar ese texto en el fichero local sin quitar nada, porque el programa ignora todo hasta la línea con <tt>"count":</tt>.

### Formato de la entrada

Recomiendo mirar el fichero <tt>input.inp</tt> y usarlo como plantilla.