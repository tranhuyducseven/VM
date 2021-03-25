#ifndef VM_H
#define VM_H

#include "main.h"
int checkRegister(string string, int sizeOfString);
int checkOperand2_Arithmetic(string string, int sizeofString);
int countNCode(string filename);
class DataStorage
{
private:
  friend class VM;

private:
  int dataInt;
  float dataFloat;
  bool dataBool;

public:
  DataStorage();
  int getDataInt();
  float getDataFLoat();
  bool getDataBool();
  ~DataStorage();
};

class VM
{
public:
  class Instruction;

private:
  string *codes;
  int *stack;
  DataStorage *staticMemory;
  DataStorage *Register;
  int nCode;
  int ip;
  int sp;
  Instruction *instr;
  


public:
  VM();
  VM(Instruction *instr, int ip, int nCode);
  void run(string filename);
  void readCode(string filename);
  void cpu();

  ~VM();

public:
  class Instruction
  {
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
};

#endif