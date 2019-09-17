/**
 * This example demonstrates how to create a directory using the new std::filesystem library
 * (still experimental thrue std::experimental::filesystem)
 *
 * ## Compilation Instructions
 *
 * - GCC 7: `g++ -Wall -pedantic -std=c++17 main.cpp -o main -lstdc++fs`
 * - GCC 8: `g++ -Wall -pedantic -std=c++17 main.cpp -o main`
 */

#include <iostream>
#include <system_error> // for std::error_code, in case you use the overload of create_directory(dir, err_code)

#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

int main() {
    const fs::path bar{"/tmp/baz"};
    try {
        if (fs::create_directory(bar)) {
            std::cout << "diretório criado com sucesso\n";
        } else {
           std::cout << "diretório " << bar << " já existe\n";
        }
    } catch (const fs::filesystem_error& fs_err) {
        std::cerr << "erro ao criar diretório " << bar << '\n';
        std::cerr << "causado por: " << fs_err.what() << '\n';
    }
}
