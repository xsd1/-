#pragma once
#include "pch.h"
#include "LA.h"
#include "Common.h"
void LA::Token::toString() {
	printf("line %-5d %-15s%-10s", this->line, typeOfToken[this->type].c_str(), this->token.c_str());
}

// �ؼ���
bool LA::isKeyWord(string s) {
	return isExist(keyword, s);
}
// ���
bool LA::isDelimiter(char ch) {
	return isExist(Delimiter, ch);
}
// ˫Ŀ�����
bool LA::isBO(char a) {
	return isExist(B_Operator, a);
}
// ��Ŀ�����
bool LA::isMO(char a[]) {
	return isExist(M_Operator, a);
}
// �ؼ���
bool LA::isKW(char a[]) {
	return isExist(keyword, (string)a);
}
// ��ĸ
bool LA::isAlpha(char ch) {
	return isalpha(ch);
}
// ����
bool LA::isDigit(char ch) {
	return isdigit(ch);
}

// ����NFA
void LA::createNFA() {

	// printf("grammar --> NFA...\n");
	// Sleep(1000);
	// std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // ����1000���� һ���

	ifstream inn("my_res\\�ʷ�����_�ķ�.txt"); // ���������ķ�
	if (!inn) {
		printf("file open failed\n");
		return;
	}
	inn >> g_Len >> g_start;
	g_state_set.push_back(g_start);
	while (g_Len--) {
		char left, hyphen, arrow;
		char right[3];
		inn >> left >> hyphen >> arrow >> right;
		// �����ھͷŵ�������
		if (!isExist(g_state_set, left)) {
			g_state_set.push_back(left);
		}

		if (!isExist(g_final_set, right[0])) {
			g_final_set.push_back(right[0]);
		}
		char right_state = strlen(right) == 1 ? 'Z' : right[1];
		g_NFA_set[pair<char, char>(left, right[0])].push_back(right_state);
	}
}

void LA::getClosure(vector<char>& T) {

	for (auto ch : T) {
		if (g_NFA_set.count(pair<char, char>(ch, '@'))) {
			for (auto chh : g_NFA_set[(pair<char, char>(ch, '@'))]) {
				if (!isExist(T, chh)) {
					T.push_back(chh);
				}
			}
		}
	}
}

void LA::nfaToDfa() {

	vector<char>T0;
	T0.push_back(g_start);
	getClosure(T0);

	bool isFinalState = false;
	if (isExist(T0, 'Z'))isFinalState = true;
	g_DFA_set.push_back(T0);
	g_DFA_IsFinal.push_back(isFinalState);

	stack<pair<vector<char>, int>>sk;
	int i = 0;
	sk.push(pair<vector<char>, int>(T0, i));

	while (!sk.empty()) {
		vector<char>T = sk.top().first;
		int num = sk.top().second;
		sk.pop();
		set<char>s;
		vector<char>v;
		for (auto chh : g_final_set) {
			s.clear();
			for (auto ch : T) {
				if (g_NFA_set.count(pair<char, char>(ch, chh))) {
					vector<char>vv = g_NFA_set[pair<char, char>(ch, chh)];
					s.insert(vv.begin(), vv.end());
				}
			}
			v.assign(s.begin(), s.end());
			getClosure(v);
			s.clear();
			s.insert(v.begin(), v.end());
			v.assign(s.begin(), s.end());
			isFinalState = false;

			int IsExist = isExist(g_DFA_set, v);
			if (!IsExist&&v.size()) {
				if (isExist(v, 'Z'))isFinalState = true;
				g_DFA_IsFinal.push_back(isFinalState);
				g_DFA_set.push_back(v);
				i++;
				g_DFA_trans[pair<int, char>(num, chh)] = i;// ״̬ Tnum --> chh --> Ti
				sk.push(pair<vector<char>, int>(v, i));
			}
			else if (IsExist) {
				g_DFA_trans[{num, chh}] = IsExist - 1;
			}
		}
	}
}

void LA::printDFA(char(*T)[200], int Size, int& flag) {
	vector<bool>::iterator it1 = g_DFA_IsFinal.begin();
	vector<vector<char>>::iterator it = g_DFA_set.begin();
	int len = 0;
	string s;
	ofstream off("my_out\\LA\\DFA.txt");
	for (; it1 != g_DFA_IsFinal.end() && it != g_DFA_set.end(); it++, it1++) {
		s = "";
		if (*it1) {
			//printf("%s", "��̬��");
			s = "��̬��";
		}
		for (auto ch : *it) {
			//printf("%c ", ch);
			s = s + ch + " ";
		}
		off << s << '\n';
		if (len < Size) {
			strcpy_s(T[len++], s.c_str());
		}
		else {
			flag = -1;
			return;
		}
		//printf("\n");
	}
	off.close();
	flag = len;
}

