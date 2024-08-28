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
| `reflection` | Pruebas del módulo de reflección estática (tiempo de compilación). |
| `fundamental` | Pruebas de serialización y deserialización de tipos fundamentales. |
| `carray` | Pruebas de serialización y deserialización de arreglos de C. |
| `container` | Pruebas de serialización y deserialización de contenedores. |