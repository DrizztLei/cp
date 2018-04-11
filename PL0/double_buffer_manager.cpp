//
// Created by elvis on 11/7/17.
//

#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>

#include <cstring>
#include <stdio.h>

using namespace std;
using namespace boost;

boost::thread buffer_thread;

boost::interprocess::interprocess_semaphore semaphore_a_enable_use(0);
boost::interprocess::interprocess_semaphore semaphore_a_enable_fill(0);
boost::interprocess::interprocess_semaphore semaphore_b_enable_use(0);
boost::interprocess::interprocess_semaphore semaphore_b_enable_fill(0);

boost::interprocess::interprocess_semaphore semaphore_control(0);

string file_path;

const int read_length = 1024;

char double_buffer_manager_buffer_a[read_length];
char double_buffer_manager_buffer_b[read_length];

FILE* buffer_point_file;

void load_into_a()
{
    if(NULL == fgets(double_buffer_manager_buffer_a, read_length, buffer_point_file))
    {
        cout << "ERROR FOR READ IN A " << endl;
    }
}

void load_into_b()
{
    if(NULL == fgets(double_buffer_manager_buffer_b, read_length, buffer_point_file))
    {
        cout << "ERROR FOR READ IN B " << endl;
    }
}

void load_into_buffer()
{
    buffer_point_file = fopen(file_path.c_str(), "r");
    load_into_a();
    load_into_b();
}

void print_buffer_a()
{

}

void print_buffer_b()
{

}

void buffer_a()
{
    while(true)
    {
        semaphore_a_enable_fill.wait();
        load_into_a();
        semaphore_a_enable_use.post();
    }
}

void buffer_b()
{
    while(true)
    {
        semaphore_b_enable_fill.wait();
        load_into_b();
        semaphore_b_enable_use.post();
    }
}

void create_double_buffer()
{
    load_into_buffer();
    
    boost::thread buffer_a(&buffer_a);
    boost::thread buffer_b(&buffer_b);
    
    semaphore_control.post();
}

void test(std::string path)
{
    file_path = path;
    buffer_thread = boost::thread(create_double_buffer) ;
    
    semaphore_control.wait();
    
    while(rand() < 10)
    {
        semaphore_a_enable_use.wait();
        print_buffer_a();
        semaphore_a_enable_fill.post();
        
        semaphore_b_enable_use.wait();
        print_buffer_b();
        semaphore_b_enable_fill.post();
    }
    sleep(2);
    std::cout << "done " << std::endl;
    buffer_thread.join();
}

void collect()
{

}
