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
    int xtype; // -1: none  1: DIGIT, 2: REAL, 3:  ID, 4: array, 5 int reg, 6, reg 0.string

    Word() {
    }

    Word(string t, string tx, int xt = -1)
    : type(t), x(tx), xtype(xt) {
    }

    bool operator<(const Word& a) const {
        if (xtype != a.xtype) return xtype < a.xtype;
        if (x != a.x) return x < a.x;
        return type < a.type;
    }
};

class Generator {
public:
    void init_lr1();
    void read();
    bool reduce();
    Word do_reduce(pair<string, vector<string> >& prod, int id);
    Generator();
    Generator(const Generator& orig);
    virtual ~Generator();
    int get_reg();
    Word deal_op(string op, Word& a, Word& b);
    Word deal_cmp(string op, Word& a, Word& b);
    bool is_float(Word &a);
    void copy();
private:
    string get_new_tmp();
    string get_new_label();
    vector<string> labels;
    vector<string> for_beg;
    vector<Word> src;
    LR1 lr1;
    Word stk[1000];
    int stat[1000];
    int top;
    string else_flag, end_flag;
    string cur_types;
    map<string, string> decs;
    string exp;
    vector<Word> vlist;
    vector<Word> exp_vec;
    bool is_busy[4];
    int tmp_cnt;
    int label_cnt;
    ofstream section_data;
    ofstream section_text;
};

#endif	/* GENERATOR_H */

