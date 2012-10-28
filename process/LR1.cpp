/* 
 * File:   LR1.cpp
 * Author: discover
 * 
 * Created on 2012年10月24日, 下午2:02
 */

#include "LR1.h"

#include <iostream>
#include <stack>
#include <cstdio>
#include <sstream>

LR1::LR1() {
}

LR1::LR1(const LR1& orig) {
}

LR1::~LR1() {
}

bool LR1::nullable(string _v) {
    return nulls.find(_v) != nulls.end();
}

void LR1::get_first() {
    nulls.clear();
    first.clear();

    for (set<string>::iterator it = ts.begin(); it != ts.end(); ++it) {
        first[*it].insert(*it);
    }

    bool changed;
    do {
        changed = false;
        for (vector<pair<string, vector<string> > >::iterator it = ps.begin(); it != ps.end(); ++it) {
            string x = it->first;
            vector<string> & vec = it->second;
            bool can_null = true;
            for (vector<string>::iterator py = vec.begin(); py != vec.end(); ++py) {
                set<string> &ys = first[*py];
                set<string> &xs = first[x];
                for (set<string>::iterator fy = ys.begin(); fy != ys.end(); ++fy) {
                    if (xs.find(*fy) == xs.end()) {
                        changed = true;
                        xs.insert(*fy);
                    }
                }

                if (!nullable(*py)) {
                    can_null = false;
                    break;
                }
            }

            if (can_null && !nullable(x)) {
                changed = true;
                nulls.insert(x);
            }
        }
    } while (changed);
}

vector<string> LR1::firsts(vector<string>& vec) {
    vector<string> res;
    bool can_null = true;
    for (int i = 0; i < vec.size(); ++i) {
        for (set<string>::iterator it = first[vec[i]].begin(); it != first[vec[i]].end(); ++it) {
            res.push_back(*it);
        }

        if (!nullable(vec[i])) {
            can_null = false;
            break;
        }
    }
    return res;
}

set<LR1_State> LR1::closure(set<LR1_State> & I) {

    bool changed = false;
    do {
        changed = false;
        for (set<LR1_State>::iterator it = I.begin(); it != I.end(); ++it) {
            LR1_State state = *it;
            if (state.pos < state.y.size()) {
                string x = state.y[state.pos];
                vector<string> vec;
                for (int i = state.pos + 1; i < state.y.size(); ++i) {
                    vec.push_back(state.y[i]);
                }
                vec.push_back(state.flag);
                vector<string> flags = firsts(vec);
                for (vector<pair<string, vector<string> > >::iterator pp = ps.begin(); pp != ps.end(); ++pp) {
                    if (pp->first == x) {
                        for (int i = 0; i < flags.size(); ++i) {
                            LR1_State tmp(x, pp->second, 0, flags[i]);
                            if (I.find(tmp) == I.end()) {
                                changed = true;
                                I.insert(tmp);
                            }
                        }
                    }
                }
            }
        }
    } while (changed);

    return I;
}

set<LR1_State> LR1::go(const set<LR1_State> & I, string flag) {
    set<LR1_State> res;
    for (set<LR1_State>::iterator it = I.begin(); it != I.end(); ++it) {
        LR1_State st = *it;
        if (st.pos < st.y.size() && st.y[st.pos] == flag) {
            res.insert(LR1_State(st.x, st.y, st.pos + 1, st.flag));
        }
    }
    return closure(res);
}

void LR1::set_v_t_dic() {
    v_t_dic.clear();
    cols = 0;
    for (set<string>::iterator i = ts.begin(); i != ts.end(); ++i) {
        v_t_dic[*i] = ++cols;
    }
    for (set<string>::iterator i = vs.begin(); i != vs.end(); ++i) {
        v_t_dic[*i] = ++cols;
    }
    v_t_dic["$"] = ++cols;
}

