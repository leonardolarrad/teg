## PRUEBAS

Se han desarrollado diversas pruebas unitarias y pruebas de integración; el código fuente de las mismas se encuentra en el directorio `src/`.

> [!NOTE]
> En el directorio `include/` se encuentran las herramientas de testing utilizadas. Este proyecto no tiene dependencias externas, sino que utiliza su propio framework de pruebas.

### EJECUCIÓN

```bash
# Configurar el proyecto en el directorio `build/debug/` en modo `debug`.
meson setup build/debug/

# Ejecutar todas las pruebas.
# La bandera opcional `-v` activa el modo verbose.
meson test -v -C build/debug/

# Ejecutar una prueba en isolación.
# Se reemplaza <prueba> con el nombre de una prueba existente (véase tabla de pruebas).
meson test <prueba> -v -C build/debug/
```

### TABLA DE PRUEBAS

| Prueba | Descripción |
| --- | --- |
| `concepts` | Pruebas de Conceptos (C++20 _Concepts_) utilizados por la librería. |
| `reflection` | Pruebas de reflección estática (reflección en tiempo de compilación). |
| `overload_resolution` | Resolución de sobrecarga de funciones a través de Conceptos. |
| `fundamental` | Serialización de tipos de datos fundamentales (ej. `bool`, `char`, `unsigned int`, etc.) |
| `carray` | De/serialización de arreglos tipo C. |
| `optional` | De/serialización de tipos de datos opcionales (específicamente `std::optional`). |
| `container` | De/serialización básica de contenedores. |
| `vector` | De/serialización detallada del contenedor estándar `std::vector`. |
| `string` | De/serialización de cadenas de caracteres (ej. `std::string`, `std::u16string`, `std::u32string`, etc.). |
| `set` | De/serialización de conjuntos `std::set`, `std::unordered_set`, `std::multiset` y `std::unordered_multiset`. |
| `map` | De/serialización de tablas hash `std::map`, `std::unordered_map` y `std::multimap` y `std::unordered_multimap`. |
| `owning_pointer` | De/serialización de punteros propietarios (específicamente `std::unique_ptr` y `std::shared_ptr`). |
| `tuple` | De/serialización de tuplas (específicamente `std::tuple` y `std::pair`). |
| `variant` | De/serialización de variantes; unión discriminada de tipos de datos (específicamente `std::variant`). |
| `aggregates` | De/serialización de agregados. Pruebas de integración de serialización por componentes. |

### RESULTADOS

Si todas las pruebas han sido exitosas, la salida del terminal se mostrará de la siguiente manera:

```
 1/17 reflection                  OK              0.03s
 2/17 overload_resolution         OK              0.02s
 3/17 concepts                    OK              0.05s
 4/17 type_code                   OK              0.03s
 5/17 fundamental                 OK              0.02s
 6/17 optional                    OK              0.02s
 7/17 carray                      OK              0.03s
 8/17 vector                      OK              0.02s
 9/17 container                   OK              0.04s
10/17 string                      OK              0.03s
11/17 fixed_string                OK              0.02s
12/17 map                         OK              0.02s
13/17 set                         OK              0.03s
14/17 owning_pointer              OK              0.02s
15/17 tuple                       OK              0.02s
16/17 aggregates                  OK              0.02s
17/17 variant                     OK              0.01s

Ok:                 17
Expected Fail:      0
Fail:               0
Unexpected Pass:    0
Skipped:            0
Timeout:            0
```