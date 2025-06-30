#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include "scanner.h"
#include "parser.h"
#include "visitor.h"

namespace fs = std::filesystem;

int run_test_with_file(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "❌ No se pudo abrir el archivo: " << filename << "\n";
        return 1;
    }

    std::string input, line;
    while (std::getline(infile, line)) {
        input += line + '\n';
    }
    infile.close();

    try {
        Scanner scanner(input.c_str());
        Scanner scanner_test(input.c_str());
        test_scanner(&scanner_test);

        std::cout << "\n📌 Ejecutando: " << filename << std::endl;
        Parser parser(&scanner);
        Program* program = parser.parseProgram();

        TYPEVisitor typeVisitor;
        typeVisitor.visit(program);

        PrintVisitor printVisitor;
        EVALVisitor evalVisitor;

        printVisitor.imprimir(program);
        evalVisitor.ejecutar(program);

        fs::path path(filename);
        std::string output_filename = "../asembly/" + path.stem().string() + ".s";
        std::ofstream asmOut(output_filename);
        CodeGenVisitor codeGen(asmOut);
        codeGen.generate(program);
        asmOut.close();

        std::cout << "✅ Ensamblador generado en: " << output_filename << "\n";
        delete program;
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "❌ Error: " << e.what() << "\n";
        return 1;
    }
}

int main() {
    std::string test_dir = "../test_cases";
    if (!fs::exists(test_dir)) {
        std::cerr << "❌ Carpeta 'test_cases' no encontrada.\n";
        return 1;
    }

    for (const auto& entry : fs::directory_iterator(test_dir)) {
        if (entry.path().extension() == ".txt") {
            run_test_with_file(entry.path().string());
        }
    }

    return 0;
}
