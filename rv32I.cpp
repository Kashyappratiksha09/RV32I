#include<iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bitset>
using namespace std;

struct lbl
{
	string name;
	int pos;
};

void FillRegister(string regs[][2]);
void FillFunct(string functs[][5]);
void FillLabels(string filename, vector <lbl> &v);
int GetBranchImmediate(int currentline, string lab, vector <lbl> &v);
string ReturnRegister(string s, string regs[][2]);
int LineIndex(string s, string funct[][5]);
bool CheckString(string s);

int main( )
{

	string regs[33][2];
	string functs[38][5];
	string temp;
	string print;

	string opcode, funct3, funct7, rd, rs1, rs2, rt;
	vector <lbl> v;
	string filename = "input.txt";
	ifstream assembly;
	ofstream binary;
	int x, t, currentline = 0;
	int offset;
	char c;
	string I_imm_string;
	string S_imm_string;
	string B_imm_string;
	string  U_imm_string;
	string J_imm_string;
	FillLabels(filename, v);

	assembly.open("input.txt");
	binary.open("output.txt");
	FillFunct(functs);
	FillRegister(regs);


	for (int i = 0; i < v.size(); i++)
	{
		cout << v[i].name;
		cout << v[i].pos << endl;
	}

	for (int i = 0; i < 33; i++)
	{
		for (int j = 0; j < 2; j++)
			cout << regs[i][j] << " ";
		cout << endl;
	}
	do
	{
		getline(assembly, temp, ' ');
		if (CheckString(temp))
			getline(assembly, temp, ' ');
		x = LineIndex(temp, functs);
		c = functs[x][4].at(0);
		switch (c)
		{
		case 'I':
		{
			opcode = functs[x][1];
			funct3 = functs[x][2];
			if ((temp == "lb") || (temp == "lbu") || (temp == "lh") || (temp == "lhu") || (temp == "lw"))
			{
				getline(assembly, temp, ',');
				rd = ReturnRegister(temp, regs);
				getline(assembly, temp, '(');
				bitset<12> I_imm(stoi(temp));
				I_imm_string = I_imm.to_string();
				getline(assembly, temp, ')');
				rs1 = ReturnRegister(temp, regs);
				getline(assembly, temp, '\n');
			}

			else
			{
				getline(assembly, temp, ',');
				rd = ReturnRegister(temp, regs);
				getline(assembly, temp, ',');
				rs1 = ReturnRegister(temp, regs);
				getline(assembly, temp, '\n');
				bitset<12> I_imm(stoi(temp));
				I_imm_string = I_imm.to_string();

			}
			print = I_imm_string + rs1 + funct3 + rd + opcode;
			cout << "I-format " << print << endl;
		}
		break;

		case 'B':
		{
			opcode = functs[x][1];
			getline(assembly, temp, ',');
			rs1 = ReturnRegister(temp, regs);
			getline(assembly, temp, ',');
			rs2 = ReturnRegister(temp, regs);
			getline(assembly, temp, '\n');
			offset = GetBranchImmediate(currentline, temp, v) - 1;
			bitset<13> S_imm(offset);
			funct3 = functs[x][2];
			S_imm_string = S_imm.to_string();
			print = S_imm_string[0] + S_imm_string.substr(2, 6) + rs2 + rs1 + funct3 + S_imm_string.substr(8, 4) + S_imm_string[1] + opcode;
			cout << "B-format " << print << endl;
		}
		break;

		case 'U':
		{
			opcode = functs[x][1];
			getline(assembly, temp, ',');
			rd = ReturnRegister(temp, regs);
			getline(assembly, temp, '\n');
			bitset<32>U_imm(stoi(temp));
			U_imm_string = U_imm.to_string();
			print = U_imm_string.substr(0, 20) + rd + opcode;
			cout << "U-format " << print << endl;

		}
		break;

		case 'S':
		{
			opcode = functs[x][1];
			funct3 = functs[x][2];

			if ((temp == "sw") || (temp == "sh") || (temp == "sb"))
			{

				getline(assembly, temp, ',');
				rd = ReturnRegister(temp, regs);
				getline(assembly, temp, '(');
				bitset<12> S_imm(stoi(temp));
				S_imm_string = S_imm.to_string();
				getline(assembly, temp, ')');
				rs1 = ReturnRegister(temp, regs);
				getline(assembly, temp, '\n');
			}
			else
			{
				getline(assembly, temp, ',');
				rs1 = ReturnRegister(temp, regs);
				getline(assembly, temp, ',');
				rs2 = ReturnRegister(temp, regs);
				getline(assembly, temp, '\n');
				bitset<12> S_imm(atoi(temp.c_str()));
				S_imm_string = S_imm.to_string();
			}
			print = S_imm_string.substr(0, 7) + rs2 + rs1 + funct3 + S_imm_string.substr(7, 5) + opcode;
			cout << "S-format " << print << endl;
		}
		break;

		case 'R':
		{
			opcode = functs[x][1];
			getline(assembly, temp, ',');
			rd = ReturnRegister(temp, regs);
			getline(assembly, temp, ',');
			rs1 = ReturnRegister(temp, regs);
			getline(assembly, temp, '\n');
			rs2 = ReturnRegister(temp, regs);
			funct7 = functs[x][3];
			print = funct7 + rs2 + rs1 + funct3 + rd + opcode;
			cout << "R-format " << print << endl;
		}
		break;

		case 'J':
		{
			opcode = functs[x][1];
			getline(assembly, temp, ',');
			rd = ReturnRegister(temp, regs);
			getline(assembly, temp, '\n');
			offset = GetBranchImmediate(currentline, temp, v) - 1;
			bitset<21> J_imm(offset);
			J_imm_string = J_imm.to_string();
			print = J_imm_string[0] + J_imm_string.substr(10, 10) + J_imm_string[9] + J_imm_string.substr(1, 8) + rd + opcode;
			cout << "UJ-format " << print << endl;
		}
		break;

		default:
			cout << "Type Not Found!!";
		}
		cout << currentline << endl;
		currentline++;

		binary.write(print.c_str(), print.length());
		binary.put('\n');

	} while (!assembly.eof());


	assembly.close();
	binary.close();
	system("pause");
}

