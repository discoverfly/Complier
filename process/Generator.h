/* 
 * File:   Generator.h
 * Author: discover
 *
 * Created on 2012年10月30日, 上午11:18
 */

#ifndef GENERATOR_H
#define	GENERATOR_H
#include "LR1.h"
#include <vector>
#include <fstream>
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

    bool operator<(const Word& a) const {
        if (x != a.x) return x < a.x;
        return type < a.type;
    }
};

class Generator {
public:
    void init_lr1();
    void read();
    bool reduce();
    string do_reduce(pair<string, vector<string> >& prod, int id);
    Generator();
    Generator(const Generator& orig);
    virtual ~Generator();
    int get_reg();
private:
    vector<Word> src;
    LR1 lr1;
    Word stk[1000];
    int stat[1000];
    int top;


    string cur_types;
    vector<pair<string, string> > decs;
    string exp;
    vector<string> vlist;
    vector<Word> exp_vec;
    bool is_busy[4];

    ofstream section_data;
    ofstream section_text;
};

#endif	/* GENERATOR_H */

