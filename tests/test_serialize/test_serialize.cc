// Copyright (c) 2024 Adrian & Leonardo Larrad.
// 
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
// 
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include <iostream>
#include <vector>
#include <larrad/larrad.h>

struct vec2 {
    float x, y;
};

int main() {
    // Creamos un objeto a serializar
    auto v = vec2 { 1.f, 2.f };
    larrad::buffer buffer;

    // Serializamos
    auto r = larrad::serialize(buffer, v);

    if (larrad::failure(r)) {
        std::cout << "Seralization has failed" << std::endl;
        return 1;
    }

    // Printeamos el buffer de datos
    std::cout << "buffer size: " << buffer.size() << std::endl;
    for (const auto b : buffer) {
        // print byte a byte
        std::cout << (int)b << " ";
    }

    // --------------------------------------

    // Y ahora deserializamos
    float x, y;
    auto r2 = larrad::deserialize(buffer, x, y);
    
    if (larrad::failure(r2)) {
        std::cout << "Deserialization has failed" << std::endl;
        return 1;
    }

    // Printeamos los valores deserializados
    std::cout << "x: " << x << " y: " << y << std::endl;

    return 0;
}