int LineIndex(string s, string functs[][5])
{

	for (int i = 0; i < 38; i++)
		if (s == functs[i][0])
			return i;

}

string ReturnRegister(string s, string regs[][2])
{
	for (int i = 0; i < 33; i++)
	{
		if (s == regs[i][0])
			return regs[i][1];
	}

	return "-1";

}

void FillFunct(string funct[][5])
{
	ifstream function;
	function.open("Functions.txt");
	string temp;

	for (int i = 0; i <38; i++)
		for (int j = 0; j<5; j++)
		{
			if (j == 4)
				getline(function, temp, '\n');
			else
				getline(function, temp, ' ');
			funct[i][j] = temp;
		}


	function.close();
}

void FillRegister(string regs[][2])
{
	ifstream registers;
	registers.open("Registers.txt");
	string temp;

	for (int i = 0; i <33; i++)
		for (int j = 0; j < 2; j++)
		{
			if (j == 1)
				getline(registers, temp, '\n');
			else
				getline(registers, temp, ' ');
			regs[i][j] = temp;
		}


	registers.close();
}
void FillLabels(string filename, vector <lbl> &v)
{
	ifstream label;
	label.open(filename);
	string temp;
	lbl x;
	int i = 1;

	do
	{
		getline(label, temp, ' ');
		if (CheckString(temp))
		{
			if ((temp[0] == '\n') || (temp[0] == ' '))
				x.name = temp.substr(1, temp.length() - 2);
			else
				x.name = temp.substr(0, temp.length() - 1);
			x.pos = i;
			v.push_back(x);
		}
		getline(label, temp, '\n');
		i++;
	} while (!label.eof());
	label.close();

}
bool CheckString(string s)
{
	for (int i = 0; i < s.length(); i++)
		if (s[i] == ':')
			return true;

	return false;
}
int GetBranchImmediate(int currentpos, string lab, vector<lbl> &v)
{
	int imm = 0;
	for (int i = 0; i < v.size(); i++)
	{
		if (lab == v[i].name)
		{
			imm = v[i].pos - currentpos;
			return imm;
		}
	}
}
