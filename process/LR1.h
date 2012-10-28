/* 
 * File:   LR1.h
 * Author: discover
 *
 * Created on 2012年10月24日, 下午2:02
 */

#ifndef LR1_H
#define	LR1_H


#include <vector>
#include <string>
#include <map>
#include <set>

using namespace std;

class LR1_State {
public:
    string x;
    vector<string> y;
    int pos;
    string flag;

    LR1_State() {
    }

    LR1_State(string tx, vector<string> ty, int tpos, string tflag)
    : x(tx), y(ty), pos(tpos), flag(tflag) {
    }

    bool operator<(const LR1_State & a) const {
        if (x != a.x) return x < a.x;
        if (pos != a.pos) return pos < a.pos;
        if (flag != a.flag) return flag < a.flag;
        return y < a.y;
    }
    
};

namespace LR1_const {
    const int MAXN = 500;
}

class LR1 {
public:
    LR1();
    LR1(const LR1& orig);
    void test();
    virtual ~LR1();

    void set_ts(set<string>& _ts);
    void set_vs(set<string>& _vs);
    void set_ps(vector<pair<string, vector<string> > >& _ps);
    void get_first();
    void print_first();
    void get_LR1();
    void add_p(string x);
    void add_t(string x);
    void add_v(string x);
private:
    
    void set_v_t_dic();
    set<LR1_State> closure(set<LR1_State> & I);
    set<LR1_State> go(const set<LR1_State> & I, string flag);
    vector<string> firsts(vector<string>& vec);
    bool nullable(string _v);

    set<string> nulls;
    map<string, set<string> > first;

    int rows, cols;
    map<string, int> v_t_dic;
    map<set<LR1_State>, int> LR1_dic;
    pair<string,int> mat[LR1_const::MAXN][LR1_const::MAXN];

    set<string> ts;
    set<string> vs;
    vector<pair<string, vector<string > > > ps;
};

#endif	/* LR1_H */

