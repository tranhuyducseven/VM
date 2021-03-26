#ifndef VM_H
#define VM_H

#include "main.h"
class VM;
class DataStorage;
class Instruction;
int pow(int base, int exp);
string eraseChar(string str, char c);
string eraseCharAtIndex(string str, int index);
int checkRegister(string string, int sizeOfString);
int checkOperand2_Arithmetic(string str, DataStorage& value);
int countNCode(string filename);
class DataStorage
{
private:
	friend class VM;

private:
	int dataInt;
	float dataFloat;
	bool dataBool;
	int checkType;

public:
	DataStorage();
	int getDataInt();
	void setDataInt(int data);
	float getDataFloat();
	void setDataFloat(float data);
	bool getDataBool();
	void setDataBool(bool data);
	int getTypeData();
	void setTypeData(int check);
	~DataStorage();
};
class Instruction
{
private:
	friend class VM;

private:
	string nameOpcode;
	int nOperands;
	string op1;
	string op2;

public:
	Instruction();
	string getNameOpcode();
	string getOp1();
	string getOp2();
	int getNOperands();
	Instruction getElementInstruction(string Code);
	~Instruction();
};

class VM
{

private:
	string* codes;
	int* stack;
	DataStorage* staticMemory;
	DataStorage* Register;
	int nCode;
	int ip;
	int sp;
	Instruction* instr;

public:
	VM();
	VM(Instruction* instr, int ip, int nCode);
	void run(string filename);
	void readCode(string filename);
	void cpu();
	~VM();
};

#endif
