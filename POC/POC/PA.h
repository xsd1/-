#pragma once
#include <bits/stdc++.h>
using namespace std;
class PA
{
public:
	int g_Len; // �ķ��ĳ���
	char g_start;// �ķ���ʼ����
	vector<char>g_state_set;// ���ս����
	vector<char>g_final_set;// �ս����

	vector<pair<char, string> >g_grammar;// ��¼����ʽ���󲿺��Ҳ�
	vector<vector<char> >g_first_set;// �±������Ŀ����� first��
	map<int, vector<int> >g_item_trans;// ��Ŀ��ת�� ��ʱû��
	bitset<100>bs;// ��Ϊ��� ʹ��

	struct Item {// ��Ŀ����һ��
		char left;// ���
		string right;// �Ҷ�
		int dot; // СԲ���λ�� ����right.size()��Ϊ���
		char s;  // ��ǰ������
		// bool isFinal;// ���

		// �еȺ���
		bool operator==(const Item& it)const;
		// Item& operator=(const Item& it){
		//     left = it.left;
		//     dot = it.dot;
		//     right = it.right;
		//     s = it.s;
		//     return *this;
		// }
		// ��ӡ
		void toString();
	};

	struct Project { // ��Ŀ���淶���һ��
		int num; // ��Ŀ�����
		int sz_item;// items�ĳ���
		vector<int> next;// ָ����һ��
		vector<Item>items;// ÿһ��
		vector<char>ch_trans;// ת�����ַ� ��ָ���Ŷ�Ӧ
		// �е�
		bool operator==(const Project& p)const;
		// �Ƿ����
		bool isExists(Item item);
		// ��ӡ
		void toString();
	};
	Project project[200] = {};
	int sz_project = 0;// ��Ŀ���ĸ���


	struct Action {
		char ch;// �������ַ� vn/vt
		char act;// r/S/g ���еĶ���
		int next_state;//  ָ����һ��״̬
		//bool isAcc;// �Ƿ񵽴�Acc ��ʵ����next_state�Ѿ����������ж�
	};
	vector<vector<Action> >g_action;
	map<pair<int, char>, Action>g_actions;

	// ����
	int isDuplicated(Project p);
	bool isDuplicated(Item item, Item* items, int sz);

	// ��ӡ
	void print(vector<char> v);
	void printSet(vector<pair<char, vector<char> > >v);

	// ׼������
	void loadGrammar();
	void getSet(vector<char>&v, char nt, int index);
	void getFirstSet();
	void writeFirstSet();
	void getClosure(Project& pr, map<char, vector<string> >tmp);
	void constructItemSet();
	void writeItemSet();
	void constructAGT();

	// ׼�� + ����
	void LR1();
	void process(char T[200], char(*Res)[500], int& flag, char& error, int& Size);
};

