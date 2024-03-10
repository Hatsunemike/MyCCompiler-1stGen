/**
 * main source code of 1st C Compiler.
 */

#include <iostream>
#include <string>
#include <fstream>
#include <stack>
using namespace std;

bool isnum(char ch) {
    return ch>='0' && ch<='9';
}

bool isIden(const string& word) {
    return (word[0]>='a' && word[1]<='z') || (word[0]>='A' && word[0]<='Z');
}

bool isConst(const string& word) {
    int len = word.length();
    if(!len)return false;
    if(len == 1) {
        if(!isnum(word[0]))return false;
        return true;
    }
    if(!isnum(word[0]) && word[0] != '-')return false;
    for(int i=1;i<len;++i) {
        if(!isnum(word[i]))return false;
    }
    return true;
}

char operators[6] = {'+','-','*','/','(',')'};
const int oper_size = 6;

bool isOper(const char& ch) {
    for(int i=0;i<oper_size;++i){
        if(ch == operators[i]) {
            return true;
        }
    }
    return false;
}

bool isop(const char& ch) {
    return ch=='+'||ch=='-'||ch=='*'||ch=='/';
}

int usemem; // 使用空间
char nametable[30]; // 名字表（因为都是1位，所以没用字符数组）
int rk[255]; // 相当于nametable^{-1}
int var_cnt = 0;

void GenInt(ifstream&);
void GenReturn(ifstream&);
void GenExpr(ifstream&);

void GenInt(ifstream& rd_stream) {
    string name;
    rd_stream >> name;
    char ch_name = name[0];
    usemem += 4;
    nametable[++var_cnt] = ch_name;
    rk[ch_name] = var_cnt;
    printf("mov DWORD PTR [ebp-%d], 0\n",usemem);
    rd_stream >> name; // 把分号读掉
}

void GenReturn(ifstream &rd_stream)
{
    GenExpr(rd_stream);
    printf("ret\n");
}

void GenCalcOnce(char op){
    printf("pop ebx\npop eax\n");
    if(op == '+') {
        printf("add eax, ebx\n");
    }
    else if(op == '-') {
        printf("sub eax,ebx\n");
    }
    else if(op == '*') {
        printf("xor ecx,ecx\nimul eax,ebx\n");
    }
    else if(op == '/') {
        printf("cdq\nidiv ebx\n");
    }
    printf("push eax\n");
}

void GenExpr(ifstream &rd_stream)
{
    string word;
    stack<char> op;
    op.push('(');
    bool const_flag = false;
    char op1,op2;
    bool end_flag = false;
    while( !end_flag ){
        rd_stream >> word;
        if(word == ";") {
            op.push(')');
            end_flag = true;
        }
        else if((const_flag = isConst(word)) || isIden(word)) {
            printf("mov eax,%s\npush eax\n",word.c_str());
            continue;
        }
        else if(isOper(word[0])) {
            op.push(word[0]);
        }

        while(!op.empty()) {
            op2 = op.top();op.pop();
            op1 = op.top();
            if(op2 == ')') {
                if(op1 == '(') {
                    op.pop();
                    continue;
                }
                GenCalcOnce(op1);
                op.pop();
            }
            else if(op2 == '+' || op2 == '-') {
                if(op1 == '(') {
                    op.push(op2);
                    break;
                }
                GenCalcOnce(op1);
                op.pop();
            }
            else if(op2 == '*' || op2 == '/') {
                if(op1 == '(' || op1 == '+' || op1 == '-') {
                    op.push(op2);
                    break;
                }
                GenCalcOnce(op1);
                op.pop();
            }
            op.push(op2);
        }
    }
    printf("pop eax\n");
}

int main(int argc,char* argv[]) {
    string rd_file = (argc > 1)?argv[1]:"stdin";
    cout << "rd_file: " << rd_file << endl;
    ifstream rd_stream;
    rd_stream.open(rd_file);
    string word;
    while( true ) {
        word.clear();
        rd_stream >> word;
        if(word.empty())break;
        if(word == "int") {
            GenInt(rd_stream);
        }
        else if(word == "return") {
            GenReturn(rd_stream);
        }
        else if(isIden(word)) { // 赋值语句
            char ch_name = word[0];
            rd_stream >> word; // 把=读了
            GenExpr(rd_stream);
            printf("mov DWORD PTR [ebp-%d], eax\n", rk[ch_name] * 4);
        }
    }
    return 0;
}