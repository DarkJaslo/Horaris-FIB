# Horaris-FIB

## Generador de horarios sencillo para la FIB-UPC
Genera todos los horarios posibles de un conjunto de asignaturas.
### Plus: 
-Permite escoger asignaturas "obligatorias" (que saldrán sí o sí en el horario).
### Problemas: 
-La entrada de asignaturas es a mano. Recomiendo usar un fichero de texto como el de <tt>input_example.txt</tt> y redirigir tanto entrada como salida.

-No filtra los horarios malos ni ordena de mejor a peor (de momento).

-De momento, requiere al menos una asignatura obligatoria y una no obligatoria (será arreglado pronto).

### Cómo compilar y ejecutar
Requisitos: poder usar un Makefile (make) y poder compilar C++11.

Desde una consola en el directorio del repositorio, escribir <tt>make</tt> y posteriormente <tt>./main.exe < [input_file]</tt>.
  
  Se puede redirigir la salida (para que no aparezca en la consola) añadiendo <tt> > [output_file]</tt>, de forma que quedaría <tt>./main.exe < [input_file] > [output_file]</tt>.

### Formato de la entrada
Recomiendo mirar el fichero de ejemplo para entenderlo. Algunas aclaraciones:
1.  Se añade cada subgrupo por separado, pero todos los que sean de la misma asignatura deben compartir el mismo nombre (por ejemplo, PRO1 11 y PRO1 12 son grupos distintos, pero de la misma asignatura)
  2.  Hay <tt>teoría</tt> y <tt>laboratorio</tt>. Obviamente, algunas asignaturas tienen problemas y otras cosas, pero se puede mapear a teoría y laboratorio.
  3. Si una asignatura tiene teoría dos días distintos (por ejemplo, 2 horas el lunes y 1 hora el miércoles), deben ponerse dos líneas separadas. Véase en el ejemplo la entrada para IA 11 o IA 13.
  4. Al acabar de poner las horas, se pone -1, y al acabar de poner un grupo de asignatura, se pone <tt>endtipus</tt>.
  5. Al acabar la entrada de asignaturas, se pone <tt>end</tt>.
  6. Para añadir asignaturas obligatorias, se escriben sus nombres al final del fichero, como en el ejemplo.
