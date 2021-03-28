#include "VM.h"
//Math
int pow(int base, int exp)
{
    if (exp == 0)
        return 1;
    else
        return base * pow(base, exp - 1);
}
///////////
string eraseChar(string str, char c)
{
    int n = str.length();
    string newString = "";
    for (int i = 0; i < n; i++)
    {
        char currentChar = str[i];
        if (currentChar != c)
            newString += currentChar;
    }
    return newString;
}
string eraseCharAtIndex(string str, int index)
{
    int n = str.length();
    string newString = "";
    for (int i = 0; i < n; i++)
    {
        if (i != index)
        {
            newString += str[i];
        }
    }
    return newString;
}
int countNCode(string filename)
{
    fstream fileInput;
    fileInput.open(filename, ios::in);
    char c;
    int numlines = 0;
    fileInput.get(c);
    while (fileInput)
    {
        while (fileInput && c != '\n')
        {
            fileInput.get(c);
        }
        ++numlines;
        fileInput.get(c);
    }
    return numlines;
}
int checkRegister(string string, int sizeOfString)
{
    int index = 0;
    if (sizeOfString == 2 || sizeOfString == 3)
    {
        if (string[0] == 'R')
        {
            if (sizeOfString == 2)
            {
                index = (int)(string[1]) - 48;
                if (index >= 1 && index <= 9)
                {
                    return index;
                }
                else
                {
                    return -1;
                }
            }
            if (sizeOfString == 3)
            {
                int a = (int)(string[1]) - 48;
                int b = (int)(string[2]) - 48;
                index = a * 10 + b;
                if (index >= 10 && index <= 15)
                {
                    return index;
                }
                else
                    return -1;
            }
        }
        else
            return -1;
    }

    return -1;
}
bool checkSpace(string str)
{
    int n = str.length();
    for (int i = 0; i < n; i++)
    {
        if (str[i] == ' ' || str[i] == '\n' || str[i] == '\r')
        {
            return true;
        }
    }
    return false;
}
int checkOperand2(string str, DataStorage &value)
{
    //return 0 when wrong
    //flag==1 int
    //flag==2 Float
    //flag==3 Bool;
    //flag==4 Address;

    if (str == "true")
    {
        value.setDataBool(true);
        value.setTypeData(3);
        return 3;
    }
    if (str == "false")
    {
        value.setDataBool(false);
        value.setTypeData(3);
        return 3;
    }
    int length = str.length();
    int flag = 1;
    int i;
    if (str[length - 1] == 'A')
    {
        for (int i = 0; i < length - 1; i++)
        {
            if (str[i] >= '0' && str[i] <= '9')
            {
                flag = 4;
            }
            else
            {
                flag = 0;
            }
        }
    }
    for (i = 0; i < length; i++)
    {
        if (str[i] == '.')
        {
            flag = 2;
            break;
        }
    }

    if (flag == 2)
    {
        double num = 0;
        for (int j = 0; j < i; j++)
        {
            if (str[j] >= '0' && str[j] <= '9')
            {
                double temp = (double)((double)((int)(str[j])) - 48);
                num += temp * pow(10, i - j - 1);
            }
            else
                return 0;
        }
        int k = i + 1;
        for (k; k < length; k++)
        {
            if (str[k] >= '0' && str[k] <= '9')
            {
                double temp = (double)((double)((int)(str[k])) - 48);
                int exp = k - i;
                num += temp / pow(10, exp);
            }
            else
                return 0;
        }
        value.setDataFloat(num);
        value.setTypeData(2);
        return 2;
    }
    else if (flag == 1)
    {
        int num = 0;
        int t = 0;
        for (t; t < length; t++)
        {
            if (str[t] >= '0' && str[t] <= '9')
            {
                int exp = length - t - 1;
                num += ((int)(str[t]) - 48) * pow(10, exp);
            }
            else
            {
                return 0;
            }
        }
        value.setDataInt(num);
        value.setTypeData(1);
        return 1;
    }
    else if (flag == 4)
    {
        int address = 0;
        for (int i = 0; i < length - 1; i++)
        {
            address += ((int)(str[i]) - 48) * pow(10, length - 2 - i);
        }
        value.setAddress(address);
        value.setTypeData(4);
        return 4;
    }
    return 0;
}
// // DataStorage Class
// /////////////////////////////////////////////////////////
// /////////////////////////////////////////////////////////
DataStorage::DataStorage()
{
    this->dataInt = 0;
    this->dataFloat = 0;
    this->dataBool = false;
    this->dataAddress = 0;
    this->checkType = 0;
};
int DataStorage::getDataInt()
{
    return this->dataInt;
};
void DataStorage::setDataInt(int data)
{
    this->dataInt = data;
    this->dataAddress = 0;
    this->dataBool = false;
    this->dataFloat = 0;
    this->checkType = 1;
}
double DataStorage::getDataFloat()
{
    return this->dataFloat;
};
void DataStorage::setDataFloat(double data)
{
    this->dataFloat = data;
    this->checkType = 2;
    this->dataInt = data;
    this->dataAddress = 0;
    this->dataBool = false;
}
bool DataStorage::getDataBool()

{
    return this->dataBool;
    this->checkType = 3;
}
void DataStorage::setDataBool(bool data)
{
    this->dataBool = data;
    this->dataInt = 0;
    this->dataAddress = 0;
    this->dataFloat = 0;
    this->checkType = 3;
}
int DataStorage::getAddress()
{
    return this->dataAddress;
}
void DataStorage::setAddress(int address)
{
    this->dataAddress = address;
    this->dataInt = 0;
    this->dataBool = false;
    this->dataFloat = 0;
    this->checkType = 4;
}
int DataStorage::getTypeData()

