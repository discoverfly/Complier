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
#include <cstring>

void Generator::read() {
    freopen("words", "r", stdin);
    char buf[1000];
    string type, x;

    while (gets(buf) != NULL) {
        istringstream in(buf);
        in >> type >> x;
        char bef;
        in >> bef;
        getline(in, x);

        src.push_back(Word(type, bef + x));
        // cout << type << " " << x << endl;
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
    tmp_cnt = 0;
    label_cnt = 0;
    labels.clear();
    for_beg.clear();
    memset(is_busy, 0, sizeof (is_busy));
    top = -1;
    stat[++top] = 1;
    pair<string, int> nxt;
    int cur;
    string add;
    int id;
    string for_begin_label;
    pair<string, vector<string> > prod;
    vector<Word>::iterator it = src.begin();
    while (it != src.end()) {
        add = it->type;
        cur = stat[top];

        nxt = lr1.next_action(cur, add);
        if (nxt.second == -1) {
            cout << "Error in reduce" << endl;
            break;
        }
        // cout << add << " :  ";
        if (nxt.first == "r") {
            id = nxt.second;
            prod = lr1.get_p_by_id(id);
            stk[top] = do_reduce(prod, id + 1);
            cur = stat[top];
            stat[++top] = lr1.next_action(cur, prod.first).second;
        } else if (nxt.first == "AC") {
            cout << top << endl;
            section_text << "pushl\t$0\n";
            section_text << "call _exit\n";
            section_text.flush();
            return true;
        } else {
            stk[top] = *it;
            stat[++top] = nxt.second;
            ++it;
        }
        //        for (int i = 0; i < top; ++i) cout << " " <<stat[i] <<" " << stk[i].type << " ";
        //        cout << stat[top];
        //        cout << endl;

        if (top >= 4 && stk[top - 4].x == "for" && stk[top - 1].x == ";") {
            //cerr <<stk[top-4].x <<" "<<stk[top-3].x <<" "<<stk[top-2].x <<" "<<stk[top-1].x <<endl;
            cerr << "has for" << endl;
            for_begin_label = get_new_label();
            section_text << for_begin_label << ":\n";
            for_beg.push_back(for_begin_label);
            section_text.flush();
        }
        if (top >= 1 && stk[top - 1].x == "if") {
            end_flag = get_new_label();
        }
        if (top >= 1 && stk[top - 1].x == "else") {

            section_text << "jmp\t" << end_flag << "\n";
            else_flag = get_new_label();
            section_text << else_flag << ":\n";
        }
    }
    section_data.close();
    section_text.close();
    return false;
}

int Generator::get_reg() {
    for (int i = 0; i < 4; ++i) {
        if (!is_busy[i]) return i;
    }
    return -1;
}

string Generator::get_new_tmp() {
    string res = "tmp_value_";
    int t = tmp_cnt;
    do {
        res += ((t % 26) + 'a');
        t /= 26;
    } while (t);
    ++tmp_cnt;
    return res;
}

string Generator::get_new_label() {
    string res = "LABEL_";
    int t = label_cnt;
    do {
        res += ((t % 26) + 'A');
        t /= 26;
    } while (t);
    ++label_cnt;
    return res;
}

bool Generator::is_float(Word &a) {
    if (a.xtype == 1) return false;
    if (a.xtype == 2) return true;
    if (a.xtype == 3 || a.xtype == 4) {
        return decs[a.x] == "float";
    }
    return false;
}

Word Generator::deal_op(string op, Word& a, Word& b) {
    Word res;
    bool has_float = (is_float(a) || is_float(b));
    if (has_float == false) {
        string ans = get_new_tmp();
        section_data << ans << ":\n\t." << "int " << 0 << "\n";
        section_text << "pushl\t%eax\n";
        if (a.xtype == 1 || a.xtype == 2) {
            section_text << "movl\t$" << a.x << ",\t%eax\n";
        } else {
            section_text << "movl\t" << a.x << ",\t%eax\n";
        }
        if (op == "+") {
            if (b.xtype == 1 || b.xtype == 2) {
                section_text << "addl\t$" << b.x << ",\t%eax\n";
            } else {
                section_text << "addl\t" << b.x << ",\t%eax\n";
            }
        } else if (op == "-") {
            if (b.xtype == 1 || b.xtype == 2) {
                section_text << "subl\t$" << b.x << ",\t%eax\n";
            } else {
                section_text << "subl\t" << b.x << ",\t%eax\n";
            }
        } else if (op == "*") {
            if (b.xtype == 1 || b.xtype == 2) {
                section_text << "imull\t$" << b.x << ",\t%eax\n";
            } else {
                section_text << "imull\t" << b.x << ",\t%eax\n";
            }
        } else if (op == "/") {
            if (b.xtype == 1 || b.xtype == 2) {
                section_text << "divl\t$" << b.x << ",\t%eax\n";
            } else {
                section_text << "divl\t" << b.x << ",\t%eax\n";
            }
        }
        section_text << "movl\t%eax, " << ans << "\n";
        section_text << "popl\t%eax\n";
        decs[ans] = "int";
        return Word("", ans, 3);
    } else {
        string left, right, ans;
        if (a.xtype == 1 || a.xtype == 2) {
            left = get_new_tmp();
            section_data << left << ":\n\t." << "float " << a.x << "\n";
        } else left = a.x;

        if (b.xtype == 1 || b.xtype == 2) {
            right = get_new_tmp();
            section_data << right << ":\n\t." << "float " << b.x << "\n";
        } else right = b.x;

        ans = get_new_tmp();
        section_data << ans << ":\n\t." << "float " << 0 << "\n";

        if (op == "+") {
            section_text << "flds\t" << left << "\n";
            section_text << "fadd\t" << right << "\n";
            section_text << "fstps\t" << ans << "\n";
        } else if (op == "-") {
            section_text << "flds\t" << left << "\n";
            section_text << "fsub\t" << right << "\n";
            section_text << "fstps\t" << ans << "\n";
        } else if (op == "*") {
            section_text << "flds\t" << left << "\n";
            section_text << "fmul\t" << right << "\n";
            section_text << "fstps\t" << ans << "\n";
        } else if (op == "/") {
            section_text << "flds\t" << left << "\n";
            section_text << "fdiv\t" << right << "\n";
            section_text << "fstps\t" << ans << "\n";
        } else cerr << "unkown operator\n";
        decs[ans] = "float";
        return Word("", ans, 3);
    }
}

Word Generator::deal_cmp(string op, Word& a, Word& b) {
    if (!is_float(a) && !is_float(b)) {
        section_text << "cmp\t"
                << (a.xtype == 1 ? "$" : "") << a.x << ",\t"
                << (b.xtype == 1 ? "$" : "") << b.x << "\n";
        else_flag = get_new_label();
        string jmp;
        if (op == "<") jmp = "jge";
        else if (op == ">") jmp = "jle";
        else if (op == "<=") jmp = "jg";
        else if (op == ">=") jmp = "jl";
        else if (op == "==") jmp = "jne";
        else if (op == "!=") jmp = "je";
        section_text << jmp << "\t" << else_flag << "\n";
    } else {

    }
    return Word("", "", -1);
}

Word Generator::do_reduce(pair<string, vector<string> >& prod, int id) {
    int t;
    string reg;
    string res = "";
    int xtype = -1;
    ostringstream out;
    string tmp_name;
    Word tw;
    string flag;

    switch (id) {
        case 1:break;
        case 2:break;
        case 3: //X -> D #
            break;
        case 4: ///X -> D # X
            break;
        case 5: //X -> FUNC #
            break;
        case 6: //X -> FUNC # X
            break;
        case 7: //X -> ASSIGN #
            break;
        case 8: // X -> ASSIGN # X
            break;
        case 9: //X -> FLOOP
            break;
        case 10: //X -> FLOOP X
            break;
        case 11: //X -> JUDGE
            break;
        case 12: // X -> JUDGE X
            break;
        case 13: //D ->  TYPE IDS
            break;
        case 14: // IDS -> ID
            section_data << stk[top - 1].x << ":\n\t." << cur_types << " 0\n";
            decs[stk[top - 1].x] = cur_types;
            break;
        case 15: // IDS -> ID , IDS
            section_data << stk[top - 3].x << ":\n\t." << cur_types << " 0\n";
            decs[stk[top - 3].x] = cur_types;
            break;
        case 16: // IDS -> ID EQ
            break;
        case 17:
            break;
        case 18:
            break;
        case 19:break;
        case 20: //IDS -> ID [ DIGIT ] EQS
            section_data << stk[top - 5].x << ":\n\t." << cur_types << " " << stk[top - 1].x << "\n";
            decs[stk[top - 5].x] = cur_types;
            break;
        case 21:
            break;
        case 22: // EQ -> = VALUE
            break;
        case 23: // EQS -> = { VLSIT }
            for (int i = vlist.size() - 1; i >= 0; --i) {
                out << vlist[i].x;
                if (i) out << ", ";
            }
            res = out.str();
            vlist.clear();
            break;
        case 24: //VLIST ->  VALUE
            vlist.push_back(Word("", stk[top - 1].x, stk[top - 1].xtype));
            break;
        case 25: //VLIST ->  VALUE , VLIST
            vlist.push_back(Word("", stk[top - 3].x, stk[top - 3].xtype));
            break;
        case 26: //VALUE -> DIGIT
            res = stk[top - 1].x;
            xtype = 1;
            break;
        case 27: //VALUE -> REAL
            res = stk[top - 1].x;
            xtype = 2;
            break;
        case 28: //VALUE -> STRING
            tmp_name = get_new_tmp();
            section_data << tmp_name << ":\n\t .string\t" << stk[top - 1].x << "\n";
            res = tmp_name;
            xtype = 0;
            break;
        case 29: //VALUE -> ID
            res = stk[top - 1].x;
            xtype = 3;
            break;
        case 30:
            break;
        case 31: //VALUE -> ID [ ID ]
            tmp_name = get_new_tmp();
            section_data << tmp_name << ":\n\t." << decs[stk[top - 2].x] << "\t0\n";
            section_text << "pushl\t%eax\n";
            section_text << "pushl\t%ebx\n";
            section_text << "movl\t" << stk[top - 2].x << ",\t%eax\n";
            section_text << "movl\t" << stk[top - 4].x + "(,\t%eax" + ", 4),\t%ebx" << "\n";
            section_text << "movl\t%ebx,\t" << tmp_name << "\n";
            section_text << "popl\t%ebx\n";
            section_text << "popl\t%eax\n";
            res = tmp_name;
            xtype = 4;
            break;
        case 32: // TYPE -> int
            cur_types = "int";
            break;
        case 33: // TYPE -> float
            cur_types = "float";
            break;
        case 34: //FUNC -> ID ( VLIST )
            if (stk[top - 4].x == "printf") flag = "";
            else flag = "$";
            for (int i = 0; i < vlist.size(); ++i) {
                if (vlist[i].xtype == 0) {
                    section_text << "pushl\t$" << vlist[i].x << "\n";
                } else if (vlist[i].xtype == 1) {
                    section_text << "pushl\t" << flag << vlist[i].x << "\n";
                } else if (vlist[i].xtype == 3) {
                    if (decs[vlist[i].x] == "int") {
                        section_text << "pushl\t" << flag << vlist[i].x << "\n";
                    } else if (decs[vlist[i].x] == "float") {
                        section_text << "subl\t$8,\t%esp\n";
                        section_text << "flds\t" << vlist[i].x << "\n";
                        section_text << "fstpl\t(%esp)\n";
                    }
                }
            }
            section_text << "call\t_" << stk[top - 4].x << "\n";
            vlist.clear();
            break;
        case 35: //ASSIGN -> ID = EXP
            switch (stk[top - 1].xtype) {
                case 1:
                    if (decs[stk[top - 3].x] == "int") {
                        section_text << "movl\t$" << stk[top - 1].x << ",\t" << stk[top - 3].x << "\n";
                    } else if (decs[stk[top - 3].x] == "float") {
                        tmp_name = get_new_tmp();
                        section_data << tmp_name << ":\n\t." << "float " << stk[top - 1].x << "\n";
                        section_text << "flds\t" << tmp_name << "\n";
                        section_text << "fstps\t" << stk[top - 3].x << "\n";
                    } else {
                        cerr << "ERROR in Assignment from Int to others" << endl;
                    }
                    break;
                case 2:
                    tmp_name = get_new_tmp();
                    section_data << tmp_name << ":\n\t." << "float " << stk[top - 1].x << "\n";
                    section_text << "flds\t" << tmp_name << "\n";
                    section_text << "fstps\t" << stk[top - 3].x << "\n";
                    break;
                case 3:
                    if (decs[stk[top - 3].x] == "int") {
                        section_text << "pushl\t%eax\n";
                        section_text << "movl\t" << stk[top - 1].x << ",\t%eax" << "\n";
                        section_text << "movl\t%eax,\t" << stk[top - 3].x << "\n";
                        section_text << "popl\t%eax\n";
                    } else if (decs[stk[top - 3].x] == "float") {

                        section_text << "flds\t" << stk[top - 1].x << "\n";
                        section_text << "fstps\t" << stk[top - 3].x << "\n";
                    } else {
                        cerr << "ERROR in Assignment from Int to others" << endl;
                    }
                    break;
                case 4:
                    break;
                case 5:
                    break;
                case 6:
                    break;
                default:cout << "Error in assinment" << endl;
            }
            memset(is_busy, 0, sizeof (is_busy));
            break;
        case 36: //EXP -> EXP + EXP1
            tw = deal_op("+", stk[top - 3], stk[top - 1]);
            res = tw.x;
            xtype = tw.xtype;
            break;
        case 37: //EXP -> EXP - EXP1
            tw = deal_op("-", stk[top - 3], stk[top - 1]);
            res = tw.x;
            xtype = tw.xtype;
            break;
        case 38: //EXP -> EXP1
            res = stk[top - 1].x;
            xtype = stk[top - 1].xtype;
            break;
        case 39: //EXP1 -> EXP1 * F
            tw = deal_op("*", stk[top - 3], stk[top - 1]);
            res = tw.x;
            xtype = tw.xtype;
            break;
        case 40: //EXP1 -> EXP1 / F
            tw = deal_op("/", stk[top - 3], stk[top - 1]);
            res = tw.x;
            xtype = tw.xtype;
            break;
        case 41: //EXP1 -> F
            res = stk[top - 1].x;
            xtype = stk[top - 1].xtype;
            break;
        case 42: //F -> ( EXP )
            res = stk[top - 2].x;
            xtype = stk[top - 2].xtype;
            break;
        case 43: //F -> VALUE
            res = stk[top - 1].x;
            xtype = stk[top - 1].xtype;
            break;
        case 44: //FLOOP -> for ( ASSIGN # LOGIC # ID ++ ) { X }
            section_text << "incl\t" << stk[top - 6].x << "\n";
            section_text << "jmp\t" << for_beg[for_beg.size() - 1] << "\n";
            section_text << else_flag << ":\n";
            for_beg.pop_back();
            break;
        case 45: // LOGIC -> VALUE > VALUE
            deal_cmp(">", stk[top - 1], stk[top - 3]);
            break;
        case 46: //LOGIC -> VALUE >= VALUE
            deal_cmp(">=", stk[top - 1], stk[top - 3]);
            break;
        case 47: // LOGIC -> VALUE == VALUE

            break;
        case 48: //LOGIC -> VALUE <= VALUE
            deal_cmp("<=", stk[top - 1], stk[top - 3]);
            break;
        case 49: //LOGIC -> VALUE < VALUE
            deal_cmp("<", stk[top - 1], stk[top - 3]);
            break;
        case 50: //LOGIC -> VALUE != VALUE
            deal_cmp("!=", stk[top - 1], stk[top - 3]);
            break;
        case 51: //LOGIC -> VALUE != VALUE
            break;
        case 52: // JUDGE -> if ( LOGIC ) { X } else { X }
            section_text << end_flag << ":\n";
            break;
    }
    section_data.flush();
    section_text.flush();
    top = top - prod.second.size();
    return Word(prod.first, res, xtype);
}

Generator::Generator() {
    cout << "hello" << endl;
    section_data.open("section_data");
    section_text.open("section_text");
    if (!section_data.is_open()) cerr << "open section_data error" << endl;
    if (!section_text.is_open()) cerr << "open section_text error" << endl;
    section_data << ".section .data\n";
    section_text << ".section .text\n";
    section_text << ".def    _printf;    .scl	2;	.type	32;	.endef\n";
    section_text << ".def    _scanf;	.scl	2;	.type	32;	.endef\n";
    section_text << ".def	_exit;	.scl	2;	.type	32;	.endef\n";
    section_text << ".globl _main\n" << "_main:\n";
    section_text << "nop\nfinit\n";
    section_data.flush();
    section_text.flush();
}

void Generator::copy(){
    ifstream data;
    data.open("section_data");
    ofstream code;
    code.open("gencode");
    string tmps;
    
    while (getline(data,tmps)){
        code <<tmps<<"\n";
    }
    
    ifstream text;
    text.open("section_text");
    while(getline(text, tmps)){
        code<<tmps<<"\n";
    }
    data.close();
    text.close();
    code.close();
    cerr <<"Copy code over! Successed -:)\n";
}
Generator::Generator(const Generator& orig) {
}

Generator::~Generator() {
}
