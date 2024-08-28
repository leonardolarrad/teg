# TEG
Trabajo Experimental de Grado.

## INSTALACIÓN

### Prerequisitos

* Compilador C++20: MSVC 14.20+, GCC 8.0+ o Clang 9.0+.
* [Meson](https://mesonbuild.com/): _build system_ utilizado en el proyecto.

### Compilación

```bash
# Configurar el proyecto en la carpeta 'build/'
meson setup build/

# Compilar la librería.
meson compile -C build/
```

## PRUEBAS UNITARIAS Y DE INTEGRACIÓN

El directorio [`tests/`](tests/) contiene un conjunto de pruebas unitarias y de integración desarrolladas para la asercicón de las funcionalidades de la librería. Dírigase al directorio para más información sobre la tabla de pruebas y cómo ejecutarlas.
