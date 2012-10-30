/* 
 * File:   Generator.cpp
 * Author: discover
 * 
 * Created on 2012年10月30日, 上午11:18
 */

#include "Generator.h"
#include <iostream>
#include <cstdio>
#include <sstream>
#include <string>

void Generator::read() {
    freopen("words", "r", stdin);
    char buf[1000];
    string type, x;

    while (gets(buf) != NULL) {
        istringstream in(buf);
        in >> type >> x;
        in >> x;
        src.push_back(Word(type, x));
        cout << type << " " << x << endl;
    }
    fclose(stdin);
}

void Generator::init_lr1() {
    lr1.read();
    lr1.get_first();
    lr1.get_LR1();
    //cout <<"init lr1 over!" << endl;
    //lr1.print_LR1_state();
}

bool Generator::reduce() {
    top = -1;
    stat[++top] = 1;
    pair<string, int> nxt;
    int cur;
    string add;
    int id;
    pair<string, vector<string> > prod;
    bool dec;
    for (vector<Word>::iterator it = src.begin(); it != src.end(); ++it) {
        add = it->type;
        cur = stat[top];
        dec = false;

        while (true) {
            nxt = lr1.next_action(cur, add);
            if (nxt.second == -1) {
                break;
            }
            // cout << add << " :  ";
            if (nxt.first == "r") {
                id = nxt.second;
                prod = lr1.get_p_by_id(id);
                do_reduce(prod);
                cur = stat[top];
                add = prod.first;
                if (dec == false) {
                    dec = true;
                    --it;
                }
            } else if (nxt.first == "AC") {
                cout << top << endl;
                return true;
            } else {
                stk[top] = add;
                stat[++top] = nxt.second;
                break;
            }
            // for (int i = 0; i < top; ++i) cout << stat[i] << " " << stk[i] << " ";
            // cout << stat[top];
            // cout << endl;
        }
    }
   
    return false;
}

void Generator::do_reduce(pair<string, vector<string> >& prod) {
    top = top - prod.second.size();
}

Generator::Generator() {
}

Generator::Generator(const Generator& orig) {
}

Generator::~Generator() {
}


