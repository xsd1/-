#pragma once
#include <bits/stdc++.h>
using namespace std;
class LA
{
public:
	vector<char>::iterator it;
	// 全局变量
	int g_Len; // 文法的长度
	char g_start;// 文法开始符号
	vector<char>g_state_set;// 非终结符集
	vector<char>g_final_set; // 终结符集
	map<pair<char, char>, vector<char>>g_NFA_set;   // 状态A 接收终结符 转换成状态B
	map<pair<int, char>, int>g_DFA_trans; // DFA之间的转换 T0 -> T1...
	vector<vector<char>>g_DFA_set;
	vector<bool>g_DFA_IsFinal;// 判断该状态是否是终态

	struct Token {
		int line;// 源程序的所在行号
		int type;// 类型编号 0、1、2……
		string token;// 具体字符串
		void toString();// 打印函数
	};
	vector<Token>Tokens;

	template<typename T1, typename T2>
	int isExist(T1 a, T2 b) {
		int i = 1;
		for (auto c : a) {
			if (c == b) {
				return i;
			}
			i++;
		}
		return 0;
	}

	// 判断函数
	bool isMO(char a[]);
	bool isBO(char a);
	bool isKW(char a[]);
	bool isAlpha(char ch);
	bool isDigit(char ch);
	bool isKeyWord(string s);
	bool isDelimiter(char ch);

	// 主程序
	void createNFA();
	void getClosure(vector<char>& T);
	void nfaToDfa();
	void printDFA(char(*T)[200], int Size, int& flag);
	bool dfaCheck(char a[]);
	void scanSourceFile();
	void showTokens(char(* T)[200], int Size, int& flag);

	// 开始
	void main();
};

