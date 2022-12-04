//
// Created by asr96 on 12/1/2022.
//

#include "timer.h"
#include <chrono>
#include <iostream>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

 long long timer:: get_time() {

    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}