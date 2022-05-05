#include "pch.h"
#include "PA.h"

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
// 利用set去重
template<typename T>
void setV(T& a) {
	set<char>s(a.begin(), a.end());
	a.assign(s.begin(), s.end());
}

void PA::Item::toString() {
	cout << left << "   " << right << "   " << dot << "   " << s;
}
bool PA::Item::operator==(const Item& it)const {
	return left == it.left && right == it.right && dot == it.dot && s == it.s;
}

bool PA::Project::operator==(const Project& p)const {
	if (sz_item != p.sz_item)return false;
	for (auto item : p.items) {
		if (!isExist(items, item))return false;
	}
	return true;
}

// Project内部函数
bool PA::Project::isExists(Item item) {
	for (int i = 0; i < sz_item; i++) {
		if (item == items[i])return true;
	}
	return false;
}
void PA::Project::toString() {
	for (auto item : items) {
		item.toString();
		cout << endl;
	}
}


// 判重
int PA::isDuplicated(Project p) {
	for (int i = 0; i < sz_project; i++) {
		if (project[i] == p)return i + 1;
	}
	return 0;
}
bool PA::isDuplicated(Item item, Item* items, int sz) {
	for (int i = 0; i < sz; i++) {
		if (items[i] == item)return true;
	}
	return false;
}

// 打印
void PA::print(vector<char> v) {
	for (auto ch : v) {
		cout << ch << ' ';
	}
	cout << endl;
}
void PA::printSet(vector<pair<char, vector<char> > >v) {
	for (auto p : v) {
		print(p.second);
	}
}

// 导入文法
void PA::loadGrammar() {
	ifstream inn("my_res\\语法分析_文法.txt");
	if (!inn) {
		//perror("file open error!");
		AfxMessageBox("文件打开失败");
		return;
	}
	inn >> g_Len;
	char right;
	char left, hyphen, arrow;
	char temp;
	string tmp;
	g_grammar.push_back({ 'S', "P" });// 拓广文法
	g_state_set.push_back('S');
	for (int i = 0; i < g_Len; i++) {
		inn >> left;
		if (!isExist(g_state_set, left))
			g_state_set.push_back(left);
		inn >> hyphen >> arrow;
		tmp = "";
		while (true) {
			inn >> temp;
			if (temp == '$')break;
			tmp += temp;
			if ((temp<'A' || temp>'Z') && !isExist(g_final_set, temp))g_final_set.push_back(temp);
		}
		g_grammar.push_back({ left, tmp });
	}
	inn.close();
}
// 计算first集
void PA::getSet(vector<char>&v, char nt, int index) {
	vector<char>vs;
	vs.clear();
	for (auto p : g_grammar) {
		if (p.first == nt) {
			int sz = p.second.size();
			vector<char>vv;
			for (int i = 0; i < sz; i++) {
				char right = p.second[i];
				if (isExist(g_final_set, right)) {
					if (right == '@')right = '#';
					vv.push_back(right);
					break;
				}
				else {
					if (right == nt)continue;
					int nindex = isExist(g_state_set, right) - 1;
					if (bs[nindex]) {
						vv.insert(vv.end(), g_first_set[nindex].begin(), g_first_set[nindex].end());
					}
					else getSet(vv, right, nindex);
					if (!isExist(vv, '#'))break;
				}
			}
			vs.insert(vs.end(), vv.begin(), vv.end());
		}

	}
	v.insert(v.end(), vs.begin(), vs.end());
	setV(v);
	g_first_set[index] = v;
	bs[index] = 1;
}
void PA::getFirstSet() {
	vector<char>v;
	g_first_set.resize(50);
	g_first_set[0] = { '#' };
	bs[0] = 1;
	for (int i = 1; i < g_state_set.size(); i++) {
		char l = g_state_set[i];
		v.clear();
		if (bs[i])continue;
		getSet(v, l, i);
		// for(auto ch : v){
		//     cout << ch << ' '; // 输出顺序不按照getSet顺序 因为set去重
		// }
		// cout << endl;
	}
}

