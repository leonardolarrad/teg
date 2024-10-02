# TEG
Trabajo Experimental de Grado.

## INSTALACIÓN

### Prerequisitos

* Compilador C++20: MSVC 14.20+, GCC 8.0+ o Clang 9.0+.
* [Meson](https://mesonbuild.com/): Sistema de construcción utilizado en el proyecto.

### Compilación

```bash
# Configurar el proyecto en el directorio 'build/release'.
meson setup build/release/ --buildtype=release

# Compilar la librería.
meson compile -C build//release/
```

## PRUEBAS

El directorio [`tests/`](tests/) contiene un conjunto de pruebas desarrolladas para la asercicón de las funcionalidades de la librería. Dírigase al directorio para más información sobre la tabla de pruebas y cómo ejecutarlas.
