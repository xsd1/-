#pragma once
#include "pch.h"
#include "LA.h"
#include "Common.h"
void LA::Token::toString() {
	printf("line %-5d %-15s%-10s", this->line, typeOfToken[this->type].c_str(), this->token.c_str());
}

// 关键词
bool LA::isKeyWord(string s) {
	return isExist(keyword, s);
}
// 界符
bool LA::isDelimiter(char ch) {
	return isExist(Delimiter, ch);
}
// 双目运算符
bool LA::isBO(char a) {
	return isExist(B_Operator, a);
}
// 单目运算符
bool LA::isMO(char a[]) {
	return isExist(M_Operator, a);
}
// 关键词
bool LA::isKW(char a[]) {
	return isExist(keyword, (string)a);
}
// 字母
bool LA::isAlpha(char ch) {
	return isalpha(ch);
}
// 数字
bool LA::isDigit(char ch) {
	return isdigit(ch);
}

// 创造NFA
void LA::createNFA() {

	// printf("grammar --> NFA...\n");
	// Sleep(1000);
	// std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // 休眠1000毫秒 一会查

	ifstream inn("my_res\\词法分析_文法.txt"); // 读入正规文法
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
		// 不存在就放到数组里
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
				g_DFA_trans[pair<int, char>(num, chh)] = i;// 状态 Tnum --> chh --> Ti
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
			//printf("%s", "终态：");
			s = "终态：";
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
	int first_state = 0; // DFA 初状态为 0 
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
	fopen_s(&fp, "my_res\\词法分析_源程序.txt", "r");
	// fp = fopen("测试.txt", "r");
	if (fp == NULL) {
		//perror("打开文件时发生错误");
		AfxMessageBox("打开文件时发生错误");
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
		if (isDigit(ch)) { // 常量 科学计数法、复数
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
		if (isAlpha(ch) || ch == '_') { // 标识符  字母或下划线开头
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
		if (isDelimiter(ch)) { // 界符
			token = { line, 5, string(1, ch) };
			Tokens.push_back(token);
		}
		if (isBO(ch)) {   // 单目运算符 或者可能是注释
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
	//printf("\n\n....扫描成功....\n");
	sprintf_s(a,"共 %d 个token序列***序列如下\n\n", Tokens.size());
	int i = 0;
	strcpy_s(T[i++], "....扫描成功....\n");
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
		case 2: off << 3; // 常量
			break;
		case 3: off << 2; // 标识符
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