bool LA::dfaCheck(char a[]) {
	int first_state = 0; // DFA ��״̬Ϊ 0 
	for (int i = 0; i < strlen(a); i++) {
		if (g_DFA_trans.count(pair<int, char>(first_state, a[i]))) {
			first_state = g_DFA_trans[pair<int, char>(first_state, a[i])];
		}
		else return false;
	}
	if (g_DFA_IsFinal[first_state])
		return true;
	return false;
}

void LA::scanSourceFile() {
	FILE* fp = NULL;
	fopen_s(&fp, "my_res\\�ʷ�����_Դ����.txt", "r");
	// fp = fopen("����.txt", "r");
	if (fp == NULL) {
		//perror("���ļ�ʱ��������");
		AfxMessageBox("���ļ�ʱ��������");
		return;
	}
	int line = 1;
	char ch;
	Token token;
	char* str = new char[100];
	int len;
	while ((ch = fgetc(fp)) != EOF) {
		if (ch == ' ')continue;
		if (ch == '\n') {
			line++;
			continue;
		}
		memset(str, 0, sizeof(str));
		len = 0;
		if (isDigit(ch)) { // ���� ��ѧ������������
			str[len++] = ch;
			while ((ch = fgetc(fp)) != EOF && isDigit(ch) || isAlpha(ch) || ch == '.' || ch == '+' || ch == '-') {
				str[len++] = ch;
			}
			str[len] = '\0';
			if (dfaCheck(str)) {
				token = { line, 2, str };
			}
			else {
				token = { line, 0, str };
			}
			Tokens.push_back(token);
			len = 0;
		}
		if (isAlpha(ch) || ch == '_') { // ��ʶ��  ��ĸ���»��߿�ͷ
			str[len++] = ch;
			while ((ch = fgetc(fp)) != EOF && isDigit(ch) || isAlpha(ch)) {
				str[len++] = ch;
			}
			str[len] = '\0';

			if (isKeyWord(str)) {
				token = { line, 4, str };
			}
			else if (dfaCheck(str)) {
				token = { line, 3, str };
			}
			else {
				token = { line, 0, str };
			}
			Tokens.push_back(token);
		}
		len = 0;
		if (isDelimiter(ch)) { // ���
			token = { line, 5, string(1, ch) };
			Tokens.push_back(token);
		}
		if (isBO(ch)) {   // ��Ŀ����� ���߿�����ע��
			// while((ch=fgetc(fp))!=EOF){
			str[len++] = ch;
			ch = fgetc(fp);
			if (ch != EOF) {
				str[len++] = ch;
				str[len] = '\0';
				if (isMO(str)) {
					token = { line, 1, str };
					Tokens.push_back(token);
					continue;
				}
				else {
					ungetc(ch, fp);
				}
			}
			token = { line, 1, string(1, str[0]) };
			Tokens.push_back(token);
		}
	}
	fclose(fp);
}

void LA::showTokens(char(*T)[200], int Size, int& flag) {
	ofstream out("my_out\\LA\\token1.txt");
	ofstream off("my_out\\LA\\token2.txt");
	char a[100];
	memset(a, 0, sizeof(a));
	//printf("\n\n....ɨ��ɹ�....\n");
	sprintf_s(a,"�� %d ��token����***��������\n\n", Tokens.size());
	int i = 0;
	strcpy_s(T[i++], "....ɨ��ɹ�....\n");
	strcpy_s(T[i++], a);
	for (auto token : Tokens) {
		//token.toString();
		sprintf_s(a, "line %-5d %-15s%-12s", token.line, typeOfToken[token.type].c_str(), token.token.c_str());
		if (i < Size) {
			strcpy_s(T[i++], a);
		}
		else {
			flag = -1;
			return;
		}
		switch (token.type)
		{
		case 0:
			break;
		case 1: off << token.token;
			break;
		case 2: off << 3; // ����
			break;
		case 3: off << 2; // ��ʶ��
			break;
		case 4: off << abbrOfKwd[token.token];
			break;
		case 5:
			if (token.token == "#")off << '*';
			else off << token.token;
			break;
		case 6: off << token.token;
			break;
		default:
			break;
		}
		out << a << '\n';
		printf("\n");
	}
	out.close();
	flag = i;
}

void LA::main() {
	this->createNFA();
	this->nfaToDfa();
	//scanSourceFile();
	//showTokens();
}