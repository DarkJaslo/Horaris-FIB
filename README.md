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

### How to build
(Note: if you have previously used `qmake`, the also existing qmake setup may already work for you, try it once you meet all requirements. If it's not the case, use `cmake`.)

Requirements: `cmake` and `qt5`.

The `httplib` header-only library (https://github.com/yhirose/cpp-httplib) depends on `openSSL`. You will need the dev version (in Ubuntu/Debian, `libssl-dev`).

1. Clone / download the repository.
```sh
git clone https://github.com/DarkJaslo/Horaris-FIB .
```
2. Compile the source code.
```sh
cd /path/to/repository
mkdir build
cd build
cmake ..
make
```

3. Execute.
```sh
./HorarisFIB
```
4. The program displays schedules at the side, one by one, with buttons to move between them. If no compatible schedule is found, it will display nothing.

### Program usage

Before generating schedules, you need to specify:

1. **Semester** (Q1 is fall and Q2 is next year's spring).
2. **Major** (especially if you're not an Informatics Engineering student [GEI]).
3. **Number of subjects**: the default is 5, but if you want less or 6 (the maximum they allow), remember to change it.
4. **Preference**: Morning -> will output morning schedules first; Afternoon -> same thing with afternoon schedules.
5. **Mixed groups**: allows generating 'illegal' schedules where, for instance, the theory group is 20 but the lab group (your real enrollment here) is 11, which can be useful if your intended theory group (10 here) is impossible or very inconvenient for you. This is playing a dangerous game, but you can enable it if needed. Disable for efficiency.
6. **Subjects**: find and select all subjects you want to include in your schedules.
7. **Compulsory subjects**: to force a subject to appear in all schedules, click on it again in the `Include in schedules` menu. You can remove a subject from the list doing it again from `Force in all schedules`.

Then, you can generate schedules. An example for a third-year student majoring in Computing would be:

<img src="https://raw.githubusercontent.com/DarkJaslo/Horaris-FIB/master/img/screenshot2.PNG" alt="example" title = "UI" width="99%" height="99%">

Here, the semester is 2023Q1 (fall 2023-24), for the Informatics Engineering major, with 5 subjects, preferring morning and allowing mixed groups.

Since attending PROP is a must to enroll in anything else, it is marked as a forced subject. All other subjects are marked as optional, letting the generator decide which option is best.

The first four (and best) results are:

<div style="display: flex;">
  <img src="https://raw.githubusercontent.com/DarkJaslo/Horaris-FIB/master/img/screenshot3.PNG" alt="example" title = "UI" width="49%">
  <img src="https://raw.githubusercontent.com/DarkJaslo/Horaris-FIB/master/img/screenshot4.PNG" alt="example" title = "UI" width="49%">
</div>
<div style="display: flex;">
  <img src="https://raw.githubusercontent.com/DarkJaslo/Horaris-FIB/master/img/screenshot5.PNG" alt="example" title = "UI" width="49%">
  <img src="https://raw.githubusercontent.com/DarkJaslo/Horaris-FIB/master/img/screenshot6.PNG" alt="example" title = "UI" width="49%">
</div>

See how the first one mixes PAR groups. The last one mixes PAR and PROP groups. Good schedules, right? You can see up to 1000 options, provided they exist. As you go on, schedules start to get worse.

### Efficiency notes

The program is highly combinatorial, so some extreme configurations may halt or run out of memory. If you run into this issue, I recommend, in this order:
1. Waiting for a minute or two.
2. Disabling mixed groups.
3. Making more subjects compulsory.
4. Putting in less subjects.

However, this is usually not a problem, especially if you don't use mixed groups.

### Future upgrades

Being able to explicitly exclude subgroups of selected subjects, so that annoying subgroup you can't attend but keeps getting in all the good schedules disappears.

This won't be working in Windows anytime soon. Not for any particular reason, it's just that Windows and compiling C++ with libraries don't get along. 

## (Español) Generador de horarios para la FIB-UPC 
Genera buenos horarios a partir de una lista de asignaturas. Funciona en Linux (probado solo en Ubuntu).
### Utilidades: 

* Puedes pedir que aparezcan tantas asignaturas como quieras, pues especificas a parte cuáles quieres que salgan obligatoriamente siempre (si las hay).

* Se puede activar la generación de horarios que mezclen grupos de teoría y lab (ej: PAR 23L, pero vas a teoría del PAR 40T)

* Se puede decir si se prefieren horarios de mañanas o tardes (o si no se prefiere nada en particular).

* En general, los horarios buenos saldrán primero. Se usa mi criterio personal para ordenarlos y creo que funciona, pero de todas formas se imprimen hasta 1000 horarios (¡unos cuantos!), así que creo que debería servir a todo el mundo.

### Cómo compilar y ejecutar

(Nota: si has usado `qmake` antes quizás te funcione el setup de qmake, pruébalo una vez comprobados los requisitos. Si no, usa `cmake`.)

Requisitos: `cmake` y `qt5`.

La librería ya incluída `httplib` (https://github.com/yhirose/cpp-httplib) depende de `openSSL`. Es necesaria la versión de desarrollo (en Ubuntu/Debian, `libssl-dev`).

1. Clonar / descargar el repositorio.
```sh
git clone https://github.com/DarkJaslo/Horaris-FIB .
```
2. Compilar.
```sh
cd /path/del/repositorio
mkdir build
cd build
cmake ..
make
```

3. Ejecutar.
```sh
./HorarisFIB
```
4. El programa enseña los horarios a un lado, con botones para moverse entre ellos. Si no aparece ninguno, es seguramente porque ningún horario con esos grupos es compatible.

### Instrucciones de uso

Debes especificar:

1. **Cuatrimestre** (Q1 es otoño y Q2 es la primavera del siguiente año. La FIB no da el otoño siguiente durante la primavera hasta que termina el curso).
2. **Grado** (especialmente si no haces ingeniería informática [GEI]).
3. **Número de asignaturas**: por defecto son 5, pero si quieres 4 o 6 recuerda cambiarlo.
4. **Preferencia**: Mañana -> sacará en general primero los horarios de mañanas; Tarde -> lo mismo pero por la tarde.
5. **Grupos mixtos**: permite generar horarios 'ilegales' donde, por ejemplo, el grupo de teoría que te indica es el 20, pero tu grupo de laboratorio (en el que te matriculas) es el 11, cosa que te puede ser útil si el grupo al que deberías ir (el 10 en este ejemplo) no te es posible o es demasiado incómodo para ti. Esto es jugársela, pues en algunas asignaturas de una forma u otra es obligatorio ir a teoría de tu grupo específicamente, pero la opción está ahí y la puedes activar. Desactiva para mejorar la eficiencia.
6. **Asignaturas**: busca y selecciona todas las asignaturas que quieras incluir.
7. **Asignaturas obligatorias**: para forzar que una asignatura aparezca en todos los horarios, haz click otra vez en ella en el menú `Incluidas en horarios`. Puedes quitar asignaturas de los menús haciendo lo mismo pero desde `Aparecen en todos los horarios`.

Una vez esté todo, genera los horarios. Un ejemplo para un estudiante de tercer año de Computación sería:

<img src="https://raw.githubusercontent.com/DarkJaslo/Horaris-FIB/master/img/screenshot2.PNG" alt="example" title = "UI" width="99%" height="99%">

El semestre es 2023Q1 (otoño 2023-24), el grado es Ingeniería Informática (GEI), con 5 asignaturas, preferencia de mañanas y grupos mixtos activados.

Pongo PROP como obligatoria ya que en Computación hay que matricularla para matricular más asignaturas sí o sí. Pongo el resto como opcionales para ver qué hace el generador.

Los primeros cuatro (y por lo tanto mejores) resultados son:

<div style="display: flex;">
  <img src="https://raw.githubusercontent.com/DarkJaslo/Horaris-FIB/master/img/screenshot3.PNG" alt="example" title = "UI" width="49%">
  <img src="https://raw.githubusercontent.com/DarkJaslo/Horaris-FIB/master/img/screenshot4.PNG" alt="example" title = "UI" width="49%">
</div>
<div style="display:flex;">
  <img src="https://raw.githubusercontent.com/DarkJaslo/Horaris-FIB/master/img/screenshot5.PNG" alt="example" title = "UI" width="49%">
  <img src="https://raw.githubusercontent.com/DarkJaslo/Horaris-FIB/master/img/screenshot6.PNG" alt="example" title = "UI" width="49%">
</div>

Obsérvese que el primer resultado mezcla grupos de PAR, y que el último mezcla grupos tanto de PAR como de PROP. Genera horarios interesantes, ¿eh? Puedes ver hasta 1000 (si los hay), pero irán empeorando a medida que avances.

### Consejos sobre eficiencia

El programa tiene un componente combinatorio bastante importante, así que algunas opciones pueden hacer que se quede colgado o se quede sin memoria. Si te llegara a pasar esto, recomiendo, en este orden:
1. Esperar un poco. Dale un minuto o dos.
2. Desactivar los grupos mixtos.
3. Hacer que más asignaturas salgan siempre.
4. Poner menos asignaturas.

De todas formas, es difícil que eso sea un problema, especialmente si no usas grupos mixtos.

### Futuras actualizaciones

Poder excluir de forma explícita subgrupos de asignaturas seleccionadas.

No está previsto que jamás funcione en Windows. No es por nada en particular, es que es imposible conseguir que compile con las librerías.