# Horaris-FIB

## [English] Schedule generator for FIB-UPC 
Generates all possible schedules given a set of subjects.
### Good stuff:
-Allows choosing "mandatory" subjects (that will always appear in every schedule generated).

-Sorts the results, such that good schedules will be printed first. My own criteria are used here, but it's way better than not having them ordered.

### Issues:
-Subjects are entered manually. I greatly recommend using an input file like <tt>input_example.txt</tt> and redirecting stdin as well as stdout.

-It's intended to work with schedules of exactly five different subjects at a time.

### How to build and use
Requirements: being able to use a Makefile, or at least being able to compile C++11.

In any console, navigate to the repository's directory and enter <tt>make</tt>. Then, type <tt>./main.exe < [input_file]</tt>.
  
The program's output can be redirected (so it does not appear in the console) by adding <tt> > [output_file]</tt>. The whole command would be <tt>./main.exe < [input_file] > [output_file]</tt>.

### Input format
I recommend looking at the example input file in order to understand everything. Some clarifications:
  1. Every subgroup is added separately, but groups that share subject must share the same name (for instance, PRO1 11 and PRO1 12 are different groups of the same subject and should be added separately).
  2. There's theory classes and labs. Understandably, some subjects are structured differently, but you get the idea.
  3. If a subject has theory classes in two different days, they must be added in two separated lines. Consult the example for more info.
  4. To end class hours input, type -1. To end subject groups input, type <tt>endassig</tt>.
  5. To end all subjects, type <tt>end</tt>.
  6. After said <tt>end</tt>, you can write the names of mandatory subjects (that will be included in every schedule forcefully) as seen in the example.  
  7. I recommend using very short names, with two characters.

  
## [Español] Generador de horarios sencillo para la FIB-UPC 
Genera todos los horarios posibles de un conjunto de asignaturas.
### Ventajas: 
-Permite escoger asignaturas "obligatorias" (que saldrán sí o sí en el horario).

-Ordena los horarios, de forma que en general los horarios buenos saldrán primero y los malos después. Se usa mi criterio personal, pero en general sirve muchísimo más que no ordenarlos.
### Problemas: 
-La entrada de asignaturas es a mano. Recomiendo usar un fichero de texto como el de <tt>input_example.txt</tt> y redirigir tanto entrada como salida.

-Está pensado para hacer horarios con 5 asignaturas.

### Cómo compilar y ejecutar
Requisitos: poder usar un Makefile (make) y poder compilar C++11.

Desde una consola en el directorio del repositorio, escribir <tt>make</tt> y posteriormente <tt>./main.exe < [input_file]</tt>.
  
  Se puede redirigir la salida (para que no aparezca en la consola) añadiendo <tt> > [output_file]</tt>, de forma que quedaría <tt>./main.exe < [input_file] > [output_file]</tt>.

### Formato de la entrada
Recomiendo mirar el fichero de ejemplo para entenderlo. Algunas aclaraciones:
1.  Se añade cada subgrupo por separado, pero todos los que sean de la misma asignatura deben compartir el mismo nombre (por ejemplo, PRO1 11 y PRO1 12 son grupos distintos, pero de la misma asignatura)
  2.  Hay <tt>teoría</tt> y <tt>laboratorio</tt>. Obviamente, algunas asignaturas tienen problemas y otras cosas, pero se puede mapear a teoría y laboratorio.
  3. Si una asignatura tiene teoría dos días distintos (por ejemplo, 2 horas el lunes y 1 hora el miércoles), deben ponerse dos líneas separadas. Véase en el ejemplo la entrada para IA 11 o IA 13.
  4. Al acabar de poner las horas, se pone -1, y al acabar de poner un grupo de asignatura, se pone <tt>endassig</tt>.
  5. Al acabar la entrada de asignaturas, se pone <tt>end</tt>.
  6. Para añadir asignaturas obligatorias, se escriben sus nombres al final del fichero, como en el ejemplo.
  7. Recomiendo usar nombres de asignatura muy cortos, de uno o dos caracteres.
