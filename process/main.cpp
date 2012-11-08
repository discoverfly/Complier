

#include <cstdlib>
#include "LR1.h"
#include "generator.h"
#include<cstdio>
#include <iostream>
using namespace std;

int main(int argc, char** argv) {
    //LR1 lr1;
    //lr1.test();
    Generator* gen = new Generator();
    gen->init_lr1();
    gen->read();
    if (gen->reduce()) puts("AC");
    else puts("Error");
    gen->copy();
    return 0;
}