// 项目集闭包
void PA::getClosure(Project& pr, map<char, vector<string> >tmp) {
	Item items[100];
	for (int i = 0; i < pr.items.size(); i++) {
		items[i] = pr.items[i];
	}
	for (int i = 0; i < pr.sz_item; i++) {
		Item item = items[i];
		int index = item.dot;
		string s = item.right;
		if (index < s.size()) {// 点不在最后一位 判断点后面的是否为非终结符
			if (isExist(g_state_set, s[index])) {// 如果是非终结符
				vector<string>right = tmp[s[index]];// 添加新的Item
				index++;
				string str = s.substr(index, s.size() - index);// βa的的first集
				str += item.s;
				for (auto ch : str) {
					if (!isExist(g_state_set, ch)) { // 终结符
						for (auto it : right) {
							Item p({ s[index - 1], it, 0, ch });
							if (isDuplicated(p, items, pr.sz_item))continue;
							items[pr.sz_item++] = p;
						}
						break;
					}
					else {
						int nindex = isExist(g_state_set, ch) - 1;
						vector<char>t = g_first_set[nindex];// VN的first集
						for (auto chh : t) {// 对于first的每一个元素 都要作为向前搜索符出现一次
							for (auto it : right) {
								Item p({ s[index - 1], it, 0, chh });
								if (isDuplicated(p, items, pr.sz_item))continue;
								items[pr.sz_item++] = p;
							}
						}
						if (!isExist(t, '#')) {
							break;//非终结符的first集不包含#后面的不必再看了
						}
					}
				}
			}
		}
	}
	pr.items.clear();
	for (int i = 0; i < pr.sz_item; i++) {
		pr.items.push_back(items[i]);
	}
}

// 构造项目集
void PA::constructItemSet() {

	map<char, vector<string> >tmp;
	for (auto p : g_grammar) {
		tmp[p.first].push_back(p.second); // 左部相同的产生式放到一组 方便求闭包
	}

	Item item({ 'S', "P", 0, '#' });
	Project pr;
	pr.num = 0;
	pr.sz_item = 1;
	pr.next.clear();
	pr.items.push_back(item);
	getClosure(pr, tmp);// I0闭包
	project[sz_project++] = pr;

	bitset<300>bst;

	for (int i = 0; i < sz_project; i++) {
		Project pr = project[i];
		bst.reset();
		for (auto item : pr.items) {
			if (item.dot < item.right.size() && !bst[item.right[item.dot]]) {
				bst[item.right[item.dot]] = 1;// 该移进符号标记
				Project p({ 0, 1 });
				p.next.clear();
				char ch = item.right[item.dot];// 状态转换的字符
				Item temp({ item.left, item.right, item.dot + 1, item.s });
				p.items.push_back(temp);
				// 找相同的移进Item
				for (auto item1 : pr.items) {
					if (item1.right == item.right && item1.s == item.s)continue;// 重复
					if (item1.dot < item1.right.size() && item1.right[item1.dot] == ch) {
						Item temp({ item1.left, item1.right, item1.dot + 1, item1.s });
						p.items.push_back(temp);
						p.sz_item++;
					}
				}
				p.num = sz_project;// 项目编号
				getClosure(p, tmp);// 闭包
				project[i].ch_trans.push_back(ch); // 转换

				int index = isDuplicated(p);// 项目集是否已有该项目集
				if (index == 0) { // 没有
					project[sz_project++] = p; // 加进去
					project[i].next.push_back(p.num);
				}
				else {
					index--;// 已存在
					if (!isExist(project[i].next, index)) {
						project[i].next.push_back(index);
					}
				}
			}
		}
	}
	// vector数组insert后迭代器失效，需重新更新
	/*for(auto pr : project){
		int i=0;
		// cout << pr.num << endl;
		for(auto item : pr.items){
			if(item.dot!=item.right.size()){// 点不在最后
				Project p;

				// 点所在下标的字符就是状态转换的字符
				char ch = item.right[item.dot];
				Item temp({item.left, item.right, item.dot+1, item.s});
				p.items.push_back(temp);
				for(auto it : pr.items){
					if(it.right==item.right)continue;// 加过一次了
					if(it.dot<it.right.size() && it.right[it.dot]==ch){
						Item temp({it.left, it.right, it.dot+1, it.s});
						p.items.push_back(temp);
					}
				}
				p.num=sz_project++;
				getClosure(p, tmp);
				// cout << i++ << "------------\n";
				// p.toString();
				// cout << "------------\n";
				int nindex = isExist(project, p)-1;

				if(nindex==-1){
					cout << -1  << ' ';
					pr.next.push_back(sz_project);
					project.push_back(p);
					cout << project.size() << endl;
				}
				else {
					pr.next.push_back(nindex);
				}
			}
		}
	}*/
}

