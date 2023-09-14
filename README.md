# Horaris-FIB

<img src="https://raw.githubusercontent.com/DarkJaslo/Horaris-FIB/master/img/screenshot1.PNG" alt="image 1" title = "Normal output" width="49%" height="49%"> <img src="https://raw.githubusercontent.com/DarkJaslo/Horaris-FIB/master/img/screenshot2.PNG" alt="image 2" title = "Mixed groups" width="49%" height="49%">

### Importante: hay una sección en [español](#español-generador-de-horarios-para-la-fib-upc) más abajo.

## (English) Schedule generator for FIB-UPC 
Generates good schedules given a set of subjects. Works in Linux (only tested in Ubuntu).
### Good stuff:

* You can request all the subjects you want, because you also can tell the program which ones must appear always (which can be none, too).

* Allows removing specific subgroups, which can be useful when they appear a lot but are already full, expected to be full, or just not convenient for you.

* If needed, can generate schedules mixing theory and lab groups (eg. enrolling in PAR 23L, but going to PAR 40T)

* Allows specifying preference between morning, afternoon or no preference at all.

* Most of the time, good schedules will be printed first. My own criteria are used here, but since unwanted subgroups can always be removed and the output is long (1000 different unique options!), I think it works for everyone.

### How to build and use
Requirements: being able to use a <tt>Makefile</tt>, or at least being able to compile <tt>C++11</tt>. The <tt>Makefile</tt> uses <tt>g++</tt>.

The <tt>httplib</tt> header-only library (https://github.com/yhirose/cpp-httplib) depends on <tt>openSSL</tt>. You will need the dev version (in Ubuntu/Debian, <tt>libssl-dev</tt>).

1. Clone / download the repository.
2. In any console, navigate to the repository's directory and execute <tt>make</tt>.
3. Then, type <tt>./horaris.exe < [input_file]</tt>, replacing these files with your actual files.
4. The program's output can be redirected (so it does not appear in the console) by adding <tt> > [output_file]</tt>. The whole command would be <tt>./horaris.exe < [input_file] > [output_file]</tt>.

### Input format

I recommend looking at <tt>input.txt</tt> and using it as a template.

## (Español) Generador de horarios para la FIB-UPC 
Genera buenos horarios a partir de una lista de asignaturas. Funciona en Linux (probado solo en Ubuntu).
### Utilidades: 

* Puedes pedir que aparezcan tantas asignaturas como quieras, pues especificas a parte cuáles quieres que salgan obligatoriamente siempre (si las hay).

* Permite escoger qué subgrupos no aparecen en los horarios. Útil para mirar horarios alternativos si ciertos grupos se llenan, por ejemplo.

* Se puede activar la generación de horarios que mezclen grupos de teoría y lab (ej: PAR 23L, pero vas a teoría del PAR 40T)

* Se puede decir si se prefieren horarios de mañanas o tardes (o si no se prefiere nada en particular).

* En general, los horarios buenos saldrán primero. Se usa mi criterio personal para ordenarlos y creo que funciona, pero siempre se pueden quitar subgrupos para ver otros horarios.

### Cómo compilar y ejecutar
Requisitos: poder usar un <tt>Makefile</tt> <tt>(make)</tt> y poder compilar <tt>C++11</tt>. El <tt>Makefile</tt> usa <tt>g++</tt>.

La librería ya incluída <tt>httplib</tt> (https://github.com/yhirose/cpp-httplib) depende de <tt>openSSL</tt>. Es necesaria la versión de desarrollo (en Ubuntu/Debian, <tt>libssl-dev</tt>).

1. Clonar / descargar el repositorio.
2. Desde una consola en el directorio del repositorio, ejecutar <tt>make</tt> 
3. Escribir <tt>./horaris.exe < [input_file]</tt>, sustituyendo los valores por los ficheros que tú tengas.
4. Se puede redirigir la salida (para que no aparezca en la consola) añadiendo <tt> > [output_file]</tt>, de forma que quedaría <tt>./horaris.exe < [input_file] > [output_file]</tt>.

### Formato de la entrada

Recomiendo mirar el fichero <tt>input.txt</tt> y usarlo como plantilla.