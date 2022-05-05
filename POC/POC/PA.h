#pragma once
#include <bits/stdc++.h>
using namespace std;
class PA
{
public:
	int g_Len; // 文法的长度
	char g_start;// 文法开始符号
	vector<char>g_state_set;// 非终结符集
	vector<char>g_final_set;// 终结符集

	vector<pair<char, string> >g_grammar;// 记录产生式的左部和右部
	vector<vector<char> >g_first_set;// 下标代表项目集编号 first集
	map<int, vector<int> >g_item_trans;// 项目集转换 暂时没用
	bitset<100>bs;// 作为标记 使用

	struct Item {// 项目集的一项
		char left;// 左端
		string right;// 右端
		int dot; // 小圆点的位置 等于right.size()视为句柄
		char s;  // 向前搜索符
		// bool isFinal;// 句柄

		// 判等函数
		bool operator==(const Item& it)const;
		// Item& operator=(const Item& it){
		//     left = it.left;
		//     dot = it.dot;
		//     right = it.right;
		//     s = it.s;
		//     return *this;
		// }
		// 打印
		void toString();
	};

	struct Project { // 项目集规范族的一项
		int num; // 项目集编号
		int sz_item;// items的长度
		vector<int> next;// 指向下一个
		vector<Item>items;// 每一项
		vector<char>ch_trans;// 转换的字符 与指向编号对应
		// 判等
		bool operator==(const Project& p)const;
		// 是否存在
		bool isExists(Item item);
		// 打印
		void toString();
	};
	Project project[200] = {};
	int sz_project = 0;// 项目集的个数


	struct Action {
		char ch;// 遇到的字符 vn/vt
		char act;// r/S/g 进行的动作
		int next_state;//  指向下一个状态
		//bool isAcc;// 是否到达Acc 其实根据next_state已经可以做出判断
	};
	vector<vector<Action> >g_action;
	map<pair<int, char>, Action>g_actions;

	// 判重
	int isDuplicated(Project p);
	bool isDuplicated(Item item, Item* items, int sz);

	// 打印
	void print(vector<char> v);
	void printSet(vector<pair<char, vector<char> > >v);

	// 准备函数
	void loadGrammar();
	void getSet(vector<char>&v, char nt, int index);
	void getFirstSet();
	void writeFirstSet();
	void getClosure(Project& pr, map<char, vector<string> >tmp);
	void constructItemSet();
	void writeItemSet();
	void constructAGT();

	// 准备 + 处理
	void LR1();
	void process(char T[200], char(*Res)[500], int& flag, char& error, int& Size);
};

