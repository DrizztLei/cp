//
// Created by elvis on 11/9/17.
//

#ifndef PL0_BUFFER_MANAGER_H
#define PL0_BUFFER_MANAGER_H

#include <iostream>
#include <cstring>
#include <fstream>
#include <ostream>
#include <boost/thread/thread.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>

// #include "pl0.h"

class buffer_manager
{
public:
    void test();
    explicit buffer_manager();
    explicit buffer_manager(std::string path);
    void init_thread();
    void run_thread();
    
    void set_argc(int argc);
    void set_argv(char** argv);
    
    static bool load_into_text_a(std::string& result);
    static bool load_into_text_b(std::string& result);
    
    static void backstage_b();
    static void backstage_a();
    
    static void start_thread_load_content();
    
    void set_path(std::string path);
    
    void wait_thread();
    
    virtual ~buffer_manager();
    
    static void create_buffer();
    static void output(std::string info);
    
    static const int length = 1024;
    
    static boost::interprocess::interprocess_semaphore* semaphore_a_enable_use;
    static boost::interprocess::interprocess_semaphore* semaphore_a_enable_fill;
    static boost::interprocess::interprocess_semaphore* semaphore_b_enable_use;
    static boost::interprocess::interprocess_semaphore* semaphore_b_enable_fill;
    static boost::interprocess::interprocess_semaphore* semaphore_control;
    static boost::interprocess::interprocess_semaphore* semaphore_thread_control;
    
    static boost::thread backstage_thread_text_a;
    static boost::thread backstage_thread_text_b;
    
    static FILE* buffer_point_file;
    static std::ifstream file_stream;
    static boost::thread* main_thread;
    
    static std::string path;
    static char text_a[];
    static char text_b[];
    
    static void process_getsym(std::string info);
    
    static int argc;
    static char** argv;
};

#endif //PL0_BUFFER_MANAGER_H
