#pragma once
#include <bits/stdc++.h>
using namespace std;
class LA
{
public:
	vector<char>::iterator it;
	// ȫ�ֱ���
	int g_Len; // �ķ��ĳ���
	char g_start;// �ķ���ʼ����
	vector<char>g_state_set;// ���ս����
	vector<char>g_final_set; // �ս����
	map<pair<char, char>, vector<char>>g_NFA_set;   // ״̬A �����ս�� ת����״̬B
	map<pair<int, char>, int>g_DFA_trans; // DFA֮���ת�� T0 -> T1...
	vector<vector<char>>g_DFA_set;
	vector<bool>g_DFA_IsFinal;// �жϸ�״̬�Ƿ�����̬

	struct Token {
		int line;// Դ����������к�
		int type;// ���ͱ�� 0��1��2����
		string token;// �����ַ���
		void toString();// ��ӡ����
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

	// �жϺ���
	bool isMO(char a[]);
	bool isBO(char a);
	bool isKW(char a[]);
	bool isAlpha(char ch);
	bool isDigit(char ch);
	bool isKeyWord(string s);
	bool isDelimiter(char ch);

	// ������
	void createNFA();
	void getClosure(vector<char>& T);
	void nfaToDfa();
	void printDFA(char(*T)[200], int Size, int& flag);
	bool dfaCheck(char a[]);
	void scanSourceFile();
	void showTokens(char(* T)[200], int Size, int& flag);

	// ��ʼ
	void main();
};

