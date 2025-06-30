#include <iostream>
#include <fstream>
#include <string>
#include "scanner.h"
#include "parser.h"
#include "visitor.h"
#include <filesystem>
using namespace std;

int main(int argc, const char* argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <archivo_entrada>\n";
        return 1;
    }

    std::ifstream infile(argv[1]);
    if (!infile.is_open()) {
        std::cerr << "No se pudo abrir el archivo: " << argv[1] << "\n";
        return 1;
    }

    string input;
    string line;
    while (getline(infile, line)) {
        input += line + '\n';
    }
    infile.close();

    Scanner scanner(input.c_str());

    string input_copy = input;
    Scanner scanner_test(input_copy.c_str());
    test_scanner(&scanner_test);
    cout << "Scanner exitoso" << endl;
    cout << endl;
    cout << "Iniciando parsing:" << endl;
    Parser parser(&scanner);
    try {
        Program* program = parser.parseProgram();
        cout << "Parsing exitoso" << endl << endl;
        cout << "Iniciando Visitor:" << endl;
        TYPEVisitor typeVisitor;
        cout << "TYPE CHECK:" << endl;
        typeVisitor.visit(program);
        cout << "Type checking exitoso" << endl << endl;


        PrintVisitor printVisitor;
        EVALVisitor evalVisitor;

        cout << "IMPRIMIR:" << endl;
        printVisitor.imprimir(program);
        cout  << endl;
        cout << "EJECUTAR:" << endl;
        evalVisitor.ejecutar(program);

        std::string input_filename(argv[1]);
        std::filesystem::path path(input_filename);
        std::string output_filename = "../asembly/"+path.stem().string() + ".s";

        std::ofstream asmOut(output_filename);
        CodeGenVisitor codeGen(asmOut);
        codeGen.generate(program);
        asmOut.close();
        std::cout << ">> Ensamblador generado en "<<output_filename << std::endl;
        delete program;
    } catch (const exception& e) {
        cout << "Error durante la ejecución: " << e.what() << endl;
        return 1;
    }

    return 0;
}