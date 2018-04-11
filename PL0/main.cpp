#include <cstring>
#include <iostream>

#include "buffer_manager.h"
// #include "pl0.c"

using namespace std;

std::string input_file("/home/elvis/work/cp/PL0/input.txt");

int main(int argc, char **argv)
{
    int const pl0_argc = 2;
    argv[1] = input_file.c_str();
    
    buffer_manager x(input_file);
    
    x.set_argc(argc);
    x.set_argv(argv);
    x.init_thread();
    x.run_thread();
    
    // int return_value = pl0_main(pl0_argc, argv);
    return EXIT_SUCCESS;
}