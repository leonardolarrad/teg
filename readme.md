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

# Compilar la librería y las pruebas.
meson compile -C build/
```
> [!TIP]
> Si utilizas Visual Studio Code bastará con presionar F5 para compilar la librería.

## PRUEBAS

En la carpeta `tests` enncontrará un conjunto de pruebas. 
```bash
# Ejecutar todas las pruebas,
meson test -C build/

# Ejecutar una única prueba en isolación.
meson test <nombre de la prueba> -C build/
```

## LICENCIA

Copyright (c) 2024 Adrian & Leonardo Larrad.

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