// 构造Action/GOTO表
void PA::constructAGT() {
	// ofstream off("test.txt");
	ofstream off("my_out\\PA\\LR1分析表.txt");
	off << "-------------------------------------------------LR1(1)分析表-------------------------------------------------n";
	char a[400];
	sprintf_s(a, "%-85s%-155s%s\n", "状态", "Action", "GOTO");
	off << a;
	vector<char>v;
	v.insert(v.end(), g_final_set.begin(), g_final_set.end());
	v.insert(v.end(), g_state_set.begin(), g_state_set.end());
	for (int i = 0; i < v.size(); i++) {
		if (v[i] == '@')v[i] = '#';
	}
	off << "      ";
	for (auto ch : v) {
		off << ch << "      ";
	}

	off << "\n";
	for (int i = 0; i < sz_project; i++) {
		int index;
		Project p = project[i];
		off << i << "    ";
		if (i < 10)off << ' ';
		for (auto ch : g_final_set) {
			if (ch == '@')ch = '#';
			int flag = 0;
			for (auto item : p.items) {
				if ((item.right.size() == 1 && item.right == "@" || item.dot == item.right.size()) && ch == item.s) {// 遇到向前搜索符规约
					flag = 1;
					int index = isExist(g_grammar, pair<char, string>(item.left, item.right)) - 1;
					//Action a({ item.s, 'r', index });
					Action a;
					a.act = 'r';
					a.ch = item.s;
					a.next_state = index;
					g_actions[pair<int, char>(i, item.s)] = a;
					off << 'r' << index << "    ";
					break;
				}
				else if (ch == item.right[item.dot]) {
					flag = 1;
					int index = isExist(p.ch_trans, ch) - 1;
					int next_state = p.next[index];
					//Action a({ ch, 'S', next_state });
					Action a;
					a.act = 'S';
					a.ch = ch;
					a.next_state = next_state;
					g_actions[pair<int, char>(i, ch)] = a;
					off << 'S' << next_state << "    ";
					break;
				}
			}
			if (!flag) {
				off << "       ";
			}
		}
		for (auto ch : g_state_set) {
			int flag = 0;
			for (auto item : p.items) {
				if (item.dot < item.right.size() && item.right[item.dot] == ch) {
					flag = 1;
					int index = isExist(p.ch_trans, ch) - 1;
					int go = p.next[index];
					//Action a({ ch, 'g', go });
					Action a;
					a.act = 'g';
					a.ch = ch;
					a.next_state = go;
					g_actions[pair<int, char>(i, ch)] = a;
					off << go << "    ";
					break;
				}
			}
			if (!flag) {
				off << "       ";
			}
		}
		off << '\n';
	}
	off.close();
}

//写first集、项目集
void PA::writeFirstSet() {
	ofstream out("my_out\\PA\\first集.txt");
	if (!out) {
		cout << "error!";
		return;
	}
	for (int i = 0; i < g_state_set.size(); i++) {
		out << "first(" << g_state_set[i] << ") = {";
		for (int j = 0; j < g_first_set[i].size(); j++) {
			out << g_first_set[i][j];
			if (j != g_first_set[i].size() - 1)out << ',';
		}
		out << "}\n";
	}
	out.close();
}
void PA::writeItemSet() {
	ofstream off("my_out\\PA\\项目集.txt");
	off << "项目集的个数为 " << sz_project << '\n';
	for (int i = 0; i < sz_project; i++) {
		off << project[i].num << '\n';
		for (auto item : project[i].items) {
			off << item.left << "   " << item.right << "   " << item.dot << "   " << item.s;
			off << endl;
		}
		off << endl;
	}
	off.close();
	// cout << "\n项目集的个数为 " << project.size() << '\n';
	// for(auto p : project){
	//     cout << p.num << "    ";
	//     // for(auto nex : p.next){
	//     //     cout << nex << ' ';
	//     // }
	//     cout << endl;
	// }   
}

