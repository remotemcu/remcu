#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <cstring>
#include <assert.h>
#include <vector>

#include "exports.h"

namespace remcu {

static ErrorSignalFunc error_callback = nullptr;

static size_t error_count = 0;

void setErrorSignalFunc(ErrorSignalFunc callback){
    error_callback = callback;
}

size_t getErrorCout(){
    return error_count;
}

void clearErrorCount(){
    error_count = 0;
}

void errorAppear(){
    if(error_callback != nullptr){
        if(error_count == 0){
            error_callback();
        }
    }

    error_count++;
}

} //namespace
