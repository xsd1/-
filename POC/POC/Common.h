#pragma once
#include <bits/stdc++.h>
using namespace std;
const int num_of_keyword = 22;
const int num_of_MO = 11;
const int num_of_BO = 12;
const int num_of_D = 9;
const vector<string> keyword = { "break","case","char","continue","do","default","double",
"else","float","for","if","int","include","long","main","return","switch","typedef","void","unsigned","while","iostream" , "vector", "set", "string" };
const vector<char> B_Operator = { '+','-','*','/','!','%','~','&','|','^','=' };// 单字符运算符
const vector<string> M_Operator = { "++","--","&&","||","<=","!=","==",">=","+=","-=","*=","/=" }; // 双字符运算符
const vector<char> Delimiter = { ',','(',')','{','}',';','<','>','#' }; //界符
const string typeOfToken[10] = { "出错", "运算符", "常量", "标识符", "关键词", "界符"};
map<string, char>abbrOfKwd = { {"break", 'b'}, {"case", 'c'}, {"char", 'a'}, {"continue", 'o'},
{"do", 'd'}, {"default", 'n'}, {"double", 'z'}, {"else", 'e'}, {"float", 'y'}, {"for", 'f'}, {"if", 'i'},
{"int", 'h'}, {"include", 'p'}, {"long", 'l'}, {"main", 'm'}, {"return", 'r'}, {"switch", 's'}, {"typedef", 't'},
{"void", 'v'}, {"unsigned", 'u'}, {"while", 'w'}, {"iostream", 'k'}, {"string", 'k'}, {"set", 'k'}, {"vector", 'k'}
};// 缩写映射
