#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "scanner.h"
#include "parser.h"
#include "visitor.h"

using namespace std;

int main(int argc, const char* argv[]) {
    if (argc < 2 || argc > 3) {
        cout << "Uso: " << argv[0] << " <archivo_de_entrada> [--frontend]" << endl;
        return 1;
    }

    string input_path = argv[1];
    bool frontend_mode = (argc == 3 && string(argv[2]) == "--frontend");

    ifstream infile(input_path);
    if (!infile.is_open()) {
        cerr << "No se pudo abrir el archivo: " << input_path << endl;
        return 1;
    }

    string input((istreambuf_iterator<char>(infile)), istreambuf_iterator<char>());
    infile.close();

    // Scanner
    Scanner scanner(input.c_str());
    Scanner scanner_test(input.c_str());
    test_scanner(&scanner_test);
    cout << "Scanner exitoso\n\nIniciando parsing:" << endl;

    Parser parser(&scanner);
    try {
        Program* program = parser.parseProgram();
        cout << "Parsing exitoso\n\nIniciando Visitor:\nTYPE CHECK:" << endl;

        TYPEVisitor typeVisitor;
        typeVisitor.visit(program);
        cout << "Type checking exitoso\n" << endl;

        PrintVisitor printVisitor;
        EVALVisitor evalVisitor;

        cout << "IMPRIMIR:" << endl;
        printVisitor.imprimir(program);
        cout << "\nEJECUTAR:" << endl;
        evalVisitor.ejecutar(program);

        // Ruta de salida del ASM
        std::filesystem::path path(input_path);
        std::string base_name = path.stem().string();

        std::string output_path = frontend_mode
            ? "project/build/" + base_name + ".s"
            : "assembly/" + base_name + ".s";

        if (frontend_mode)
            std::cout << "[DEBUG] Modo FRONTEND: escribiendo en " << output_path << std::endl;
        else
            std::cout << "[DEBUG] Modo NORMAL: escribiendo en " << output_path << std::endl;

        // Asegura que el directorio exista
        std::filesystem::create_directories(std::filesystem::path(output_path).parent_path());

        std::ofstream asmOut(output_path);
        if (!asmOut.is_open()) {
            cerr << "No se pudo crear el archivo ASM: " << output_path << endl;
            return 1;
        }

        CodeGenVisitor codeGen(asmOut);
        codeGen.generate(program);
        asmOut.close();

        cout << "Ensamblador generado en: " << output_path << endl;

        delete program;
    } catch (const exception& e) {
        cerr << "Error durante la ejecución: " << e.what() << endl;
        return 1;
    }

    return 0;
}
