# Lab 10 - Boids, inicio

Este proyecto implementa los primeros puntos del laboratorio:

1. Estructura `Boid` con posicion, velocidad y direccion.
2. Inicializacion de 50 boids en posiciones aleatorias dentro de la ventana.
3. Representacion visual con orientacion igual a la direccion de movimiento.
4. Movimiento continuo y reglas de separacion, alineamiento y cohesion.
5. Combinacion ponderada de las tres reglas.
6. Limites de velocidad minima y maxima.
7. Comportamiento en bordes: rebote o mundo toroidal.

El codigo usa `Vec3` desde el inicio para que el paso de 2D a 3D sea simple.
El modo de visualizacion se controla desde `include/Config.h`:

```cpp
inline constexpr bool USE_3D = false;
```

Cambia ese valor a `true` para usar posiciones y dibujo 3D:

```cpp
inline constexpr bool USE_3D = true;
```

El comportamiento en los bordes tambien se controla desde `include/Config.h`:

```cpp
inline constexpr BoundaryMode BOUNDARY_MODE = BoundaryMode::Toroidal;
```

Puedes cambiarlo por:

```cpp
inline constexpr BoundaryMode BOUNDARY_MODE = BoundaryMode::Bounce;
```

## Compilar con CMake

Necesitas OpenGL y GLUT/freeglut instalados.

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

En Windows, si usas MinGW y freeglut, tambien puedes compilar de forma directa:

```bash
g++ src/*.cpp -Iinclude -lfreeglut -lopengl32 -lglu32 -o Lab10Boids.exe
```

## Controles

- `Esc`: salir.
- `1` / `2`: disminuir / aumentar numero de boids.
- `3` / `4`: disminuir / aumentar distancia de vecindad.
- `5` / `6`: disminuir / aumentar intensidad de separacion.
- `7` / `8`: disminuir / aumentar intensidad de alineamiento.
- `9` / `0`: disminuir / aumentar intensidad de cohesion.
- En modo 3D, arrastrar con click izquierdo: rotar camara.
- En modo 3D, rueda del mouse: acercar o alejar.

En modo 3D la caja gris marca el volumen donde se inicializan los boids y los
ejes de colores ayudan a notar la profundidad.

Los pesos de las reglas se pueden modificar en `include/Config.h`, dentro de
`SIMULATION`.

Los boids que se agregan durante la ejecucion aparecen en amarillo por unos
segundos. La duracion se controla desde `include/Config.h`:

```cpp
inline constexpr float NEW_BOID_HIGHLIGHT_SECONDS = 2.0f;
```

La combinacion del comportamiento colectivo se realiza en `src/Flock.cpp`:

```cpp
separacion * separationWeight
+ alineamiento * alignmentWeight
+ cohesion * cohesionWeight
```

Los limites de velocidad tambien se ajustan en `include/Config.h`:

```cpp
8.0f,   // minSpeed
22.0f,  // maxSpeed
```
