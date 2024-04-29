# Horaris-FIB

### Importante: hay una sección en [español](#español-generador-de-horarios-para-la-fib-upc) más abajo.

<img src="https://raw.githubusercontent.com/DarkJaslo/Horaris-FIB/master/img/screenshot1.PNG" alt="image" title = "UI" width="99%" height="99%">

## (English) Schedule generator for FIB-UPC 
Generates good schedules given a set of subjects. Works in Linux (only tested in Ubuntu).
### Good stuff:

* You can request all the subjects you want, because you also can tell the program which ones must always appear (which can be none, too).

* If needed, can generate schedules mixing theory and lab groups (eg. enrolling in PAR 23L, but going to PAR 40T)

* Allows specifying preference between morning, afternoon or no preference at all.

* Most of the time, good schedules will be displayed first. My own criteria are used here, but since the output is long (1000 different unique options!), I think it works for everyone.

### How to build and use
Requirements: being able to use a <tt>Makefile</tt>, or at least being able to compile <tt>C++11</tt>. The <tt>Makefile</tt> uses <tt>g++</tt>.

The <tt>httplib</tt> header-only library (https://github.com/yhirose/cpp-httplib) depends on <tt>openSSL</tt>. You will need the dev version (in Ubuntu/Debian, <tt>libssl-dev</tt>).

The application also uses the <tt>Qt5 library</tt>.

1. Clone / download the repository.
2. In any console, navigate to the repository's directory and execute <tt>make</tt>.
3. Then, type <tt>./Horaris-FIB</tt> to execute the program built by <tt>make</tt>.
4. The program displays schedules at the side, one by one, with buttons to move between them. If no compatible schedule is found, it will display nothing.

### Program usage

You need to specify:

1. Semester (Q1 is fall and Q2 is next year's spring).
2. Major (especially if you're not an Informatics Engineering student [GEI]).
3. Number of subjects: the default is 5, but if you want less or 6 (the maximum they allow), remember to change it.
4. Preference: Morning -> will output morning schedules first; Afternoon -> same thing with afternoon schedules.
5. Allow mixed groups: allows generating 'illegal' schedules where, for instance, the theory group is 20 but the lab group (your real enrollment here) is 11, which can be useful if your intended theory group (10 here) is impossible or very inconvenient for you. This is playing a dangerous game, but you can enable it if needed.

### Future upgrades

Being able to explicitly exclude subgroups of selected subjects.

## (Español) Generador de horarios para la FIB-UPC 
Genera buenos horarios a partir de una lista de asignaturas. Funciona en Linux (probado solo en Ubuntu).
### Utilidades: 

* Puedes pedir que aparezcan tantas asignaturas como quieras, pues especificas a parte cuáles quieres que salgan obligatoriamente siempre (si las hay).

* Se puede activar la generación de horarios que mezclen grupos de teoría y lab (ej: PAR 23L, pero vas a teoría del PAR 40T)

* Se puede decir si se prefieren horarios de mañanas o tardes (o si no se prefiere nada en particular).

* En general, los horarios buenos saldrán primero. Se usa mi criterio personal para ordenarlos y creo que funciona, pero de todas formas se imprimen hasta 1000 horarios (¡unos cuantos!), así que creo que debería servir a todo el mundo.

### Cómo compilar y ejecutar
Requisitos: poder usar un <tt>Makefile</tt> <tt>(make)</tt> y poder compilar <tt>C++11</tt>. El <tt>Makefile</tt> usa <tt>g++</tt>.

La librería ya incluída <tt>httplib</tt> (https://github.com/yhirose/cpp-httplib) depende de <tt>openSSL</tt>. Es necesaria la versión de desarrollo (en Ubuntu/Debian, <tt>libssl-dev</tt>).

<tt>Qt5</tt> para la interfaz gráfica.

1. Clonar / descargar el repositorio.
2. Desde una consola en el directorio del repositorio, ejecutar <tt>make</tt> 
3. Para ejecutar, escribir <tt>./Horaris-FIB</tt>.
4. El programa enseña los horarios a un lado, con botones para moverse entre ellos. Si no aparece ninguno, es seguramente porque ningún horario con esos grupos es compatible.

### Instrucciones de uso

Debes especificar:

1. Cuatrimestre (Q1 es otoño y Q2 es la primavera del siguiente año. La FIB no da el otoño siguiente durante la primavera hasta que termina el curso).
2. Grado (especialmente si no haces ingeniería informática [GEI]).
3. Número de asignaturas: por defecto son 5, pero si quieres 4 o 6 recuerda cambiarlo.
4. Preferencia: Mañana -> sacará en general primero los horarios de mañanas; Tarde -> lo mismo pero por la tarde.
5. Permite grupos mixtos: permite generar horarios 'ilegales' donde, por ejemplo, el grupo de teoría que te indica es el 20, pero tu grupo de laboratorio (en el que te matriculas) es el 11, cosa que te puede ser útil si el grupo al que deberías ir (el 10 en este ejemplo) no te es posible o es demasiado incómodo para ti. Esto es jugársela, pues en algunas asignaturas de una forma u otra es obligatorio ir a teoría de tu grupo específicamente, pero la opción está ahí y la puedes activar.

### Futuras actualizaciones

Poder excluir de forma explícita subgrupos de asignaturas seleccionadas.