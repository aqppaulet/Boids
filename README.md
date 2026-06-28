# Lab 10 - Boids, inicio

Este proyecto implementa los primeros puntos del laboratorio:

1. Estructura `Boid` con posicion, velocidad y direccion.
2. Inicializacion de 50 boids en posiciones aleatorias dentro de la ventana.
3. Representacion visual con orientacion igual a la direccion de movimiento.

El codigo usa `Vec3` desde el inicio para que el paso de 2D a 3D sea simple.
El modo de visualizacion se controla desde `include/Config.h`:

```cpp
inline constexpr bool USE_3D = false;
```

Cambia ese valor a `true` para usar posiciones y dibujo 3D:

```cpp
inline constexpr bool USE_3D = true;
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

En modo 3D la camara gira lentamente alrededor de la escena. La caja gris marca
el volumen donde se inicializan los boids y los ejes de colores ayudan a notar
la profundidad.

Este primer avance todavia no implementa movimiento continuo ni las reglas de separacion,
alineamiento y cohesion; eso corresponde a los siguientes puntos del laboratorio.
