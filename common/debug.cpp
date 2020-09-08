//------------------------------------------------------------------------------
///@file debug.cpp                                                              
///                                                                             
///@par Classification:  UNCLASSFIIED, OPEN SOURCE                              
///                                                                             
///@version 2020-05-04  DHF     Open sourced.                                   
//------------------------------------------------------------------------------
#include "debug.h"
#include <algorithm>
#include <stdio.h>

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void debug_hexdump(const void* memory, size_t bytes)
{
    uint8_t* data( (uint8_t*) memory);

    for (size_t i = 0; i < bytes; ++i)
    {
        if ((i % 16) == 0)
        {
            printf("\n%5ld.  ", i);
        } else {
            printf(",  ");
        }
        printf("%02x", data[i]);
    }
    printf("\n");
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void debug_hexdump(const std::string& str)
{
    debug_hexdump(str.c_str(), str.size());
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void debug_diff(const std::string& left, const std::string& right)
{
    size_t min = std::min(left.size(), right.size());

    printf("difference: ");

    bool diff(false);

    for (size_t i = 0; i < min; ++i)
    {
        if (left[i] != right[i])
        {
            diff = true;
            printf("%ld,  ", i);
        }
    }

    if (left.size() != min)  diff = true, printf("left shorter");
    if (right.size() != min) diff = true, printf("right shorter");

    if (!diff) printf("same");


    printf("\n");
}
