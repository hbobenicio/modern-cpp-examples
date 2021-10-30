/**
 * ## Build
 * 
 * ### Listando as opções de linkagem/implementação
 * 
 * ```
 * ls -l /lib/x86_64-linux-gnu/libboost_stacktrace*
 * ```
 * 
 * Você deve obter estas flags:
 * 
 * ```
 * -lboost_stacktrace_addr2line (linhas de fontes com paths globais)
 * -lboost_stacktrace_backtrace (linhas de fontes com paths relativos)
 * -lboost_stacktrace_basic (sem linhas de fontes)
 * -lboost_stacktrace_noop
 * ```
 * 
 * ### Exemplos de compilação
 * 
 * ```
 * clang++ -g -Wall -pedantic -std=c++17 -o build/boost-program-options-example -ldl -lboost_program_options -lboost_stacktrace_backtrace src/main.cpp && ./build/boost-program-options-example
 * clang++ -g -Wall -pedantic -std=c++17 -o build/boost-program-options-example -ldl -lboost_program_options -lboost_stacktrace_addr2line src/main.cpp && ./build/boost-program-options-example
 * clang++ -g -Wall -pedantic -std=c++17 -o build/boost-program-options-example -ldl -lboost_program_options -lboost_stacktrace_basic src/main.cpp && ./build/boost-program-options-example
 * ```
 * 
 * # Dependencias
 * 
 * - libboost-program-options-dev
 * - libboost-stacktrace-dev
 */
#include <iostream>
#include <signal.h>

#define BOOST_STACKTRACE_LINK
#include <boost/stacktrace.hpp>
#include <boost/program_options.hpp>

using namespace std;
namespace po = boost::program_options;

static void signal_handler_print_stacktrace(int signal_num);

int main(int argc, char** argv) {
    // SIGABRT: ?
    signal(SIGABRT, &signal_handler_print_stacktrace);

    // SIGSEGV: Invalid memory segment access (ANSI)
    signal(SIGSEGV, &signal_handler_print_stacktrace);

    po::options_description description{ "Allowed Options" };
    description.add_options()
        ("help", "Help message")
        ("verbose,v", po::bool_switch()->default_value(false), "Turn on verbosity")
        ("host,h", po::value<string>()->default_value("localhost"), "Server host/ip")
        ("port,p", po::value<unsigned short>()->default_value(443), "Server port")
    ;

    po::variables_map input_map;

    const auto cli_style
        = po::command_line_style::allow_long // Allow "--long_name" style
        | po::command_line_style::long_allow_adjacent // Allow option parameter in the same token for long option like in --foo=10
        | po::command_line_style::long_allow_next // Allow option parameter in the next token for long options
        | po::command_line_style::allow_short // Allow "-single character" style
        | po::command_line_style::allow_dash_for_short // Allow "-" in short options
        | po::command_line_style::short_allow_next // Allow option parameter in the next token for short options
        ;

    try {
        po::store(po::parse_command_line(argc, argv, description, cli_style), input_map);
    } catch (const po::error& err) {
        cerr << "error: " << err.what() << '\n';
        return EXIT_FAILURE;
    }

    po::notify(input_map);

    //throw runtime_error("fuu");

    if (input_map.count("help") > 0) {
        cerr << description << '\n';
        return EXIT_SUCCESS;
    }
    if (input_map["verbose"].as<bool>()) {
        cerr << "info: setting verbosity output.\n";
    }

    string host = input_map["host"].as<string>();
    unsigned short port = input_map["port"].as<unsigned short>();

    cout << "server address: " << host << ":" << port << '\n';

    return 0;
}

static void signal_handler_print_stacktrace(int signal_num) {
    cerr << "\nstacktrace:\n" << boost::stacktrace::stacktrace() << '\n';
}