void LR1::get_LR1() {
    nulls.insert("$");
    first["$"].insert("$");
    vector<string> vec;
    vec.push_back("S");
    vec.push_back("$");
    set<LR1_State> start;
    start.insert(LR1_State("S0", vec, 0, "?"));

    set<LR1_State> st = closure(start);
    /*
     for (set<LR1_State>::iterator it = st.begin(); it != st.end(); ++it) {
         cout << it->x << "->";
         for (int i = 0; i < it->y.size(); ++i) {
             if (it->pos == i) cout << ".";
             cout << it->y[i];
         }
         if (it->pos == it->y.size()) cout << ".";
         cout << "  " << it->flag << endl;
     }*/

    set_v_t_dic();
    for (int i = 0; i < LR1_const::MAXN; ++i) {
        for (int j = 0; j < LR1_const::MAXN; ++j) {
            mat[i][j] = make_pair(" ", -1);
        }
    }

    rows = 0;
    LR1_dic[st] = ++rows;

    bool changed = false;
    do {
        changed = false;
        for (map<set<LR1_State>, int>::iterator it = LR1_dic.begin(); it != LR1_dic.end(); ++it) {

            for (set<LR1_State>::iterator i = it->first.begin(); i != it->first.end(); ++i) {
                if (i->pos < i->y.size() && i->y[i->pos] != "$") {
                    set<LR1_State> new_state = go(it->first, i->y[i->pos]);
                    if (LR1_dic.find(new_state) == LR1_dic.end()) {
                        changed = true;
                        LR1_dic[new_state] = ++rows;
                    }

                    if (ts.find(i->y[i->pos]) != ts.end()) {
                        mat[LR1_dic[it->first]][v_t_dic[i->y[i->pos]]] = make_pair("s", LR1_dic[new_state]);
                    } else if (vs.find(i->y[i->pos]) != vs.end()) {
                        mat[LR1_dic[it->first]][v_t_dic[i->y[i->pos]]] = make_pair("g", LR1_dic[new_state]);
                    }
                }
            }
        }
    } while (changed);


    for (map<set<LR1_State>, int>::iterator i = LR1_dic.begin(); i != LR1_dic.end(); ++i) {
        const set<LR1_State>& lr_set = i->first;
        for (set<LR1_State>::iterator it = lr_set.begin(); it != lr_set.end(); ++it) {
            int r = LR1_dic[i->first];
            if (it->pos == it->y.size()) {
                int id = 0;
                for (id = 0; id < ps.size(); ++id) {
                    if (ps[id].first == it->x && ps[id].second == it->y) {
                        break;
                    }
                }


                mat[r][v_t_dic[it->flag]] = make_pair("r", id);
            }

            if (it ->x == "S0" && it->pos == 1 && it->y.size() == 2 && it->y[0] == "S" && it->y[1] == "$") {
                mat[r][v_t_dic["$"]] = make_pair("AC", 0);
            }
        }
    }

    for (map<set<LR1_State>, int>::iterator i = LR1_dic.begin(); i != LR1_dic.end(); ++i) {
        cout << endl;
        const set<LR1_State>& lr_set = i->first;
        for (set<LR1_State>::iterator it = lr_set.begin(); it != lr_set.end(); ++it) {
            cout << it->x << "->";
            for (int i = 0; i < it->y.size(); ++i) {
                if (it->pos == i) cout << ".";
                cout << it->y[i] << " ";
            }
            if (it->pos == it->y.size()) cout << ".";
            cout << "  " << it->flag << endl;
        }
        cout << endl;
    }
    /*
    for (int i = 1; i <= rows; ++i) {
        for (int j = 1; j <= cols; ++j) {
            printf("  %s%02d  ", mat[i][j].first.c_str(), mat[i][j].second);
        }
        puts("");
    }
     */
}

void LR1::print_first() {
    for (map<string, set<string> >::iterator px = first.begin(); px != first.end(); ++px) {
        cout << px->first << " : " << (nullable(px->first) ? "Yes" : "No") << endl << "    ";
        for (set<string>::iterator pf = px->second.begin(); pf != px->second.end(); ++pf) {
            cout << *pf << " ";
        }
        cout << endl;
    }
}

void LR1::set_ts(set<string>& _ts) {
    ts = _ts;
}

void LR1::set_vs(set<string>& _vs) {
    vs = _vs;
}

void LR1::set_ps(vector<pair<string, vector<string> > >& _ps) {
    ps = _ps;
}

void LR1::add_p(string x) {
    string name;
    istringstream in1(x);
    in1 >> name;
    vector<string> tmp;
    string str;
    in1 >> str;
    //if(str != "->") cout << "!!!!!!!!!!!!!!!!!!!!!!" << endl;
    while (in1 >> str) {
        tmp.push_back(str);
    }
    ps.push_back(make_pair(name, tmp));
}

void LR1::add_t(string x) {
    string name;
    istringstream in1(x);
    string str;
    while (in1 >> str) {
        ts.insert(str);
    }
}

void LR1::add_v(string x) {
    string name;
    istringstream in1(x);
    string str;
    while (in1 >> str) {
        vs.insert(str);
    }
}

void LR1::test() {
    /*
    set<string> _ts;
    _ts.insert("x");
    _ts.insert("*");
    _ts.insert("=");

    set<string> _vs;
    _vs.insert("S");
    _vs.insert("E");
    _vs.insert("V");
    _vs.insert("S0");

    vector<pair<string, vector<string> > > _ps;
    vector<string> tmp;

    tmp.clear();
    tmp.push_back("V");
    tmp.push_back("=");
    tmp.push_back("E");
    _ps.push_back(make_pair("S", tmp));

    tmp.clear();
    tmp.push_back("E");
    _ps.push_back(make_pair("S", tmp));

    tmp.clear();
    tmp.push_back("V");
    _ps.push_back(make_pair("E", tmp));

    tmp.clear();
    tmp.push_back("x");
    _ps.push_back(make_pair("V", tmp));


    tmp.clear();
    tmp.push_back("*");
    tmp.push_back("E");
    _ps.push_back(make_pair("V", tmp));
     this->set_ps(_ps);

    this->add_p("S1", "S $");
    this->add_p("S", "V = E");
    this->add_p("S", "E");
    this->add_p("E", "V");
    this->add_p("V", "x");
    this->add_p("V", "* E");

    this->set_ts(_ts);

    this->set_vs(_vs);
     */
    
    this->add_v("S0 S  X D ASSIGN FLOOP JUDGE FUNC TYPE EQ EQS VALUE VLIST EXP EXP1 F IDS LOGIC JUDGE ");
    this->add_t("int float , # [ ] ( ) [ ] { } main DIGIT REAL STRING for if else + - * / = > < == != >= <= ++ ID return");
    freopen("syntax", "r", stdin);
    string str;
    while (getline (cin, str)){
        //cout << str << endl;
        this->add_p(str);
    }

      /*
    this->add_t("* i =");
    this->add_v("S0 S L R");
    this->add_p("S0 -> S $");
    this->add_p("S -> L = R");
    this->add_p("S -> R");
    this->add_p("L -> * R");
    this->add_p("L -> i");
    this->add_p("R -> L");
    */
    this->get_first();
    //this->print_first();

    this->get_LR1();
    cout << rows << endl;
}