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
        //  cout << type << " " << x << endl;
    }
    fclose(stdin);
}

void Generator::init_lr1() {
    lr1.read();
    cout << "lr1 read overed" << endl;
    lr1.get_first();
    cout << "get_frist overed" << endl;
    lr1.get_LR1();
    cout << "get_LR1 overed" << endl;
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
                do_reduce(prod, id + 1);
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
                stk[top] = *it;
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

void Generator::do_reduce(pair<string, vector<string> >& prod, int id) {
    switch (id) {
        case 1:break;
        case 2:break;
        case 3:break;
        case 4:break;
        case 5:break;
        case 6:break;
        case 7:break;
        case 8:break;
        case 9:break;
        case 10:break;
        case 11:break;
        case 12:break;
        case 13:break;
        case 14: // IDS -> ID
            section_data << stk[top - 1].x << ":\n\t" << "." << cur_types << " 0\n";
            decs.push_back(make_pair(cur_types, stk[top - 1].x));
            break;
        case 15: // IDS -> ID
            section_data << stk[top - 3].x << ":\n\t" << "." << cur_types << " 0\n";
            decs.push_back(make_pair(cur_types, stk[top - 3].x));
            break;
        case 16: // IDS -> ID EQ
            break;
        case 17:break;
        case 18:
            break;
        case 19:break;
        case 20:
            section_data << stk[top - 5].x << ":\n\t" << "." << cur_types <<" ";
            for (int i = 0; i < vlist.size(); ++i) {
                if (i)  section_data << ", ";
                section_data <<vlist[i];
            }
            section_data<<"\n";
            vlist.clear();
            break;
        case 21:break;
        case 22: // EQ -> = VALUE
            break;
        case 23:break;
        case 24:
            vlist.push_back(stk[top - 1].x);
            break;
        case 25:
            vlist.push_back(stk[top - 3].x);
            break;
        case 26:break;
        case 27:break;
        case 28:break;
        case 29:break;
        case 30:break;
        case 31:break;
        case 32: // TYPE -> int
            cur_types = "int";
            break;
        case 33: // TYPE -> float
            cur_types = "float";
            break;
        case 34:break;
        case 35:break;
        case 36:break;
        case 37:break;
        case 38:break;
        case 39:break;
        case 40:break;
        case 41:break;
        case 42:break;
        case 43:break;
        case 44:break;
        case 45:break;
        case 46:break;
        case 47:break;
        case 48:break;
        case 49:break;
        case 50:break;
        case 51:break;
        case 52:break;
    }
    section_data.flush();
    section_text.flush();
    top = top - prod.second.size();
}

Generator::Generator() {
    cout << "hello" << endl;
    section_data.open("section_data");
    section_text.open("section_text");
    if (!section_data.is_open()) cerr << "open section_data error" << endl;
    if (!section_text.is_open()) cerr << "open section_text error" << endl;
    section_data << ".section .data\n";
    section_text << ".section .text\n";
    section_text <<".globl _start\n" <<"_start:\n";
    section_data.flush();
    section_text.flush();
}

Generator::Generator(const Generator& orig) {
}

Generator::~Generator() {
    section_data.close();
    section_text.close();
}


