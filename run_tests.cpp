#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <string>

namespace fs = std::filesystem;

int main() {
    std::string test_dir = "..test_cases";
    std::string bin_path = "../cmake-build-debug/Proyect.exe"; // O "./build/Proyect" si usas CMake
    std::string output_dir = "../asembly";

    // Crear carpeta de salida si no existe
    fs::create_directories(output_dir);

    // Recorrer todos los archivos .txt en test_cases
    for (const auto& entry : fs::directory_iterator(test_dir)) {
        if (entry.path().extension() == ".txt") {
            std::string input_path = entry.path().string();
            std::string command = bin_path + " " + input_path;
            std::cout << "== Ejecutando test: " << input_path << std::endl;
            int result = system(command.c_str());
            if (result != 0) {
                std::cerr << ">> Falló el test: " << input_path << std::endl;
            }
            std::cout << "-----------------------------" << std::endl;
        }
    }

    std::cout << "✅ Todos los tests ejecutados." << std::endl;
    return 0;
}
