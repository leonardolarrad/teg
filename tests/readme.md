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
| `concepts` | Pruebas de Conceptos (C++20 Concepts) utilizados por la librería. |
| `reflection` | Pruebas de reflección estática (reflección en tiempo de compilación). |
| `overload_resolution` | Resolución de sobrecarga de funciones a través de Conceptos. |
| `fundamental` | Serialización de tipos de datos fundamentales (ej. `bool`, `char`, `unsigned int`, etc.) |
| `carray` | Serialización de arreglos de C. |
| `optional` | Serialización de tipos de datos opcionales (ej. `std::optional`). |
| `container` | Serialización básica de contenedores de la librería estándar. |
| `vector` | De/serialización detallada del contenedor estándar `std::vector`. |
| `string` | De/serialización de cadenas de caracteres (ej. `std::string`, `std::u16string`, `std::u32string`, etc.). |
| `set` | Serialización de conjuntos `std::set`, `std::unordered_set`, `std::multiset` y `std::unordered_multiset`. |
| `map` | Serialización de mapas `std::map`, `std::unordered_map` y `std::multimap` y `std::unordered_multimap`. |
| `owning_pointer` | Serialización de punteros propietarios (específicamente `std::unique_ptr` y `std::shared_ptr`). |
| `tuple` | Serialización de tuplas (específicamente `std::tuple` y `std::pair`). |
| `variant` | Serialización de variantes; unión discriminada de tipos de datos (específicamente `std::variant`). |