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