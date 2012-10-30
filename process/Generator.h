/* 
 * File:   Generator.h
 * Author: discover
 *
 * Created on 2012年10月30日, 上午11:18
 */

#ifndef GENERATOR_H
#define	GENERATOR_H
#include "LR1.h"

using namespace std;

class Word {
public:
    string type;
    string x;

    Word() {
    }

    Word(string t, string tx)
    : type(t), x(tx) {
    }
};

class Generator {
public:
    void init_lr1();
    void read();
    bool reduce();
    void do_reduce(pair<string, vector<string> >& prod);
    Generator();
    Generator(const Generator& orig);
    virtual ~Generator();
private:
    vector<Word> src;
    LR1 lr1;
    string stk[10000];
    int stat[10000];
    int top;
};

#endif	/* GENERATOR_H */

