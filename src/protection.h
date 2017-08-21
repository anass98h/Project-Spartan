#pragma once 
#include <iostream>
#include <string.h>
#include <memory>
#include <stdio.h>
#include <list>
#include <time.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <sys/time.h>
#include "Utils/xorstring.h"
#include "project-spartan.h"
namespace protection {
    bool pwmatch(char* Pass);
    void timerstart();
    int UniqueId();
    void verify(int id);
    bool isVerified();
}
