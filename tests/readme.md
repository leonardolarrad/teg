## PRUEBAS
Pruebas unitarias y de integración para la aserción de las funcionalidades de la librería.

Nota: en la carpeta `include/` se encuentran las herramientas de testing utilizadas. Este proyecto no utiliza dependencias externas sino que tiene construido _built-in_ su propio framework de pruebas.

### EJECUCIÓN

```bash
# Ejecutar todas las pruebas
# La bandera opcional `-v` activa el modo verbose
meson test -v -C build/

# Ejecutar una prueba en isolación
# Se reemplaza <prueba> con el nombre de una prueba existente
meson test <prueba> -v -C build/
```

### TABLA DE PRUEBAS

| Prueba | Descripción |
| --- | --- |
| `concepts` | Pruebas de _Conceptos_ (_C++20 Concepts_) utilizados por la librería. |
| `reflection` | Pruebas de reflección estática (reflección en tiempo de compilación). |
| `overload_resolution` | Resolución de sobrecargas de funciones con conceptos. |
| `buffer` | Computo del tamaño en memoria ocupado por datos serializables. |
| `fundamental` | Serialización de tipos de datos fundamentales. |
| `carray` | Serialización de arreglos de C. |
| `optional` | Serialización de tipos de datos opcionales. |
| `container` | Serialización básica de contenedores de la librería estándar. |
| `vector` | Serialización detallada del contenedor estándar `std::vector`. |
| `string` | Serialización de cadenas de caracteres UTF-8, UTF-16 y UTF-32. |
| `set` | Serialización de conjuntos `std::set`, `std::unordered_set`, `std::multiset` y `std::unordered_multiset`. |
| `map` | Serialización de mapas `std::map`, `std::unordered_map` y `std::multimap` y `std::unordered_multimap`. |