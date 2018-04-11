//
// Created by elvis on 11/9/17.
//

#include "buffer_manager.h"
#include "pl0.c"

using namespace boost::interprocess;
using namespace boost;

// const int buffer_manager::length = 1024;

char buffer_manager::text_a[buffer_manager::length];
char buffer_manager::text_b[buffer_manager::length];

int buffer_manager::argc = 0;
char** buffer_manager::argv = NULL;

interprocess_semaphore *buffer_manager::semaphore_a_enable_use = new interprocess_semaphore(0);
interprocess_semaphore *buffer_manager::semaphore_a_enable_fill = new interprocess_semaphore(0);
interprocess_semaphore *buffer_manager::semaphore_b_enable_use = new interprocess_semaphore(0);
interprocess_semaphore *buffer_manager::semaphore_b_enable_fill = new interprocess_semaphore(0);
interprocess_semaphore *buffer_manager::semaphore_control = new interprocess_semaphore(0);
interprocess_semaphore *buffer_manager::semaphore_thread_control = new interprocess_semaphore(0);

thread buffer_manager::backstage_thread_text_a;
thread buffer_manager::backstage_thread_text_b;

std::string buffer_manager::path;
FILE *buffer_manager::buffer_point_file;
std::ifstream buffer_manager::file_stream;
thread *buffer_manager::main_thread;

void buffer_manager::test()
{
    char temp[128];
    for(int i = 0; i < 1; i++)
    {
        temp[i] = 'a';
    }
}

buffer_manager::~buffer_manager()
{
    // std::cout << "run in dis construct function." << std::endl;
    
    if(semaphore_a_enable_fill != nullptr)
    {
        delete (semaphore_a_enable_use);
        semaphore_a_enable_use = nullptr;
    }
    
    if(semaphore_b_enable_fill != nullptr)
    {
        delete (semaphore_b_enable_fill);
        semaphore_b_enable_fill = nullptr;
    }
    
    if(semaphore_a_enable_fill != nullptr)
    {
        delete (semaphore_a_enable_fill);
        semaphore_a_enable_fill = nullptr;
    }
    
    if(semaphore_b_enable_use != nullptr)
    {
        delete (semaphore_b_enable_use);
        semaphore_b_enable_use = nullptr;
    }
    
    if(semaphore_control != nullptr)
    {
        delete (semaphore_control);
        semaphore_control = nullptr;
    }
    
    if(main_thread != nullptr)
    {
        delete (main_thread);
        main_thread = nullptr;
    }
    
    if(semaphore_thread_control != nullptr)
    {
        delete (semaphore_thread_control);
        semaphore_thread_control = nullptr;
    }
}


buffer_manager::buffer_manager()
{
    using namespace boost::interprocess;
}

buffer_manager::buffer_manager(std::string path)
{
    using namespace boost::interprocess;
    
    buffer_manager::path = path;
}

void buffer_manager::init_thread()
{
    using namespace boost;
    main_thread = new thread(&buffer_manager::create_buffer);
    main_thread->join();
}

void buffer_manager::run_thread()
{
    std::string result = "";
    bool flag;
    
    while(true)
    {
        semaphore_a_enable_use->wait();
        flag = load_into_text_a(result);
        semaphore_a_enable_fill->post();
        
        process_getsym(result);
        
        if(flag == false)
        {
            break;
        }
        
        semaphore_b_enable_use->wait();
        flag = load_into_text_b(result);
        semaphore_b_enable_fill->post();
        
        process_getsym(result);
        
        if(flag == false)
        {
            break;
        }
    }
}

void buffer_manager::create_buffer()
{
    using namespace boost::interprocess;
    using namespace boost;
    
    // buffer_point_file = fopen(buffer_manager::path.c_str(), "r");
    file_stream.open(path, std::ios::in);
    
    backstage_thread_text_a = thread(bind(&buffer_manager::backstage_a));
    backstage_thread_text_b = thread(bind(&buffer_manager::backstage_b));
    
    semaphore_thread_control->wait();
}

void buffer_manager::set_path(std::string path)
{
    buffer_manager::path = path;
}

void buffer_manager::wait_thread()
{
    main_thread->join();
    backstage_thread_text_a.interrupt();
    backstage_thread_text_b.interrupt();
}

void buffer_manager::output(std::string info)
{
    std::cout << info << std::endl;
}

bool buffer_manager::load_into_text_a(std::string &result)
{
    result = text_a;
    
    // std::cout << result.length() << std::endl;
    // std::cout << strlen(text_a) << std::endl;
    
    if(result.length() != length - 1)
    {
        return false;
    }
    return true;
}

bool buffer_manager::load_into_text_b(std::string &result)
{
    result = text_b;
    
    if(result.length() != length - 1)
    {
        return false;
    }
    return true;
}

void buffer_manager::backstage_a()
{
    file_stream.get(text_a, length, '\0');
    // bool first_read_result = (file_stream.gcount() == length  - 1);
    
    semaphore_control->post();
    semaphore_a_enable_use->post();
    
    if(file_stream.eof())
    {
        return;
    }
    
    while(true)
    {
        semaphore_a_enable_fill->wait();
        
        file_stream.get(text_a, length, '\0');
        
        // bool read_done = (file_stream.gcount() == length - 1);
        
        semaphore_control->post();
        semaphore_a_enable_use->post();
        
        if(file_stream.eof())
        {
            // output("end here for a");
            break;
        }
    }
}

void buffer_manager::backstage_b()
{
    semaphore_control->wait();
    
    file_stream.get(text_b, length, '\0');
    // bool first_read_result = (file_stream.gcount() == length - 1);
    
    semaphore_control->post();
    semaphore_b_enable_use->post();
    semaphore_thread_control->post();
    
    if(file_stream.eof())
    {
        return;
    }
    
    while(true)
    {
        semaphore_b_enable_fill->wait();
        semaphore_control->wait();
        
        file_stream.get(text_b, length, '\0');
        semaphore_b_enable_use->post();
        
        if(file_stream.eof())
        {
            // output("end b here");
            break;
        }
    }
}

void buffer_manager::start_thread_load_content()
{
    semaphore_a_enable_fill->post();
    semaphore_b_enable_fill->post();
}

void buffer_manager::process_getsym(std::string info)
{
    char character;
    for(int i = 0; i < info.length(); i++)
    {
        character = info.at(i);
    }
    
    int return_value = pl0_main(buffer_manager::argc, buffer_manager::argv);
    assert(return_value == 0);
}

void buffer_manager::set_argc(int argc)
{
    this->argc = argc;
}

void buffer_manager::set_argv(char **argv)
{
    this->argv = argv;
}