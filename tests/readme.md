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
| `concepts` | Pruebas de conceptos (_C++20 Concepts_) utilizados por la librería para realizar aserciones y resolución de sobrecarga de funciones. |
| `reflection` | Pruebas de reflección estática (tiempo de compilación). |
| `buffer` | Computo del tamaño en memoria ocupado por datos serializables. |
| `fundamental` | Serialización de tipos de datos fundamentales. |
| `carray` | Serialización de arreglos de C. |
| `container` | Serialización básica de contenedores de la librería estándar. |
| `vector` | Serialización detallada del contenedor estándar `std::vector`. |
| `string` | Serialización de cadenas de caracteres UTF-8, UTF-16 y UTF-32. |