{
    return this->checkType;
}
void DataStorage::setTypeData(int check)
{
    this->checkType = check;
}
DataStorage::~DataStorage()
{
}
// // INSTRUCTION CLASS
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Instruction::Instruction()
{
    this->nameOpcode = "";
    this->nOperands = 0;
    this->op1 = "";
    this->op2 = "";
}
string Instruction::getNameOpcode()
{
    return this->nameOpcode;
}
string Instruction::getOp1()
{
    return this->op1;
}
string Instruction::getOp2()
{
    return this->op2;
}
int Instruction::getNOperands()
{
    return this->nOperands;
}
Instruction::~Instruction(){};
//VM CLASS
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
VM::VM()
{
    this->nCode = 0;
    this->ip = 0;
    this->sp = -1;
    this->Register = nullptr;
    this->codes = nullptr;
    this->stack = nullptr;
    this->staticMemory = nullptr;
    this->instr = nullptr;
}
VM::VM(Instruction *instr, int ip, int nCode)
{
    this->ip = ip;
    this->nCode = nCode;
    this->instr = instr;
    for (int i = 0; i < nCode; i++)
    {
        this->instr[i] = instr[i];
    }
}
VM::~VM()
{
    delete[] this->codes;
    delete[] this->Register;
    delete[] this->staticMemory;
    delete[] this->stack;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VM::readCode(string filename)
{
    fstream fileInput;
    fileInput.open(filename);
    char c;
    int numlines = 0;
    fileInput.get(c);
    int i = 0;
    while (fileInput)
    {
        while (fileInput && c != '\n')
        {
            this->codes[i] += c;
            fileInput.get(c);
        }
        i++;
        fileInput.get(c);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VM::run(string filename)
{
    this->nCode = countNCode(filename);
    if (this->nCode == 0)
    {
        InvalidInstruction e = InvalidInstruction(0);
        throw e;
    }
    else
    {
        this->codes = new string[this->nCode];
        for (int i = 0; i < this->nCode; i++)
        {
            this->codes[i] = "\0";
        }

        readCode(filename);
        int numlines = this->nCode;
        Instruction *instructions = new Instruction[numlines];
        for (int i = 0; i < numlines; i++)
        {
            instructions[i] = instructions[i].getElementInstruction(this->codes[i]);
        }
        VM *newVM = new VM(instructions, 0, numlines);
        newVM->Register = new DataStorage[15];
        newVM->staticMemory = new DataStorage[65536];
        newVM->stack = new int[1000];
        newVM->cpu();
        delete[] instructions;
        delete newVM;
    }
}
void VM::cpu()
{
    this->Register[0].setDataInt(5);
    this->Register[1].setDataFloat(6.4);
    int lengthOfCode = this->nCode;
    while (this->ip < lengthOfCode)
    {
        Instruction temp = this->instr[ip];
        string opcode = temp.getNameOpcode();
        this->ip++;

        if (opcode == "Add" || opcode == "Minus" || opcode == "Mul" || opcode == "Div")
        {
            if (temp.getNOperands() == 2)
            {
                string op1 = temp.getOp1();
                int sizeOp1 = op1.length();
                int index1 = checkRegister(op1, sizeOp1); //return R index;
                if (index1 >= 1 && index1 <= 15)
                {
                    index1 = index1 - 1;
                    string op2 = temp.getOp2();
                    if (op2[0] == ' ')
                    {
                        op2 = eraseCharAtIndex(op2, 0);
                        int sizeOp2 = op2.length();
                        bool check = checkSpace(op2);
                        if (check == true)
                        {
                            int address = this->ip - 1;
                            InvalidInstruction e = InvalidInstruction(address);
                            throw e;
                            break;
                        }
                        else
                        {
                            if (op2[0] == 'R')
                            {
                                int index2 = checkRegister(op2, sizeOp2); //return R index;
                                if (index2 >= 1 && index2 <= 15)
                                {
                                    index2 = index2 - 1;
                                    int check1 = this->Register[index1].getTypeData();
                                    int check2 = this->Register[index2].getTypeData();
                                    if (opcode == "Add")
                                    {
                                        if (check1 == 1 && check2 == 1)
                                        {
                                            int data = this->Register[index2].getDataInt();
                                            this->Register[index1].setDataInt(this->Register[index1].getDataInt() + data);
                                        }
                                        else if (check1 == 1 && check2 == 2)
                                        {
                                            double data = this->Register[index2].getDataFloat();
                                            this->Register[index1].setDataFloat(this->Register[index1].getDataInt() + data);
                                        }
                                        else if (check1 == 2 && check2 == 1)
                                        {
                                            int data = this->Register[index2].getDataInt();
                                            this->Register[index1].setDataFloat(this->Register[index1].getDataFloat() + data);
                                        }
                                        else if (check1 == 2 && check2 == 2)
                                        {
                                            double data = this->Register[index2].getDataFloat();
                                            this->Register[index1].setDataFloat(this->Register[index1].getDataFloat() + data);
                                        }
                                        else if (check2 == 0)
                                        {

                                            int address = this->ip - 1;
                                            InvalidOperand e = InvalidOperand(address);
                                            throw e;
                                            break;
                                        }
                                        else
                                        {
                                            int address = this->ip - 1;
                                            InvalidOperand e = InvalidOperand(address);
                                            throw e;
                                            break;
                                        }
                                    }
                                    else if (opcode == "Minus")
                                    {
                                        if (check1 == 1 && check2 == 1)
                                        {
                                            int data = this->Register[index2].getDataInt();
                                            this->Register[index1].setDataInt(this->Register[index1].getDataInt() - data);
                                        }
                                        else if (check1 == 1 && check2 == 2)
                                        {
                                            double data = this->Register[index2].getDataFloat();
                                            this->Register[index1].setDataFloat(this->Register[index1].getDataInt() - data);
                                        }
                                        else if (check1 == 2 && check2 == 1)
                                        {
                                            int data = this->Register[index2].getDataInt();
                                            this->Register[index1].setDataFloat(this->Register[index1].getDataFloat() - data);
                                        }
                                        else if (check1 == 2 && check2 == 2)
                                        {
                                            double data = this->Register[index2].getDataFloat();
                                            this->Register[index1].setDataFloat(this->Register[index1].getDataFloat() - data);
                                        }
                                        else if (check2 == 0)
                                        {

                                            int address = this->ip - 1;
                                            InvalidOperand e = InvalidOperand(address);
                                            throw e;
                                            break;
                                        }
                                        else
                                        {
                                            int address = this->ip - 1;
                                            InvalidOperand e = InvalidOperand(address);
                                            throw e;
                                            break;
                                        }
                                    }
                                    else if (opcode == "Mul")
                                    {
                                        if (check1 == 1 && check2 == 1)
                                        {
                                            int data = this->Register[index2].getDataInt();
                                            this->Register[index1].setDataInt(this->Register[index1].getDataInt() * data);
                                        }
                                        else if (check1 == 1 && check2 == 2)
                                        {
                                            double data = this->Register[index2].getDataFloat();
                                            this->Register[index1].setDataFloat(this->Register[index1].getDataInt() * data);
                                        }
                                        else if (check1 == 2 && check2 == 1)
                                        {
                                            int data = this->Register[index2].getDataInt();
                                            this->Register[index1].setDataFloat(this->Register[index1].getDataFloat() * data);
                                        }
                                        else if (check1 == 2 && check2 == 2)
                                        {
                                            double data = this->Register[index2].getDataFloat();
                                            this->Register[index1].setDataFloat(this->Register[index1].getDataFloat() * data);
                                        }
                                        else if (check2 == 0)
                                        {

                                            int address = this->ip - 1;
                                            InvalidOperand e = InvalidOperand(address);
                                            throw e;
                                            break;
                                        }
                                        else
                                        {
                                            int address = this->ip - 1;
                                            InvalidOperand e = InvalidOperand(address);
                                            throw e;
                                            break;
                                        }
                                    }
                                    else if (opcode == "Div")
                                    {

                                        if (check1 == 1 && check2 == 1)
                                        {
                                            if (this->Register[index2].getDataInt() == 0)
                                            {
                                                int address = this->ip - 1;
                                                DivideByZero e = DivideByZero(address);
                                                throw e;
                                                break;
                                            }
                                            else
                                            {
                                                int data = this->Register[index2].getDataInt();
                                                this->Register[index1].setDataInt(this->Register[index1].getDataInt() / data);
                                            }
                                        }
                                        else if (check1 == 1 && check2 == 2)
                                        {
                                            if (this->Register[index2].getDataFloat() == 0)
                                            {
                                                int address = this->ip - 1;
                                                DivideByZero e = DivideByZero(address);
                                                throw e;
                                                break;
                                            }
                                            else
                                            {
                                                double data = this->Register[index2].getDataFloat();
                                                this->Register[index1].setDataFloat(this->Register[index1].getDataInt() / data);
                                            }
                                        }
                                        else if (check1 == 2 && check2 == 1)
                                        {
                                            if (this->Register[index2].getDataInt() == 0)
                                            {
                                                int address = this->ip - 1;
                                                DivideByZero e = DivideByZero(address);
                                                throw e;
                                                break;
                                            }
                                            else
                                            {
                                                int data = this->Register[index2].getDataInt();
                                                this->Register[index1].setDataFloat(this->Register[index1].getDataFloat() / data);
                                            }
                                        }
                                        else if (check1 == 2 && check2 == 2)
                                        {
                                            if (this->Register[index2].getDataFloat() == 0)
                                            {
                                                int address = this->ip - 1;
                                                DivideByZero e = DivideByZero(address);
                                                throw e;
                                                break;
                                            }
                                            else
                                            {
                                                double data = this->Register[index2].getDataFloat();
                                                this->Register[index1].setDataFloat(this->Register[index1].getDataFloat() / data);
                                            }
                                        }
                                        else if (check2 == 0)
                                        {

                                            int address = this->ip - 1;
                                            InvalidOperand e = InvalidOperand(address);
                                            throw e;
                                            break;
                                        }

                                        else
                                        {
                                            int address = this->ip - 1;
                                            InvalidOperand e = InvalidOperand(address);
                                            throw e;
                                            break;
                                        }
                                    }
                                }
                                else
                                {
                                    int address = this->ip - 1;
                                    InvalidOperand e = InvalidOperand(address);
                                    throw e;
                                    break;
                                }
                            }
                            else if (op2[0] != 'R')
                            {
                                DataStorage value;
                                int checkLiteral = checkOperand2(op2, value);
                                int checkTypeR = this->Register[index1].getTypeData();
                                if (opcode == "Add")
                                {
                                    if (checkTypeR == 1 && checkLiteral == 1)
                                    {
                                        int data = value.getDataInt();
                                        this->Register[index1].setDataInt(value.getDataInt() + data);
                                    }
                                    else if (checkTypeR == 1 && checkLiteral == 2)
                                    {
                                        double data = value.getDataFloat();
                                        this->Register[index1].setDataFloat(value.getDataInt() + data);
                                    }
                                    else if (checkTypeR == 2 && checkLiteral == 1)
                                    {
                                        int data = value.getDataInt();
                                        this->Register[index1].setDataFloat(value.getDataFloat() + data);
                                    }
                                    else if (checkTypeR == 2 && checkLiteral == 2)
                                    {
                                        double data = value.getDataFloat();
                                        this->Register[index1].setDataFloat(value.getDataFloat() + data);
                                    }
                                    else if (checkLiteral == 0)
                                    {

                                        int address = this->ip - 1;
                                        InvalidOperand e = InvalidOperand(address);
                                        throw e;
                                        break;
                                    }

                                    else
                                    {
                                        int address = this->ip - 1;
                                        InvalidOperand e = InvalidOperand(address);
                                        throw e;
                                        break;
                                    }
                                }
                                else if (opcode == "Minus")
                                {
                                    if (checkTypeR == 1 && checkLiteral == 1)
                                    {
                                        int data = value.getDataInt();
                                        this->Register[index1].setDataInt(value.getDataInt() - data);
                                    }
                                    else if (checkTypeR == 1 && checkLiteral == 2)
                                    {
                                        double data = value.getDataFloat();
                                        this->Register[index1].setDataFloat(value.getDataInt() - data);
                                    }
                                    else if (checkTypeR == 2 && checkLiteral == 1)
                                    {
                                        int data = value.getDataInt();
                                        this->Register[index1].setDataFloat(value.getDataFloat() - data);
                                    }
                                    else if (checkTypeR == 2 && checkLiteral == 2)
                                    {
                                        double data = value.getDataFloat();
                                        this->Register[index1].setDataFloat(value.getDataFloat() - data);
                                    }
                                    else if (checkLiteral == 0)
                                    {

                                        int address = this->ip - 1;
                                        InvalidOperand e = InvalidOperand(address);
                                        throw e;
                                        break;
                                    }

                                    else
                                    {
                                        int address = this->ip - 1;
                                        InvalidOperand e = InvalidOperand(address);
                                        throw e;
                                        break;
                                    }
                                }
                                else if (opcode == "Mul")
                                {
                                    if (checkTypeR == 1 && checkLiteral == 1)
                                    {
                                        int data = value.getDataInt();
                                        this->Register[index1].setDataInt(value.getDataInt() * data);
                                    }
                                    else if (checkTypeR == 1 && checkLiteral == 2)
                                    {
                                        double data = value.getDataFloat();
                                        this->Register[index1].setDataFloat(value.getDataInt() * data);
                                    }
                                    else if (checkTypeR == 2 && checkLiteral == 1)
                                    {
                                        int data = value.getDataInt();
                                        this->Register[index1].setDataFloat(value.getDataFloat() * data);
                                    }
                                    else if (checkTypeR == 2 && checkLiteral == 2)
                                    {
                                        double data = value.getDataFloat();
                                        this->Register[index1].setDataFloat(value.getDataFloat() * data);
                                    }
                                    else if (checkLiteral == 0)
                                    {

                                        int address = this->ip - 1;
                                        InvalidOperand e = InvalidOperand(address);
                                        throw e;
                                        break;
                                    }
                                    else
                                    {
                                        int address = this->ip - 1;
                                        InvalidOperand e = InvalidOperand(address);
                                        throw e;
                                        break;
                                    }
                                }
                                else if (opcode == "Div")
                                {
                                    if (checkTypeR == 1 && checkLiteral == 1)
                                    {
                                        if (value.getDataInt() == 0)
                                        {
                                            int address = this->ip - 1;
                                            DivideByZero e = DivideByZero(address);
                                            throw e;
                                            break;
                                        }
                                        else
                                        {
                                            int data = value.getDataInt();
                                            this->Register[index1].setDataInt(value.getDataInt() / data);
                                        }
                                    }
                                    else if (checkTypeR == 1 && checkLiteral == 2)
                                    {
                                        if (value.getDataFloat() == 0)
                                        {
                                            int address = this->ip - 1;
                                            DivideByZero e = DivideByZero(address);
                                            throw e;
                                            break;
                                        }
                                        else
                                        {
                                            double data = value.getDataFloat();
                                            this->Register[index1].setDataFloat(value.getDataInt() / data);
                                        }
                                    }
                                    else if (checkTypeR == 2 && checkLiteral == 1)
                                    {
                                        if (value.getDataInt() == 0)
                                        {
                                            int address = this->ip - 1;
                                            DivideByZero e = DivideByZero(address);
                                            throw e;
                                            break;
                                        }
                                        else
                                        {
                                            int data = value.getDataInt();
                                            this->Register[index1].setDataFloat(value.getDataFloat() / data);
                                        }
                                    }
                                    else if (checkTypeR == 2 && checkLiteral == 2)
                                    {
                                        if (value.getDataFloat() == 0)
                                        {
                                            int address = this->ip - 1;
                                            DivideByZero e = DivideByZero(address);
                                            throw e;
                                            break;
                                        }
                                        else
                                        {
                                            double data = value.getDataFloat();
                                            this->Register[index1].setDataFloat(value.getDataFloat() / data);
                                        }
                                    }
                                    else if (checkLiteral == 0)
                                    {

                                        int address = this->ip - 1;
                                        InvalidOperand e = InvalidOperand(address);
                                        throw e;
                                        break;
                                    }

                                    else
                                    {
                                        int address = this->ip - 1;
                                        InvalidOperand e = InvalidOperand(address);
                                        throw e;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        int address = this->ip - 1;
                        InvalidOperand e = InvalidOperand(address);
                        throw e;
                        break;
                    }
                }
                else
                {
                    int address = this->ip - 1;
                    InvalidOperand e = InvalidOperand(address);
                    throw e;
                    break;
                }
            }
            else
            {
                int address = this->ip - 1;
                InvalidInstruction e = InvalidInstruction(address);
                throw e;
                break;
            }
        }
        else if (opcode == "CmpEQ" || opcode == "CmpNE" || opcode == "CmpLT" || opcode == "CmpLE" || opcode == "CmpGT" || opcode == "CmpGE")
        {
            if (temp.getNOperands() == 2)
            {
                string op1 = temp.getOp1();
                int sizeOp1 = op1.length();
                int index1 = checkRegister(op1, sizeOp1); //return R index;
                if (index1 >= 1 && index1 <= 15)
                {
                    index1 = index1 - 1;
                    string op2 = temp.getOp2();
                    if (op2[0] == ' ')
                    {
                        op2 = eraseCharAtIndex(op2, 0);
                        int sizeOp2 = op2.length();
                        bool check = checkSpace(op2);
                        if (check == true)
                        {
                            int address = this->ip - 1;
                            InvalidInstruction e = InvalidInstruction(address);
                            throw e;
                            break;
                        }
                        else
                        {
                            if (op2[0] == 'R')
                            {
                                int index2 = checkRegister(op2, sizeOp2); //return R index;
                                if (index2 >= 1 && index2 <= 15)
                                {
                                    index2 = index2 - 1;
                                    int check1 = this->Register[index1].getTypeData();
                                    int check2 = this->Register[index2].getTypeData();
                                    if (opcode == "CmpEQ")
                                    {
                                        if (check1 == 3 && check2 == 3)
                                        {
                                            if (this->Register[index1].getDataBool() == this->Register[index2].getDataBool())
                                            {
                                                this->Register[index1].setDataBool(true);
                                            }
                                            else
                                            {
                                                this->Register[index1].setDataBool(false);
                                            }
                                        }
                                        else if (check1 == 1 && check2 == 1)
                                        {
                                            if (this->Register[index1].getDataInt() == this->Register[index2].getDataInt())
                                            {
                                                this->Register[index1].setDataBool(true);
                                            }
                                            else
                                            {
                                                this->Register[index1].setDataBool(false);
                                            }
                                        }
                                        else if (check1 == 1 && check2 == 2)
                                        {
                                            if ((double)(this->Register[index1].getDataInt()) == this->Register[index2].getDataFloat())
                                            {
                                                this->Register[index1].setDataBool(true);
                                            }
                                            else
                                            {
                                                this->Register[index1].setDataBool(false);
                                            }
                                        }
                                        else if (check1 == 2 && check2 == 1)
                                        {
                                            if (this->Register[index1].getDataFloat() == (double)(this->Register[index2].getDataInt()))
                                            {
                                                this->Register[index1].setDataBool(true);
                                            }
                                            else
                                            {
                                                this->Register[index1].setDataBool(false);
                                            }
                                        }
                                        else if (check1 == 2 && check2 == 2)
                                        {
                                            if (this->Register[index1].getDataFloat() == (this->Register[index2].getDataFloat()))
                                            {
                                                this->Register[index1].setDataBool(true);
                                            }
                                            else
                                            {
                                                this->Register[index1].setDataBool(false);
                                            }
                                        }
                                        else if (check2 == 0)
                                        {

                                            int address = this->ip - 1;
                                            InvalidOperand e = InvalidOperand(address);
                                            throw e;
                                            break;
                                        }
                                        else
                                        {
                                            int address = this->ip - 1;
                                            InvalidOperand e = InvalidOperand(address);
                                            throw e;
                                            break;
                                        }
                                    }
                                    else if (opcode == "CmpNE")
                                    {
                                        if (check1 == 3 && check2 == 3)
                                        {
                                            if (this->Register[index1].getDataBool() != this->Register[index2].getDataBool())
                                            {
                                                this->Register[index1].setDataBool(true);
                                            }
                                            else
                                            {
                                                this->Register[index1].setDataBool(false);
                                            }
                                        }
                                        else if (check1 == 1 && check2 == 1)
                                        {
                                            if (this->Register[index1].getDataInt() != this->Register[index2].getDataInt())
                                            {
                                                this->Register[index1].setDataBool(true);
                                            }
                                            else
                                            {
                                                this->Register[index1].setDataBool(false);
                                            }
                                        }
                                        else if (check1 == 1 && check2 == 2)
                                        {
                                            if ((double)(this->Register[index1].getDataInt()) != this->Register[index2].getDataFloat())
                                            {
                                                this->Register[index1].setDataBool(true);
                                            }
                                            else
                                            {
                                                this->Register[index1].setDataBool(false);
                                            }
                                        }
                                        else if (check1 == 2 && check2 == 1)
                                        {
                                            if (this->Register[index1].getDataFloat() != (double)(this->Register[index2].getDataInt()))
                                            {
                                                this->Register[index1].setDataBool(true);
                                            }
                                            else
                                            {
                                                this->Register[index1].setDataBool(false);
                                            }
                                        }
                                        else if (check1 == 2 && check2 == 2)
                                        {
                                            if (this->Register[index1].getDataFloat() != (this->Register[index2].getDataFloat()))
                                            {
                                                this->Register[index1].setDataBool(true);
                                            }
                                            else
                                            {
                                                this->Register[index1].setDataBool(false);
                                            }
                                        }
                                        else if (check2 == 0)
                                        {

                                            int address = this->ip - 1;
                                            InvalidOperand e = InvalidOperand(address);
                                            throw e;
                                            break;
                                        }
                                        else
                                        {
                                            int address = this->ip - 1;
                                            TypeMismatch e = TypeMismatch(address);
                                            throw e;
                                        }
                                    }
                                    else if (opcode == "CmpLT")
                                    {
                                        if (check1 == 1 && check2 == 1)
                                        {
                                            if (this->Register[index1].getDataInt() < this->Register[index2].getDataInt())
                                            {
                                                this->Register[index1].setDataBool(true);
                                            }
                                            else
                                            {
                                                this->Register[index1].setDataBool(false);
                                            }
                                        }
                                        else if (check1 == 1 && check2 == 2)
                                        {
                                            if ((double)(this->Register[index1].getDataInt()) < this->Register[index2].getDataFloat())
                                            {
                                                this->Register[index1].setDataBool(true);
                                            }
                                            else
                                            {
                                                this->Register[index1].setDataBool(false);
                                            }
                                        }
                                        else if (check1 == 2 && check2 == 1)
                                        {
                                            if (this->Register[index1].getDataFloat() < (double)(this->Register[index2].getDataInt()))
                                            {
                                                this->Register[index1].setDataBool(true);
                                            }
                                            else
                                            {
                                                this->Register[index1].setDataBool(false);
                                            }
                                        }
                                        else if (check1 == 2 && check2 == 2)
                                        {
                                            if (this->Register[index1].getDataFloat() < (this->Register[index2].getDataFloat()))
                                            {
                                                this->Register[index1].setDataBool(true);
                                            }
                                            else
                                            {
                                                this->Register[index1].setDataBool(false);
                                            }
                                        }
                                        else if (check2 == 0)
                                        {

                                            int address = this->ip - 1;
                                            InvalidOperand e = InvalidOperand(address);
                                            throw e;
                                            break;
                                        }
                                        else
                                        {
                                            int address = this->ip - 1;
                                            TypeMismatch e = TypeMismatch(address);
                                            throw e;
                                        }
                                    }
                                    else if (opcode == "CmpLE")
                                    {
                                        if (check1 == 1 && check2 == 1)
                                        {
                                            if (this->Register[index1].getDataInt() <= this->Register[index2].getDataInt())
                                            {
                                                this->Register[index1].setDataBool(true);
                                            }
                                            else
                                            {
                                                this->Register[index1].setDataBool(false);
                                            }
                                        }
                                        else if (check1 == 1 && check2 == 2)
                                        {
                                            if ((double)(this->Register[index1].getDataInt()) <= this->Register[index2].getDataFloat())
                                            {
                                                this->Register[index1].setDataBool(true);
                                            }
                                            else
                                            {
                                                this->Register[index1].setDataBool(false);
                                            }
                                        }
                                        else if (check1 == 2 && check2 == 1)
                                        {
                                            if (this->Register[index1].getDataFloat() <= (double)(this->Register[index2].getDataInt()))
                                            {
                                                this->Register[index1].setDataBool(true);
                                            }
                                            else
                                            {
                                                this->Register[index1].setDataBool(false);
                                            }
                                        }
                                        else if (check1 == 2 && check2 == 2)
                                        {
                                            if (this->Register[index1].getDataFloat() <= (this->Register[index2].getDataFloat()))
                                            {
                                                this->Register[index1].setDataBool(true);
                                            }
                                            else
                                            {
                                                this->Register[index1].setDataBool(false);
                                            }
                                        }
                                        else if (check2 == 0)
                                        {

                                            int address = this->ip - 1;
                                            InvalidOperand e = InvalidOperand(address);
                                            throw e;
                                            break;
                                        }

                                        else
                                        {
                                            int address = this->ip - 1;
                                            TypeMismatch e = TypeMismatch(address);
                                            throw e;
                                        }
                                    }
                                    else if (opcode == "CmpGT")
                                    {
                                        if (check1 == 1 && check2 == 1)
                                        {
                                            if (this->Register[index1].getDataInt() > this->Register[index2].getDataInt())
                                            {
                                                this->Register[index1].setDataBool(true);
                                            }
                                            else
                                            {
                                                this->Register[index1].setDataBool(false);
                                            }
                                        }
                                        else if (check1 == 1 && check2 == 2)
                                        {
                                            if ((double)(this->Register[index1].getDataInt()) > this->Register[index2].getDataFloat())
                                            {
                                                this->Register[index1].setDataBool(true);
                                            }
                                            else
                                            {
                                                this->Register[index1].setDataBool(false);
                                            }
                                        }
                                        else if (check1 == 2 && check2 == 1)
                                        {
                                            if (this->Register[index1].getDataFloat() > (double)(this->Register[index2].getDataInt()))
                                            {
                                                this->Register[index1].setDataBool(true);
                                            }
                                            else
                                            {
                                                this->Register[index1].setDataBool(false);
                                            }
                                        }
                                        else if (check1 == 2 && check2 == 2)
                                        {
                                            if (this->Register[index1].getDataFloat() > (this->Register[index2].getDataFloat()))
                                            {
                                                this->Register[index1].setDataBool(true);
                                            }
                                            else
                                            {
                                                this->Register[index1].setDataBool(false);
                                            }
                                        }
                                        else if (check2 == 0)
                                        {

                                            int address = this->ip - 1;
                                            InvalidOperand e = InvalidOperand(address);
                                            throw e;
                                            break;
                                        }

                                        else
                                        {
                                            int address = this->ip - 1;
                                            TypeMismatch e = TypeMismatch(address);
                                            throw e;
                                        }
                                    }
                                    else if (opcode == "CmpGE")
                                    {
                                        if (check1 == 1 && check2 == 1)
                                        {
                                            if (this->Register[index1].getDataInt() >= this->Register[index2].getDataInt())
                                            {
                                                this->Register[index1].setDataBool(true);
                                            }
                                            else
                                            {
                                                this->Register[index1].setDataBool(false);
                                            }
                                        }
                                        else if (check1 == 1 && check2 == 2)
                                        {
                                            if ((double)(this->Register[index1].getDataInt()) >= this->Register[index2].getDataFloat())
                                            {
                                                this->Register[index1].setDataBool(true);
                                            }
                                            else
                                            {
                                                this->Register[index1].setDataBool(false);
                                            }
                                        }
                                        else if (check1 == 2 && check2 == 1)
                                        {
                                            if (this->Register[index1].getDataFloat() >= (double)(this->Register[index2].getDataInt()))
                                            {
                                                this->Register[index1].setDataBool(true);
                                            }
                                            else
                                            {
                                                this->Register[index1].setDataBool(false);
                                            }
                                        }
                                        else if (check1 == 2 && check2 == 2)
                                        {
                                            if (this->Register[index1].getDataFloat() >= (this->Register[index2].getDataFloat()))
                                            {
                                                this->Register[index1].setDataBool(true);
                                            }
                                            else
                                            {
                                                this->Register[index1].setDataBool(false);
                                            }
                                        }
                                        else if (check2 == 0)
                                        {

                                            int address = this->ip - 1;
                                            InvalidOperand e = InvalidOperand(address);
                                            throw e;
                                            break;
                                        }
                                        else
                                        {
                                            int address = this->ip - 1;
                                            TypeMismatch e = TypeMismatch(address);
                                            throw e;
                                        }
                                    }
                                }
                                else
                                {
                                    int address = this->ip - 1;
                                    InvalidOperand e = InvalidOperand(address);
                                    throw e;
                                    break;
                                }
                            }
                            else if (op2[0] != 'R')
                            {
                                DataStorage value;
                                int checkLiteral = checkOperand2(op2, value);
                                int checkTypeR = this->Register[index1].getTypeData();
                                if (opcode == "CmpEQ")
                                {
                                    if (checkTypeR == 3 && checkLiteral == 3)
                                    {
                                        if (this->Register[index1].getDataBool() == value.getDataBool())
                                        {
                                            this->Register[index1].setDataBool(true);
                                        }
                                        else
                                        {
                                            this->Register[index1].setDataBool(false);
                                        }
                                    }
                                    else if (checkTypeR == 1 && checkLiteral == 1)
                                    {
                                        if (this->Register[index1].getDataInt() == value.getDataInt())
                                        {
                                            this->Register[index1].setDataBool(true);
                                        }
                                        else
                                        {
                                            this->Register[index1].setDataBool(false);
                                        }
                                    }
                                    else if (checkTypeR == 1 && checkLiteral == 2)
                                    {
                                        if ((double)(this->Register[index1].getDataInt()) == value.getDataFloat())
                                        {
                                            this->Register[index1].setDataBool(true);
                                        }
                                        else
                                        {
                                            this->Register[index1].setDataBool(false);
                                        }
                                    }
                                    else if (checkTypeR == 2 && checkLiteral == 1)
                                    {
                                        if (this->Register[index1].getDataFloat() == (double)(value.getDataInt()))
                                        {
                                            this->Register[index1].setDataBool(true);
                                        }
                                        else
                                        {
                                            this->Register[index1].setDataBool(false);
                                        }
                                    }
                                    else if (checkTypeR == 2 && checkLiteral == 2)
                                    {
                                        if (this->Register[index1].getDataFloat() == (value.getDataFloat()))
                                        {
                                            this->Register[index1].setDataBool(true);
                                        }
                                        else
                                        {
                                            this->Register[index1].setDataBool(false);
                                        }
                                    }
                                    else if (checkLiteral == 0)
                                    {

                                        int address = this->ip - 1;
                                        InvalidOperand e = InvalidOperand(address);
                                        throw e;
                                        break;
                                    }
                                    else
                                    {
                                        int address = this->ip - 1;
                                        TypeMismatch e = TypeMismatch(address);
                                        throw e;
                                    }
                                }
                                else if (opcode == "CmpNE")
                                {
                                    if (checkTypeR == 3 && checkLiteral == 3)
                                    {
                                        if (this->Register[index1].getDataBool() == value.getDataBool())
                                        {
                                            this->Register[index1].setDataBool(true);
                                        }
                                        else
                                        {
                                            this->Register[index1].setDataBool(false);
                                        }
                                    }
                                    else if (checkTypeR == 1 && checkLiteral == 1)
                                    {
                                        if (this->Register[index1].getDataInt() != value.getDataInt())
                                        {
                                            this->Register[index1].setDataBool(true);
                                        }
                                        else
                                        {
                                            this->Register[index1].setDataBool(false);
                                        }
                                    }
                                    else if (checkTypeR == 1 && checkLiteral == 2)
                                    {
                                        if ((double)(this->Register[index1].getDataInt()) != value.getDataFloat())
                                        {
                                            this->Register[index1].setDataBool(true);
                                        }
                                        else
                                        {
                                            this->Register[index1].setDataBool(false);
                                        }
                                    }
                                    else if (checkTypeR == 2 && checkLiteral == 1)
                                    {
                                        if (this->Register[index1].getDataFloat() != (double)(value.getDataInt()))
                                        {
                                            this->Register[index1].setDataBool(true);
                                        }
                                        else
                                        {
                                            this->Register[index1].setDataBool(false);
                                        }
                                    }
                                    else if (checkTypeR == 2 && checkLiteral == 2)
                                    {
                                        if (this->Register[index1].getDataFloat() != (value.getDataFloat()))
                                        {
                                            this->Register[index1].setDataBool(true);
                                        }
                                        else
                                        {
                                            this->Register[index1].setDataBool(false);
                                        }
                                    }
                                    else if (checkLiteral == 0)
                                    {

                                        int address = this->ip - 1;
                                        InvalidOperand e = InvalidOperand(address);
                                        throw e;
                                        break;
                                    }
                                    else
                                    {
                                        int address = this->ip - 1;
                                        TypeMismatch e = TypeMismatch(address);
                                        throw e;
                                    }
                                }
                                else if (opcode == "CmpLT")
                                {
                                    if (checkTypeR == 1 && checkLiteral == 1)
                                    {
                                        if (this->Register[index1].getDataInt() < value.getDataInt())
                                        {
                                            this->Register[index1].setDataBool(true);
                                        }
                                        else
                                        {
                                            this->Register[index1].setDataBool(false);
                                        }
                                    }
                                    else if (checkTypeR == 1 && checkLiteral == 2)
                                    {
                                        if ((double)(this->Register[index1].getDataInt()) < value.getDataFloat())
                                        {
                                            this->Register[index1].setDataBool(true);
                                        }
                                        else
                                        {
                                            this->Register[index1].setDataBool(false);
                                        }
                                    }
                                    else if (checkTypeR == 2 && checkLiteral == 1)
                                    {
                                        if (this->Register[index1].getDataFloat() < (double)(value.getDataInt()))
                                        {
                                            this->Register[index1].setDataBool(true);
                                        }
                                        else
                                        {
                                            this->Register[index1].setDataBool(false);
                                        }
                                    }
                                    else if (checkTypeR == 2 && checkLiteral == 2)
                                    {
                                        if (this->Register[index1].getDataFloat() < (value.getDataFloat()))
                                        {
                                            this->Register[index1].setDataBool(true);
                                        }
                                        else
                                        {
                                            this->Register[index1].setDataBool(false);
                                        }
                                    }
                                    else if (checkLiteral == 0)
                                    {

                                        int address = this->ip - 1;
                                        InvalidOperand e = InvalidOperand(address);
                                        throw e;
                                        break;
                                    }
                                    else
                                    {
                                        int address = this->ip - 1;
                                        TypeMismatch e = TypeMismatch(address);
                                        throw e;
                                    }
                                }
                                else if (opcode == "CmpLE")
                                {
                                    if (checkTypeR == 1 && checkLiteral == 1)
                                    {
                                        if (this->Register[index1].getDataInt() <= value.getDataInt())
                                        {
                                            this->Register[index1].setDataBool(true);
                                        }
                                        else
                                        {
                                            this->Register[index1].setDataBool(false);
                                        }
                                    }
                                    else if (checkTypeR == 1 && checkLiteral == 2)
                                    {
                                        if ((double)(this->Register[index1].getDataInt()) <= value.getDataFloat())
                                        {
                                            this->Register[index1].setDataBool(true);
                                        }
                                        else
                                        {
                                            this->Register[index1].setDataBool(false);
                                        }
                                    }
                                    else if (checkTypeR == 2 && checkLiteral == 1)
                                    {
                                        if (this->Register[index1].getDataFloat() <= (double)(value.getDataInt()))
                                        {
                                            this->Register[index1].setDataBool(true);
                                        }
                                        else
                                        {
                                            this->Register[index1].setDataBool(false);
                                        }
                                    }
                                    else if (checkTypeR == 2 && checkLiteral == 2)
                                    {
                                        if (this->Register[index1].getDataFloat() <= (value.getDataFloat()))
                                        {
                                            this->Register[index1].setDataBool(true);
                                        }
                                        else
                                        {
                                            this->Register[index1].setDataBool(false);
                                        }
                                    }
                                    else if (checkLiteral == 0)
                                    {

                                        int address = this->ip - 1;
                                        InvalidOperand e = InvalidOperand(address);
                                        throw e;
                                        break;
                                    }
                                    else
                                    {
                                        int address = this->ip - 1;
                                        TypeMismatch e = TypeMismatch(address);
                                        throw e;
                                    }
                                }
                                else if (opcode == "CmpGT")
                                {
                                    if (checkTypeR == 1 && checkLiteral == 1)
                                    {
                                        if (this->Register[index1].getDataInt() > value.getDataInt())
                                        {
                                            this->Register[index1].setDataBool(true);
                                        }
                                        else
                                        {
                                            this->Register[index1].setDataBool(false);
                                        }
                                    }
                                    else if (checkTypeR == 1 && checkLiteral == 2)
                                    {
                                        if ((double)(this->Register[index1].getDataInt()) > value.getDataFloat())
                                        {
                                            this->Register[index1].setDataBool(true);
                                        }
                                        else
                                        {
                                            this->Register[index1].setDataBool(false);
                                        }
                                    }
                                    else if (checkTypeR == 2 && checkLiteral == 1)
                                    {
                                        if (this->Register[index1].getDataFloat() > (double)(value.getDataInt()))
                                        {
                                            this->Register[index1].setDataBool(true);
                                        }
                                        else
                                        {
                                            this->Register[index1].setDataBool(false);
                                        }
                                    }
                                    else if (checkTypeR == 2 && checkLiteral == 2)
                                    {
                                        if (this->Register[index1].getDataFloat() > (value.getDataFloat()))
                                        {
                                            this->Register[index1].setDataBool(true);
                                        }
                                        else
                                        {
                                            this->Register[index1].setDataBool(false);
                                        }
                                    }
                                    else if (checkLiteral == 0)
                                    {

                                        int address = this->ip - 1;
                                        InvalidOperand e = InvalidOperand(address);
                                        throw e;
                                        break;
                                    }
                                    else
                                    {
                                        int address = this->ip - 1;
                                        TypeMismatch e = TypeMismatch(address);
                                        throw e;
                                    }
                                }
                                else if (opcode == "CmpGE")
                                {
                                    if (checkTypeR == 1 && checkLiteral == 1)
                                    {
                                        if (this->Register[index1].getDataInt() >= value.getDataInt())
                                        {
                                            this->Register[index1].setDataBool(true);
                                        }
                                        else
                                        {
                                            this->Register[index1].setDataBool(false);
                                        }
                                    }
                                    else if (checkTypeR == 1 && checkLiteral == 2)
                                    {
                                        if ((double)(this->Register[index1].getDataInt()) >= value.getDataFloat())
                                        {
                                            this->Register[index1].setDataBool(true);
                                        }
                                        else
                                        {
                                            this->Register[index1].setDataBool(false);
                                        }
                                    }
                                    else if (checkTypeR == 2 && checkLiteral == 1)
                                    {
                                        if (this->Register[index1].getDataFloat() >= (double)(value.getDataInt()))
                                        {
                                            this->Register[index1].setDataBool(true);
                                        }
                                        else
                                        {
                                            this->Register[index1].setDataBool(false);
                                        }
                                    }
                                    else if (checkTypeR == 2 && checkLiteral == 2)
                                    {
                                        if (this->Register[index1].getDataFloat() >= (value.getDataFloat()))
                                        {
                                            this->Register[index1].setDataBool(true);
                                        }
                                        else
                                        {
                                            this->Register[index1].setDataBool(false);
                                        }
                                    }
                                    else if (checkLiteral == 0)
                                    {

                                        int address = this->ip - 1;
                                        InvalidOperand e = InvalidOperand(address);
                                        throw e;
                                        break;
                                    }
                                    else
                                    {
                                        int address = this->ip - 1;
                                        TypeMismatch e = TypeMismatch(address);
                                        throw e;
                                    }
                                }
                            }
                        } //else if !R
                    }     //check space false
                }         //check Op2
                else
                {
                    int address = this->ip - 1;
                    InvalidOperand e = InvalidOperand(address);
                    throw e;
                    break;
                }
            }
            else

            {
                int address = this->ip - 1;
                InvalidInstruction e = InvalidInstruction(address);
                throw e;
                break;
            }
        }

        else if (opcode == "Not")
        {
            if (temp.getNOperands() == 1)
            {

                string op1 = temp.getOp1();
                int sizeOp1 = op1.length();
                int index1 = checkRegister(op1, sizeOp1); //return R index;
                if (index1 >= 1 && index1 <= 15)
                {
                    int check1 = this->Register[index1].getTypeData();
                    if (check1 == 3)
                    {
                        bool value = !(this->Register[index1].getDataBool());
                        this->Register[index1].setDataBool(value);
                        this->Register[index1].setTypeData(3);
                    }
                    else
                    {
                        int address = this->ip - 1;
                        TypeMismatch e = TypeMismatch(address);
                        throw e;
                        break;
                    }
                }

                else
                {
                    int address = this->ip - 1;
                    InvalidOperand e = InvalidOperand(address);
                    throw e;
                    break;
                }
            }
            else
            {
                int address = this->ip - 1;
                InvalidInstruction e = InvalidInstruction(address);
                throw e;
                break;
            }
        }
        else if (opcode == "And" || opcode == "Or")
        {
            if (temp.getNOperands() == 2)
            {
                string op1 = temp.getOp1();
                int sizeOp1 = op1.length();
                int index1 = checkRegister(op1, sizeOp1); //return R index;
                if (index1 >= 1 && index1 <= 15)
                {
                    index1 = index1 - 1;
                    string op2 = temp.getOp2();
                    if (op2[0] == ' ')
                    {
                        op2 = eraseCharAtIndex(op2, 0);
                        int sizeOp2 = op2.length();
                        bool check = checkSpace(op2);
                        if (check == true)
                        {
                            int address = this->ip - 1;
                            InvalidInstruction e = InvalidInstruction(address);
                            throw e;
                            break;
                        }
                        else
                        {
                            if (op2[0] == 'R')
                            {
                                int index2 = checkRegister(op2, sizeOp2); //return R index;
                                if (index2 >= 1 && index2 <= 15)
                                {
                                    index2 = index2 - 1;
                                    int check1 = this->Register[index1].getTypeData();
                                    int check2 = this->Register[index2].getTypeData();
                                    if (opcode == "And")
                                    {
                                        if (check1 == 3 && check2 == 3)
                                        {
                                            bool value = this->Register[index1].getDataBool() && this->Register[index2].getDataBool();
                                            this->Register[index1].setDataBool(value);
                                            this->Register[index1].setTypeData(3);
                                        }

                                        else
                                        {
                                            int address = this->ip - 1;
                                            TypeMismatch e = TypeMismatch(address);
                                            throw e;
                                        }
                                    }
                                    else if (opcode == "Or")
                                    {
                                        if (check1 == 3 && check2 == 3)
                                        {
                                            bool value = this->Register[index1].getDataBool() || this->Register[index2].getDataBool();
                                            this->Register[index1].setDataBool(value);
                                            this->Register[index1].setTypeData(3);
                                        }

                                        else
                                        {
                                            int address = this->ip - 1;
                                            TypeMismatch e = TypeMismatch(address);
                                            throw e;
                                        }
                                    }
                                }
                                else
                                {
                                    int address = this->ip - 1;
                                    InvalidOperand e = InvalidOperand(address);
                                    throw e;
                                    break;
                                }
                            }
                            else if (op2[0] != 'R')
                            {
                                DataStorage value;
                                int checkLiteral = checkOperand2(op2, value);
                                int checkTypeR = this->Register[index1].getTypeData();
                                if (opcode == "And")
                                {
                                    if (checkTypeR == 3 && checkLiteral == 3)
                                    {
                                        bool temp = this->Register[index1].getDataBool() && value.getDataBool();
                                        this->Register[index1].setDataBool(temp);
                                        this->Register[index1].setTypeData(3);
                                    }
                                    else if (checkTypeR == 0)
                                    {
                                        int address = this->ip - 1;
                                        InvalidOperand e = InvalidOperand(address);
                                        throw e;
                                        break;
                                    }

                                    else
                                    {
                                        int address = this->ip - 1;
                                        TypeMismatch e = TypeMismatch(address);
                                        throw e;
                                    }
                                }
                                else if (opcode == "Or")
                                {
                                    if (checkTypeR == 3 && checkLiteral == 3)
                                    {
                                        bool temp = this->Register[index1].getDataBool() || value.getDataBool();
                                        this->Register[index1].setDataBool(temp);
                                        this->Register[index1].setTypeData(3);
                                    }
                                    else if (checkTypeR == 0)
                                    {
                                        int address = this->ip - 1;
                                        InvalidOperand e = InvalidOperand(address);
                                        throw e;
                                        break;
                                    }

                                    else
                                    {
                                        int address = this->ip - 1;
                                        TypeMismatch e = TypeMismatch(address);
                                        throw e;
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        int address = this->ip - 1;
                        InvalidInstruction e = InvalidInstruction(address);
                        throw e;
                        break;
                    }
                }
                else
                {
                    int address = this->ip - 1;
                    InvalidInstruction e = InvalidInstruction(address);
                    throw e;
                    break;
                }
            }
            else
            {
                int address = this->ip - 1;
                InvalidOperand e = InvalidOperand(address);
                throw e;
                break;
            }
        }

        else if (opcode == "Move")
        {
            if (temp.getNOperands() == 2)
            {
                string op1 = temp.getOp1();
                int sizeOp1 = op1.length();
                int index1 = checkRegister(op1, sizeOp1); //return R index;
                if (index1 >= 1 && index1 <= 15)
                {
                    index1 = index1 - 1;
                    string op2 = temp.getOp2();
                    if (op2[0] == ' ')
                    {
                        op2 = eraseCharAtIndex(op2, 0);
                        int sizeOp2 = op2.length();
                        bool check = checkSpace(op2);
                        if (check == true)
                        {
                            int address = this->ip - 1;
                            InvalidInstruction e = InvalidInstruction(address);
                            throw e;
                            break;
                        }
                        else
                        {
                            if (op2[0] == 'R')
                            {
                                int index2 = checkRegister(op2, sizeOp2); //return R index;
                                if (index2 >= 1 && index2 <= 15)
                                {
                                    index2 = index2 - 1;
                                    int check2 = this->Register[index2].getTypeData();
                                    if (check2 == 1)
                                    {
                                        this->Register[index1].setDataInt(this->Register[index2].getDataInt());
                                    }
                                    else if (check2 == 2)
                                    {
                                        this->Register[index1].setDataFloat(this->Register[index2].getDataFloat());
                                    }
                                    else if (check2 == 3)
                                    {
                                        this->Register[index1].setDataBool(this->Register[index2].getDataBool());
                                    }
                                    else if (check2 == 4)
                                    {
                                        this->Register[index1].setAddress(this->Register[index2].getAddress());
                                    }
                                    else if (check2 == 0)
                                    {
                                        int address = this->ip - 1;
                                        InvalidOperand e = InvalidOperand(address);
                                        throw e;
                                        break;
                                    }
                                    else
                                    {
                                        int address = this->ip - 1;
                                        TypeMismatch e = TypeMismatch(address);
                                        throw e;
                                        break;
                                    }
                                }
                                else
                                {
                                    int address = this->ip - 1;
                                    InvalidOperand e = InvalidOperand(address);
                                    throw e;
                                    break;
                                }
                            }
                            else if (op2[0] != 'R')
                            {
                                DataStorage value;
                                int checkLiteral = checkOperand2(op2, value);
                                int checkTypeR = this->Register[index1].getTypeData();
                                if (checkLiteral == 1)
                                {
                                    this->Register[index1].setDataInt(value.getDataInt());
                                }
                                else if (checkLiteral == 2)
                                {
                                    this->Register[index1].setDataFloat(value.getDataFloat());
                                }
                                else if (checkLiteral == 3)
                                {
                                    this->Register[index1].setDataBool(value.getDataBool());
                                }
                                else if (checkLiteral == 4)
                                {
                                    this->Register[index1].setAddress(value.getAddress());
                                }
                                else if (checkLiteral == 0)
                                {
                                    int address = this->ip - 1;
                                    InvalidOperand e = InvalidOperand(address);
                                    throw e;
                                    break;
                                }
                                else
                                {
                                    int address = this->ip - 1;
                                    TypeMismatch e = TypeMismatch(address);
                                    throw e;
                                }
                            }
                        }
                    }
                    else
                    {
                        int address = this->ip - 1;
                        InvalidInstruction e = InvalidInstruction(address);
                        throw e;
                        break;
                    }
                }
                else
                {
                    int address = this->ip - 1;
                    InvalidOperand e = InvalidOperand(address);
                    throw e;
                    break;
                }
            }
            else
            {
                int address = this->ip - 1;
                InvalidInstruction e = InvalidInstruction(address);
                throw e;
                break;
            }
        }
        else if (opcode == "Load")
        {
            if (temp.getNOperands() == 2)
            {
                string op1 = temp.getOp1();
                int sizeOp1 = op1.length();
                int index1 = checkRegister(op1, sizeOp1); //return R index;
                if (index1 >= 1 && index1 <= 15)
                {
                    index1 = index1 - 1;
                    string op2 = temp.getOp2();
                    if (op2[0] == ' ')
                    {
                        op2 = eraseCharAtIndex(op2, 0);
                        int sizeOp2 = op2.length();
                        bool check = checkSpace(op2);
                        if (check == true)
                        {
                            int address = this->ip - 1;
                            InvalidInstruction e = InvalidInstruction(address);
                            throw e;
                            break;
                        }
                        else
                        {
                            if (op2[0] == 'R')
                            {
                                int index2 = checkRegister(op2, sizeOp2); //return R index;
                                if (index2 >= 1 && index2 <= 15)
                                {
                                    index2 = index2 - 1;
                                    int check2 = this->Register[index2].getTypeData();
                                    if (check2 == 4)
                                    {
                                        int address = this->Register[index2].getAddress();
                                        if (address > 65535 && address < 0)
                                        {
                                            int address = this->ip - 1;
                                            TypeMismatch e = TypeMismatch(address);
                                            throw e;
                                            break;
                                        }
                                        else
                                        {
                                            DataStorage value = this->staticMemory[address];
                                            int checkValueOfSrc = value.getTypeData();
                                            if (checkValueOfSrc == 1)
                                            {
                                                this->Register[index1].setDataInt(value.getDataInt());
                                            }
                                            else if (checkValueOfSrc == 2)
                                            {
                                                this->Register[index1].setDataFloat(value.getDataFloat());
                                            }
                                            else if (checkValueOfSrc == 3)
                                            {
                                                this->Register[index1].setDataBool(value.getDataBool());
                                            }
                                            else if (checkValueOfSrc == 4)
                                            {
                                                this->Register[index1].setAddress(value.getAddress());
                                            }
                                            else if (checkValueOfSrc == 0)
                                            {
                                                int address = this->ip - 1;
                                                InvalidOperand e = InvalidOperand(address);
                                                throw e;
                                                break;
                                            }

                                            else
                                            {
                                                int address = this->ip - 1;
                                                TypeMismatch e = TypeMismatch(address);
                                                throw e;
                                                break;
                                            }
                                        }
                                    }
                                    else
                                    {
                                        int address = this->ip - 1;
                                        TypeMismatch e = TypeMismatch(address);
                                        throw e;
                                        break;
                                    }
                                }
                                else
                                {
                                    int address = this->ip - 1;
                                    InvalidOperand e = InvalidOperand(address);
                                    throw e;
                                    break;
                                }
                            }
                            else if (op2[0] != 'R')
                            {
                                DataStorage value;
                                int checkLiteral = checkOperand2(op2, value);
                                int address = value.getAddress();
                                if (checkLiteral == 4)
                                {

                                    if (address > 65535 && address < 0)
                                    {
                                        int address = this->ip - 1;
                                        TypeMismatch e = TypeMismatch(address);
                                        throw e;
                                    }

                                    else
                                    {
                                        DataStorage temp = this->staticMemory[address];
                                        int checkValueOfSrc = temp.getTypeData();
                                        if (checkValueOfSrc == 1)
                                        {
                                            this->Register[index1].setDataInt(temp.getDataInt());
                                        }
                                        else if (checkValueOfSrc == 2)
                                        {
                                            this->Register[index1].setDataFloat(temp.getDataFloat());
                                        }
                                        else if (checkValueOfSrc == 3)
                                        {
                                            this->Register[index1].setDataBool(temp.getDataBool());
                                        }
                                        else if (checkValueOfSrc == 4)
                                        {
                                            this->Register[index1].setAddress(temp.getAddress());
                                        }
                                        else if (checkValueOfSrc == 0)
                                        {
                                            int address = this->ip - 1;
                                            InvalidOperand e = InvalidOperand(address);
                                            throw e;
                                            break;
                                        }

                                        else
                                        {
                                            int address = this->ip - 1;
                                            TypeMismatch e = TypeMismatch(address);
                                            throw e;
                                            break;
                                        }
                                    }
                                }
                                else if (checkLiteral == 0)
                                {
                                    int address = this->ip - 1;
                                    InvalidOperand e = InvalidOperand(address);
                                    throw e;
                                    break;
                                }
                                else
                                {
                                    int address = this->ip - 1;
                                    TypeMismatch e = TypeMismatch(address);
                                    throw e;
                                    break;
                                }
                            }
                        }
                    }
                    else
                    {
                        int address = this->ip - 1;
                        InvalidOperand e = InvalidOperand(address);
                        throw e;
                        break;
                    }
                }
                else
                {
                    int address = this->ip - 1;
                    InvalidOperand e = InvalidOperand(address);
                    throw e;
                    break;
                }
            }

            else
            {
                int address = this->ip - 1;
                InvalidInstruction e = InvalidInstruction(address);
                throw e;
                break;
            }
        }
        else if (opcode == "Store")
        {
            if (temp.getNOperands() == 2)
            {
                string op1 = temp.getOp1();
                int sizeOp1 = op1.length();
                int index1 = checkRegister(op1, sizeOp1); //return R index;
                if (index1 >= 1 && index1 <= 15)
                {
                    index1 = index1 - 1;
                    string op2 = temp.getOp2();
                    if (op2[0] == ' ')
                    {
                        op2 = eraseCharAtIndex(op2, 0);
                        int sizeOp2 = op2.length();
                        bool check = checkSpace(op2);
                        if (check == true)
                        {
                            int address = this->ip - 1;
                            InvalidInstruction e = InvalidInstruction(address);
                            throw e;
                            break;
                        }
                        else
                        {
                            if (op2[0] == 'R')
                            {
                                int index2 = checkRegister(op2, sizeOp2); //return R index;
                                if (index2 >= 1 && index2 <= 15)
                                {
                                    index2 = index2 - 1;
                                    int check1 = this->Register[index1].getTypeData();
                                    int check2 = this->Register[index2].getTypeData();
                                    if (check1 == 4)
                                    {
                                        int address = this->Register[index1].getAddress();

                                        if (address > 65535 && address < 0)
                                        {
                                            int address = this->ip - 1;
                                            TypeMismatch e = TypeMismatch(address);
                                            throw e;
                                        }

                                        else
                                        {
                                            DataStorage temp;
                                            switch (check2)
                                            {
                                            case 1:
                                            {
                                                temp.setDataInt(this->Register[index2].getDataInt());
                                                break;
                                            }
                                            case 2:
                                            {
                                                temp.setDataFloat(this->Register[index2].getDataFloat());
                                                break;
                                            }
                                            case 3:
                                            {
                                                temp.setDataBool(this->Register[index2].getDataBool());
                                                break;
                                            }
                                            case 4:
                                            {
                                                temp.setAddress(this->Register[index2].getAddress());
                                                break;
                                            }
                                            default:
                                            {
                                                int add = this->ip - 1; // address
                                                TypeMismatch e = TypeMismatch(add);
                                                break;
                                            }
                                            };
                                            this->staticMemory[address] = temp;
                                        }
                                    }
                                    else
                                    {
                                        int address = this->ip - 1;
                                        TypeMismatch e = TypeMismatch(address);
                                        throw e;
                                        break;
                                    }
                                }
                                else
                                {
                                    int address = this->ip - 1;
                                    InvalidOperand e = InvalidOperand(address);
                                    throw e;
                                    break;
                                }
                            }
                            else if (op2[0] != 'R')
                            {
                                DataStorage value;
                                int checkLiteral = checkOperand2(op2, value);
                                int checkTypeR = this->Register[index1].getTypeData();
                                if (checkLiteral == 0)
                                {
                                    int address = this->ip - 1;
                                    InvalidInstruction e = InvalidInstruction(address);
                                    throw e;
                                    break;
                                }
                                if (checkTypeR == 4)
                                {
                                    int address = this->Register[index1].getAddress();
                                    if (address > 65535 && address < 0)
                                    {
                                        int address = this->ip - 1;
                                        TypeMismatch e = TypeMismatch(address);
                                        throw e;
                                        break;
                                    }

                                    else
                                    {
                                        int checkValueOfSrc = value.getTypeData();
                                        if (checkValueOfSrc == 1)
                                        {
                                            int dataInt = value.getDataInt();
                                            this->staticMemory[address].setDataInt(dataInt);
                                        }
                                        else if (checkValueOfSrc == 2)
                                        {
                                            double dataFloat = value.getDataFloat();
                                            this->staticMemory[address].setDataFloat(dataFloat);
                                        }
                                        else if (checkValueOfSrc == 3)
                                        {
                                            bool dataBool = value.getDataBool();
                                            this->staticMemory[address].setDataFloat(dataBool);
                                        }
                                        else if (checkValueOfSrc == 4)
                                        {
                                            int dataAddress = value.getAddress();
                                            this->staticMemory[address].setDataFloat(dataAddress);
                                        }
                                        else
                                        {
                                            int address = this->ip - 1;
                                            TypeMismatch e = TypeMismatch(address);
                                            throw e;
                                            break;
                                        }
                                    }
                                }
                                else
                                {
                                    int address = this->ip - 1;
                                    TypeMismatch e = TypeMismatch(address);
                                    throw e;
                                }
                            }
                        }
                    }
                    else
                    {
                        int address = this->ip - 1;
                        InvalidOperand e = InvalidOperand(address);
                        throw e;
                        break;
                    }
                }
                else
                {
                    int address = this->ip - 1;
                    InvalidOperand e = InvalidOperand(address);
                    throw e;
                    break;
                }
            }

            else
            {
                int address = this->ip - 1;
                InvalidInstruction e = InvalidInstruction(address);
                throw e;
                break;
            }
        }
        else if (opcode == "Jump")
        {
        }
        else
        {
            int address = this->ip - 1;
            InvalidInstruction e = InvalidInstruction(address);
            throw e;
            break;
        }
    } //Else if sum of opcode
};    //while loop

Instruction Instruction::getElementInstruction(string str)
{
    //erase '\r'
    string Code = eraseChar(str, '\r');
    int i = 0;
    while (Code[i] != '\0')
    {
        for (i; !(Code[i] == ' ' || Code[i] == '\0'); i++)
        {
            this->nameOpcode += Code[i];
        }
        if (!(Code[i] == '\0'))
        {
            ++i;
            for (i; !(Code[i] == ',' || Code[i] == '\0'); i++)
            {
                this->op1 += Code[i];
            }
            this->nOperands++;
            if (Code[i] == ',')
            {
                i++;
                while (!(Code[i] == '\0'))
                {
                    this->op2 += Code[i];
                    i++;
                }
                this->nOperands++;
            }
        }
        else
        {
            break;
        }
    }
    return *this;
}