void PA::LR1() {
	loadGrammar();
	getFirstSet();
	writeFirstSet();
	constructItemSet();
	writeItemSet();
	constructAGT();
}

void PA::process(char T[200], char(*Res)[500], int& flag, char& error, int& Size) {
	FILE* fp;
	fopen_s(&fp, "my_out\\LA\\token2.txt", "r");
	if (!fp) {
		//perror("file open error!");
		AfxMessageBox("文件打开失败");
		return;
	}

	char ch;
	int len_sign = 0;
	int len_state = 0;
	char str[300];
	char sign[100];
	int states[100];
	memset(str, 0, sizeof(str));
	memset(sign, 0, sizeof(sign));
	memset(states, 0, sizeof(states));
	sign[0] = '#';

	int len_str = 0;
	while ((ch = fgetc(fp)) != EOF) {
		str[len_str++] = ch;
	}
	str[len_str++] = '#';
	strcpy_s(T, 190, str);
	ofstream off("my_out\\PA\\分析过程.txt");
	if (!off) {
		AfxMessageBox("file create error!");
		return;
	}
	char out[300];
	sprintf_s(out, "%-16s%-41s%-99s%-65s%-10s%-15s%s\n", "步骤", "符号栈", "状态栈", "输入符号串", "动作", "ACTION", "GOTO");
	off << out;
	strcpy_s(Res[flag++], out);
	int step = 0;

	while (true) {
		string s_res = "";
		int state = states[len_state];// len指向当前状态栈最外面一个
		char ch = str[step];
		sprintf_s(out, "%d   ", flag);
		off << out;
		s_res += out;
		sprintf_s(out, "%s    ", sign);
		off << out;
		s_res += out;
		string s = "";
		for (int i = 0; i <= len_state; i++) {
			s += to_string(states[i]) + ' ';
		}
		sprintf_s(out, "%s    ", s.c_str());
		off << out;
		s_res += out;
		sprintf_s(out, "%s    ", str + step);
		off << out;
		s_res += out;
		off << "       ";
		s_res += "       ";

		// cout << ch;
		if (g_actions.count({ state, ch })) {
			Action a = g_actions[{state, ch}];
			if (a.next_state == 0) {
				//cout << "YES";
				off << "接受          acc";
				s_res += "接受          acc";
				strcpy_s(Res[flag++], s_res.c_str());
				return;
			}
			if (a.act == 'S') {
				states[++len_state] = a.next_state;
				sign[++len_sign] = ch;
				off << "移进          S" << a.next_state;
				s_res += "移进          S" + to_string(a.next_state);
			}
			if (a.act == 'r') {
				off << "规约          r" << a.next_state << "          ";// 
				s_res += "规约          r" + to_string(a.next_state) + "          ";
				int sz = g_grammar[a.next_state].second.size();
				if (g_grammar[a.next_state].second != "@") {
					len_sign -= sz;
					len_state -= sz;
				}
				sign[++len_sign] = g_grammar[a.next_state].first;
				sign[len_sign + 1] = '\0';
				if (g_actions.count({ states[len_state], sign[len_sign] })) {
					Action aa = g_actions[{states[len_state], sign[len_sign]}];
					states[++len_state] = aa.next_state;
					off << aa.next_state;
					s_res += to_string(aa.next_state);
				}
				else {
					//perror("process error!");
					flag = -2;
					error = ch;
					return;
				}
				off << '\n';
				if (flag < 500) {
					strcpy_s(Res[flag++], s_res.c_str());
					Size = flag;
				}
				else {
					flag = -1;
					return;
				}
				continue;
			}
		}
		else {
			flag = -2;
			//AfxMessageBox("No");
			error = ch;
			return;
		}
		step++;
		off << '\n';
		if (flag < 500) {
			strcpy_s(Res[flag++], s_res.c_str());
			Size = flag;
		}
		else {
			flag = -1;
			return;
		}
	}
}
