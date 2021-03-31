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

        for (int k = i + 1; k < length; k++)
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

        for (int t = 0; t < length; t++)
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
    this->dataInt = 0;
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
    this->sp = -1;
}
VM::~VM(){};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VM::readCode(string filename)
{
    fstream fileInput;
    fileInput.open(filename);
    char c;
    fileInput.get(c);
    int i = 0;
    //int numlines=0;
    while (fileInput)
    {
        while (fileInput && c != '\n')
        {
            this->codes[i] += c;
            fileInput.get(c);
        }
        i++;
        //++numlines;
        fileInput.get(c);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VM::run(string filename)
{
    this->nCode = countNCode(filename);
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
    delete[] newVM->Register;
    delete[] newVM->staticMemory;
    delete[] newVM->stack;
    delete newVM;
    delete[] instructions;
}

void VM::cpu()
{
    bool checkLoadingError = false;
    int lengthOfCode = this->nCode;
    while (this->ip < lengthOfCode)
    {
        Instruction temp = this->instr[ip];
        string opcode = temp.getNameOpcode();
        this->ip++;
        if (opcode == "Add" || opcode == "Minus" || opcode == "Mul" || opcode == "Div" || opcode == "CmpEQ" || opcode == "CmpNE" || opcode == "CmpLT" || opcode == "CmpLE" || opcode == "CmpGT" || opcode == "CmpGE" || opcode == "And" || opcode == "Or" || opcode == "Move" || opcode == "Load" || opcode == "Store" || opcode == "JumpIf")
        {
            if (temp.getNOperands() != 2)
            {
                checkLoadingError = true;
                int addressError = this->ip - 1;
                InvalidInstruction e = InvalidInstruction(addressError);
                throw e;
                break;
            }
            string op1 = temp.getOp1();
            int sizeOp1 = op1.length();
            int index1 = checkRegister(op1, sizeOp1); //return R index;
            if (index1 < 1 || index1 > 15)
            {
                checkLoadingError = true;
                int addressError = this->ip - 1;
                InvalidOperand e = InvalidOperand(addressError);
                throw e;
                break;
            }
            string op2 = temp.getOp2();
            if (op2[0] != ' ')
            {
                checkLoadingError = true;
                int addressError = this->ip - 1;
                InvalidInstruction e = InvalidInstruction(addressError);
                throw e;
                break;
            }
            op2 = eraseCharAtIndex(op2, 0);
            int sizeOp2 = op2.length();
            bool check = checkSpace(op2);
            if (check == true)
            {
                checkLoadingError = true;
                int addressError = this->ip - 1;
                InvalidInstruction e = InvalidInstruction(addressError);
                throw e;
                break;
            }
            if (op2[0] == 'R')
            {
                int index2 = checkRegister(op2, sizeOp2); //return R index;
                if (index2 < 1 || index2 > 15)
                {
                    checkLoadingError = true;
                    int addressError = this->ip - 1;
                    InvalidOperand e = InvalidOperand(addressError);
                    throw e;
                    break;
                }
            }
            else if (op2[0] != 'R')
            {
                DataStorage value;
                int checkLiteral = checkOperand2(op2, value);
                if (checkLiteral == 0)
                {
                    checkLoadingError = true;
                    int addressError = this->ip - 1;
                    InvalidOperand e = InvalidOperand(addressError);
                    throw e;
                    break;
                }
            }
        }
        else if (opcode == "Not" || opcode == "Input")
        {
            if (temp.getNOperands() != 1)
            {
                checkLoadingError = true;
                int addressError = this->ip - 1;
                InvalidInstruction e = InvalidInstruction(addressError);
                throw e;
                break;
            }

            string op1 = temp.getOp1();
            int sizeOp1 = op1.length();
            if (op1[0] == 'R')
            {
                int index1 = checkRegister(op1, sizeOp1); //return R index;
                if (index1 < 1 || index1 > 15)

                {
                    checkLoadingError = true;
                    int addressError = this->ip - 1;
                    InvalidOperand e = InvalidOperand(addressError);
                    throw e;
                    break;
                }
            }
            else
            {
                checkLoadingError = true;
                int addressError = this->ip - 1;
                InvalidInstruction e = InvalidInstruction(addressError);
                throw e;
                break;
            }
        }
        else if (opcode == "Jump" || opcode == "Call" || opcode == "Output")
        {
            if (temp.getNOperands() != 1)
            {
                checkLoadingError = true;
                int addressError = this->ip - 1;
                InvalidInstruction e = InvalidInstruction(addressError);
                throw e;
                break;
            }

            string op1 = temp.getOp1();
            int sizeOp1 = op1.length();
            if (op1[0] == 'R')
            {
                int index1 = checkRegister(op1, sizeOp1); //return R index;
                if (index1 < 1 || index1 > 15)

                {
                    checkLoadingError = true;
                    int addressError = this->ip - 1;
                    InvalidOperand e = InvalidOperand(addressError);
                    throw e;
                    break;
                }
            }
            else if (op1[0] != 'R')
            {
                DataStorage value;
                int checkLiteral = checkOperand2(op1, value);
                if (checkLiteral == 0)
                {
                    checkLoadingError = true;
                    int addressError = this->ip - 1;
                    InvalidOperand e = InvalidOperand(addressError);
                    throw e;
                    break;
                }
            }
        }
        else if (opcode == "Return" || opcode == "Halt")
        {
            if (temp.getNOperands() != 0)
            {
                checkLoadingError = true;
                int addressError = this->ip - 1;
                InvalidInstruction e = InvalidInstruction(addressError);
                throw e;
                break;
            }
        }
        else
        {
            checkLoadingError = true;
            int addressError = this->ip - 1;
            InvalidInstruction e = InvalidInstruction(addressError);
            throw e;
            break;
        }
    }
    this->ip = 0;
    if (!checkLoadingError)
    {
        while (this->ip < lengthOfCode)
        {
            Instruction temp = this->instr[ip];
            string opcode = temp.getNameOpcode();
            this->ip++;

            if (opcode == "Add" || opcode == "Minus" || opcode == "Mul" || opcode == "Div")
            {
                string op1 = temp.getOp1();
                int sizeOp1 = op1.length();
                int index1 = checkRegister(op1, sizeOp1) - 1;
                string op2 = temp.getOp2();
                op2 = eraseCharAtIndex(op2, 0);
                int sizeOp2 = op2.length();
                if (op2[0] == 'R')
                {
                    int index2 = checkRegister(op2, sizeOp2); //return R index;
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
                            this->Register[index1].setDataFloat((double)(this->Register[index1].getDataInt()) + data);
                        }
                        else if (check1 == 2 && check2 == 1)
                        {
                            int data = this->Register[index2].getDataInt();
                            this->Register[index1].setDataFloat(this->Register[index1].getDataFloat() + (double)(data));
                        }
                        else if (check1 == 2 && check2 == 2)
                        {
                            double data = this->Register[index2].getDataFloat();
                            this->Register[index1].setDataFloat(this->Register[index1].getDataFloat() + data);
                        }
                        else
                        {
                            int addressError = this->ip - 1;
                            TypeMismatch e = TypeMismatch(addressError);
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
                            this->Register[index1].setDataFloat((double)(this->Register[index1].getDataInt()) - data);
                        }
                        else if (check1 == 2 && check2 == 1)
                        {
                            int data = this->Register[index2].getDataInt();
                            this->Register[index1].setDataFloat(this->Register[index1].getDataFloat() - (double)(data));
                        }
                        else if (check1 == 2 && check2 == 2)
                        {
                            double data = this->Register[index2].getDataFloat();
                            this->Register[index1].setDataFloat(this->Register[index1].getDataFloat() - data);
                        }
                        else
                        {
                            int addressError = this->ip - 1;
                            TypeMismatch e = TypeMismatch(addressError);
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
                            this->Register[index1].setDataFloat((double)(this->Register[index1].getDataInt()) * data);
                        }
                        else if (check1 == 2 && check2 == 1)
                        {
                            int data = this->Register[index2].getDataInt();
                            this->Register[index1].setDataFloat(this->Register[index1].getDataFloat() * (double)(data));
                        }
                        else if (check1 == 2 && check2 == 2)
                        {
                            double data = this->Register[index2].getDataFloat();
                            this->Register[index1].setDataFloat(this->Register[index1].getDataFloat() * data);
                        }
                        else
                        {
                            int addressError = this->ip - 1;
                            TypeMismatch e = TypeMismatch(addressError);
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
                                int addressError = this->ip - 1;
                                DivideByZero e = DivideByZero(addressError);
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
                                int addressError = this->ip - 1;
                                DivideByZero e = DivideByZero(addressError);
                                throw e;
                                break;
                            }
                            else
                            {
                                double data = this->Register[index2].getDataFloat();
                                this->Register[index1].setDataFloat((double)(this->Register[index1].getDataInt()) / data);
                            }
                        }
                        else if (check1 == 2 && check2 == 1)
                        {
                            if (this->Register[index2].getDataInt() == 0)
                            {
                                int addressError = this->ip - 1;
                                DivideByZero e = DivideByZero(addressError);
                                throw e;
                                break;
                            }
                            else
                            {
                                int data = this->Register[index2].getDataInt();
                                this->Register[index1].setDataFloat((double)(this->Register[index1].getDataFloat()) / data);
                            }
                        }
                        else if (check1 == 2 && check2 == 2)
                        {
                            if (this->Register[index2].getDataFloat() == 0)
                            {
                                int addressError = this->ip - 1;
                                DivideByZero e = DivideByZero(addressError);
                                throw e;
                                break;
                            }
                            else
                            {
                                double data = this->Register[index2].getDataFloat();
                                this->Register[index1].setDataFloat(this->Register[index1].getDataFloat() / data);
                            }
                        }
                        else
                        {
                            int addressError = this->ip - 1;
                            TypeMismatch e = TypeMismatch(addressError);
                            throw e;
                            break;
                        }
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
                            this->Register[index1].setDataInt(this->Register[index1].getDataInt() + value.getDataInt());
                        }
                        else if (checkTypeR == 1 && checkLiteral == 2)
                        {

                            this->Register[index1].setDataFloat((double)(this->Register[index1].getDataInt()) + value.getDataFloat());
                        }
                        else if (checkTypeR == 2 && checkLiteral == 1)
                        {

                            this->Register[index1].setDataFloat(this->Register[index1].getDataFloat() + (double)(value.getDataInt()));
                        }
                        else if (checkTypeR == 2 && checkLiteral == 2)
                        {

                            this->Register[index1].setDataFloat(value.getDataFloat() + this->Register[index1].getDataFloat());
                        }
                        else
                        {
                            int addressError = this->ip - 1;
                            TypeMismatch e = TypeMismatch(addressError);
                            throw e;
                            break;
                        }
                    }
                    else if (opcode == "Minus")
                    {
                        if (checkTypeR == 1 && checkLiteral == 1)
                        {
                            this->Register[index1].setDataInt(this->Register[index1].getDataInt() - value.getDataInt());
                        }
                        else if (checkTypeR == 1 && checkLiteral == 2)
                        {

                            this->Register[index1].setDataFloat((double)(this->Register[index1].getDataInt()) - value.getDataFloat());
                        }
                        else if (checkTypeR == 2 && checkLiteral == 1)
                        {

                            this->Register[index1].setDataFloat(this->Register[index1].getDataFloat() - (double)(value.getDataInt()));
                        }
                        else if (checkTypeR == 2 && checkLiteral == 2)
                        {

                            this->Register[index1].setDataFloat(value.getDataFloat() - this->Register[index1].getDataFloat());
                        }
                        else
                        {
                            int addressError = this->ip - 1;
                            TypeMismatch e = TypeMismatch(addressError);
                            throw e;
                            break;
                        }
                    }
                    else if (opcode == "Mul")
                    {
                        if (checkTypeR == 1 && checkLiteral == 1)
                        {
                            this->Register[index1].setDataInt(this->Register[index1].getDataInt() * value.getDataInt());
                        }
                        else if (checkTypeR == 1 && checkLiteral == 2)
                        {

                            this->Register[index1].setDataFloat((double)(this->Register[index1].getDataInt()) * value.getDataFloat());
                        }
                        else if (checkTypeR == 2 && checkLiteral == 1)
                        {

                            this->Register[index1].setDataFloat(this->Register[index1].getDataFloat() * (double)(value.getDataInt()));
                        }
                        else if (checkTypeR == 2 && checkLiteral == 2)
                        {

                            this->Register[index1].setDataFloat(value.getDataFloat() * this->Register[index1].getDataFloat());
                        }
                        else
                        {
                            int addressError = this->ip - 1;
                            TypeMismatch e = TypeMismatch(addressError);
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
                                int addressError = this->ip - 1;
                                DivideByZero e = DivideByZero(addressError);
                                throw e;
                                break;
                            }
                            else
                            {

                                this->Register[index1].setDataInt(this->Register[index1].getDataInt() / value.getDataInt());
                            }
                        }
                        else if (checkTypeR == 1 && checkLiteral == 2)
                        {
                            if (value.getDataFloat() == 0)
                            {
                                int addressError = this->ip - 1;
                                DivideByZero e = DivideByZero(addressError);
                                throw e;
                                break;
                            }
                            else
                            {

                                this->Register[index1].setDataFloat((double)(this->Register[index1]).getDataInt() / value.getDataInt());
                            }
                        }
                        else if (checkTypeR == 2 && checkLiteral == 1)
                        {
                            if (value.getDataInt() == 0)
                            {
                                int addressError = this->ip - 1;
                                DivideByZero e = DivideByZero(addressError);
                                throw e;
                                break;
                            }
                            else
                            {
                                this->Register[index1].setDataFloat(this->Register[index1].getDataFloat() / (double)(value.getDataInt()));
                            }
                        }
                        else if (checkTypeR == 2 && checkLiteral == 2)
                        {
                            if (value.getDataFloat() == 0)
                            {
                                int addressError = this->ip - 1;
                                DivideByZero e = DivideByZero(addressError);
                                throw e;
                                break;
                            }
                            else
                            {
                                this->Register[index1].setDataFloat(this->Register[index1].getDataFloat() / value.getDataFloat());
                            }
                        }
                        else
                        {
                            int addressError = this->ip - 1;
                            TypeMismatch e = TypeMismatch(addressError);
                            throw e;
                            break;
                        }
                    }
                }
            }
            else if (opcode == "CmpEQ" || opcode == "CmpNE" || opcode == "CmpLT" || opcode == "CmpLE" || opcode == "CmpGT" || opcode == "CmpGE")
            {

                string op1 = temp.getOp1();
                int sizeOp1 = op1.length();
                int index1 = checkRegister(op1, sizeOp1);
                index1 = index1 - 1;
                string op2 = temp.getOp2();
                op2 = eraseCharAtIndex(op2, 0);
                int sizeOp2 = op2.length();
                if (op2[0] == 'R')
                {
                    int index2 = checkRegister(op2, sizeOp2) - 1;
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
                        else
                        {
                            int addressError = this->ip - 1;
                            TypeMismatch e = TypeMismatch(addressError);
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

                        else
                        {
                            int addressError = this->ip - 1;
                            TypeMismatch e = TypeMismatch(addressError);
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

                        else
                        {
                            int addressError = this->ip - 1;
                            TypeMismatch e = TypeMismatch(addressError);
                            throw e;
                            break;
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

                        else
                        {
                            int addressError = this->ip - 1;
                            TypeMismatch e = TypeMismatch(addressError);
                            throw e;
                            break;
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

                        else
                        {
                            int addressError = this->ip - 1;
                            TypeMismatch e = TypeMismatch(addressError);
                            throw e;
                            break;
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

                        else
                        {
                            int addressError = this->ip - 1;
                            TypeMismatch e = TypeMismatch(addressError);
                            throw e;
                            break;
                        }
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
                        else
                        {
                            int addressError = this->ip - 1;
                            TypeMismatch e = TypeMismatch(addressError);
                            throw e;
                            break;
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

                        else
                        {
                            int addressError = this->ip - 1;
                            TypeMismatch e = TypeMismatch(addressError);
                            throw e;
                            break;
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

                        else
                        {
                            int addressError = this->ip - 1;
                            TypeMismatch e = TypeMismatch(addressError);
                            throw e;
                            break;
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

                        else
                        {
                            int addressError = this->ip - 1;
                            TypeMismatch e = TypeMismatch(addressError);
                            throw e;
                            break;
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

                        else
                        {
                            int addressError = this->ip - 1;
                            TypeMismatch e = TypeMismatch(addressError);
                            throw e;
                            break;
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

                        else
                        {
                            int addressError = this->ip - 1;
                            TypeMismatch e = TypeMismatch(addressError);
                            throw e;
                            break;
                        }
                    }
                }
            }
            else if (opcode == "Not")
            {

                string op1 = temp.getOp1();
                int sizeOp1 = op1.length();
                int index1 = checkRegister(op1, sizeOp1);
                int check1 = this->Register[index1].getTypeData();
                if (check1 == 3)
                {
                    bool value = !(this->Register[index1].getDataBool());
                    this->Register[index1].setDataBool(value);
                    this->Register[index1].setTypeData(3);
                }
                else
                {
                    int addressError = this->ip - 1;
                    TypeMismatch e = TypeMismatch(addressError);
                    throw e;
                    break;
                }
            }
            else if (opcode == "And" || opcode == "Or")
            {
                string op1 = temp.getOp1();
                int sizeOp1 = op1.length();
                int index1 = checkRegister(op1, sizeOp1) - 1;
                string op2 = temp.getOp2();
                op2 = eraseCharAtIndex(op2, 0);
                int sizeOp2 = op2.length();
                if (op2[0] == 'R')
                {
                    int index2 = checkRegister(op2, sizeOp2) - 1;
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
                            int addressError = this->ip - 1;
                            TypeMismatch e = TypeMismatch(addressError);
                            throw e;
                            break;
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
                            int addressError = this->ip - 1;
                            TypeMismatch e = TypeMismatch(addressError);
                            throw e;
                            break;
                        }
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

                        else
                        {
                            int addressError = this->ip - 1;
                            TypeMismatch e = TypeMismatch(addressError);
                            throw e;
                            break;
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

                        else
                        {
                            int addressError = this->ip - 1;
                            TypeMismatch e = TypeMismatch(addressError);
                            throw e;
                            break;
                        }
                    }
                }
            }
            else if (opcode == "Move")
            {
                string op1 = temp.getOp1();
                int sizeOp1 = op1.length();
                int index1 = checkRegister(op1, sizeOp1) - 1;
                string op2 = temp.getOp2();
                op2 = eraseCharAtIndex(op2, 0);
                int sizeOp2 = op2.length();
                if (op2[0] == 'R')
                {
                    int index2 = checkRegister(op2, sizeOp2) - 1;
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
                }
                else if (op2[0] != 'R')
                {
                    DataStorage value;
                    int checkLiteral = checkOperand2(op2, value);
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
                }
            }
            else if (opcode == "Load")
            {
                string op1 = temp.getOp1();
                int sizeOp1 = op1.length();
                int index1 = checkRegister(op1, sizeOp1) - 1;
                string op2 = temp.getOp2();
                op2 = eraseCharAtIndex(op2, 0);
                int sizeOp2 = op2.length();
                if (op2[0] == 'R')
                {
                    int index2 = checkRegister(op2, sizeOp2) - 1;
                    int check2 = this->Register[index2].getTypeData();
                    if (check2 == 4)
                    {
                        int address = this->Register[index2].getAddress();
                        if (address > 65535 || address < 0)
                        {
                            int addressError = this->ip - 1;
                            TypeMismatch e = TypeMismatch(addressError);
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
                        }
                    }
                    else
                    {
                        int addressError = this->ip - 1;
                        TypeMismatch e = TypeMismatch(addressError);
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
                        if (address > 65535 || address < 0)
                        {
                            int addressError = this->ip - 1;
                            TypeMismatch e = TypeMismatch(addressError);
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
                        }
                    }

                    else
                    {
                        int addressError = this->ip - 1;
                        TypeMismatch e = TypeMismatch(addressError);
                        throw e;
                        break;
                    }
                }
            }
            else if (opcode == "Store")
            {
                string op1 = temp.getOp1();
                int sizeOp1 = op1.length();
                int index1 = checkRegister(op1, sizeOp1) - 1;
                string op2 = temp.getOp2();
                op2 = eraseCharAtIndex(op2, 0);
                int sizeOp2 = op2.length();
                if (op2[0] == 'R')
                {
                    int index2 = checkRegister(op2, sizeOp2) - 1;
                    int check1 = this->Register[index1].getTypeData();
                    int check2 = this->Register[index2].getTypeData();
                    if (check1 == 4)
                    {
                        int address = this->Register[index1].getAddress();

                        if (address > 65535 || address < 0)
                        {
                            int addressError = this->ip - 1;
                            TypeMismatch e = TypeMismatch(addressError);
                            throw e;
                            break;
                        }

                        else
                        {
                            DataStorage temp;
                            if (check2 == 1)
                            {
                                temp.setDataInt(this->Register[index2].getDataInt());
                                this->staticMemory[address] = temp;
                            }
                            else if (check2 == 2)
                            {
                                temp.setDataFloat(this->Register[index2].getDataFloat());
                                this->staticMemory[address] = temp;
                            }
                            else if (check2 == 3)
                            {
                                temp.setDataBool(this->Register[index2].getDataBool());
                                this->staticMemory[address] = temp;
                            }
                            else if (check2 == 4)
                            {

                                int addressError = this->ip - 1;
                                TypeMismatch e = TypeMismatch(addressError);
                                throw e;
                                break;
                            }
                        }
                    }
                    else
                    {
                        int addressError = this->ip - 1;
                        TypeMismatch e = TypeMismatch(addressError);
                        throw e;
                        break;
                    }
                }

                else if (op2[0] != 'R')
                {
                    DataStorage value;
                    int checkTypeR = this->Register[index1].getTypeData();
                    if (checkTypeR == 4)
                    {
                        int address = this->Register[index1].getAddress();
                        if (address > 65535 || address < 0)
                        {
                            int addressError = this->ip - 1;
                            TypeMismatch e = TypeMismatch(addressError);
                            throw e;
                            break;
                        }

                        else
                        {
                            int checkValueOfSrc = checkOperand2(op2, value);
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
                        }
                    }
                    else
                    {
                        int addressError = this->ip - 1;
                        TypeMismatch e = TypeMismatch(addressError);
                        throw e;
                        break;
                    }
                }
            }
            else if (opcode == "Jump")
            {
                string op1 = temp.getOp1();
                int sizeOp1 = op1.length();
                if (op1[0] == 'R')
                {
                    int index1 = checkRegister(op1, sizeOp1) - 1;
                    int check1 = this->Register[index1].getTypeData();
                    if (check1 == 4)
                    {
                        int address = this->Register[index1].getAddress();
                        if (address >= 0 && address <= lengthOfCode - 1)
                        {
                            this->ip = address;
                        }
                        else
                        {
                            int addressError = this->ip - 1;
                            InvalidDestination e = InvalidDestination(addressError);
                            throw e;
                            break;
                        }
                    }

                    else
                    {
                        int addressError = this->ip - 1;
                        TypeMismatch e = TypeMismatch(addressError);
                        throw e;
                        break;
                    }
                }
                else if (op1[0] != 'R')
                {
                    DataStorage value;
                    int checkLiteral = checkOperand2(op1, value);
                    if (checkLiteral == 4)
                    {
                        int address = value.getAddress();
                        if (address >= 0 && address <= lengthOfCode - 1)
                        {
                            this->ip = address;
                        }
                        else
                        {
                            int addressError = this->ip - 1;
                            InvalidDestination e = InvalidDestination(addressError);
                            throw e;
                            break;
                        }
                    }

                    else
                    {
                        int addressError = this->ip - 1;
                        TypeMismatch e = TypeMismatch(addressError);
                        throw e;
                        break;
                    }
                }
            }
            else if (opcode == "JumpIf")
            {
                string op1 = temp.getOp1();
                int sizeOp1 = op1.length();
                int index1 = checkRegister(op1, sizeOp1) - 1;
                string op2 = temp.getOp2();
                op2 = eraseCharAtIndex(op2, 0);
                int sizeOp2 = op2.length();
                ////////////
                if (op2[0] == 'R')
                {
                    int index2 = checkRegister(op2, sizeOp2) - 1;
                    int check1 = this->Register[index1].getTypeData();
                    int check2 = this->Register[index2].getTypeData();
                    if (check1 == 3)
                    {
                        if (this->Register[index1].getDataBool())
                        {
                            if (check2 == 4)
                            {
                                int address = this->Register[index2].getAddress();
                                if (address >= 0 && address <= lengthOfCode - 1)
                                {
                                    this->ip = address;
                                }
                                else
                                {
                                    int addressError = this->ip - 1;
                                    InvalidDestination e = InvalidDestination(addressError);
                                    throw e;
                                    break;
                                }
                            }
                            else
                            {
                                int addressError = this->ip - 1;
                                TypeMismatch e = TypeMismatch(addressError);
                                throw e;
                                break;
                            }
                        }
                    }
                    else
                    {
                        int addressError = this->ip - 1;
                        TypeMismatch e = TypeMismatch(addressError);
                        throw e;
                        break;
                    }
                }

                else if (op2[0] != 'R')
                {
                    DataStorage value;
                    int checkLiteral = checkOperand2(op2, value);
                    int checkTypeR = this->Register[index1].getTypeData();
                    if (checkTypeR == 3)
                    {
                        if (this->Register[index1].getDataBool())
                        {
                            if (checkLiteral == 4)
                            {
                                int address = value.getAddress();
                                if (address >= 0 && address <= lengthOfCode - 1)
                                {
                                    this->ip = address;
                                }
                                else
                                {
                                    int addressError = this->ip - 1;
                                    InvalidDestination e = InvalidDestination(addressError);
                                    throw e;
                                    break;
                                }
                            }
                            else
                            {
                                int addressError = this->ip - 1;
                                TypeMismatch e = TypeMismatch(addressError);
                                throw e;
                                break;
                            }
                        }
                    }
                    else
                    {
                        int addressError = this->ip - 1;
                        TypeMismatch e = TypeMismatch(addressError);
                        throw e;
                        break;
                    }
                }
            }
            else if (opcode == "Call")
            {
                string op1 = temp.getOp1();
                int sizeOp1 = op1.length();
                if (op1[0] == 'R')
                {
                    int index1 = checkRegister(op1, sizeOp1) - 1;
                    int check1 = this->Register[index1].getTypeData();
                    if (check1 == 4)
                    {
                        int address = this->Register[index1].getAddress();
                        if (address >= 0 && address <= lengthOfCode - 1)
                        {
                            this->sp++;
                            if (this->sp > 999)
                            {
                                int addressError = this->ip - 1;
                                StackFull e = StackFull(addressError);
                                throw e;
                                break;
                            }
                            else
                            {
                                int valueIP = this->ip;
                                this->stack[sp] = valueIP;
                                this->ip = address;
                            }
                        }
                        else
                        {
                            int addressError = this->ip - 1;
                            InvalidDestination e = InvalidDestination(addressError);
                            throw e;
                            break;
                        }
                    }
                    else
                    {
                        int addressError = this->ip - 1;
                        TypeMismatch e = TypeMismatch(addressError);
                        throw e;
                        break;
                    }
                }
                else if (op1[0] != 'R')
                {
                    DataStorage value;
                    int checkLiteral = checkOperand2(op1, value);
                    if (checkLiteral == 4)
                    {
                        int address = value.getAddress();
                        if (address >= 0 && address <= lengthOfCode - 1)
                        {
                            sp++;
                            if (sp > 999)
                            {
                                int addressError = this->ip - 1;
                                StackFull e = StackFull(addressError);
                                throw e;
                                break;
                            }
                            else
                            {

                                int valueIP = this->ip;
                                this->stack[sp] = valueIP;
                                this->ip = address;
                            }
                        }
                        else
                        {
                            int addressError = this->ip - 1;
                            InvalidDestination e = InvalidDestination(addressError);
                            throw e;
                            break;
                        }
                    }

                    else
                    {
                        int addressError = this->ip - 1;
                        TypeMismatch e = TypeMismatch(addressError);
                        throw e;
                        break;
                    }
                }
            }
            else if (opcode == "Return")
            {
                int valueSP = this->stack[sp];
                this->ip = valueSP;
                this->sp--;
            }
            else if (opcode == "Halt")
            {
                return;
            }
            else if (opcode == "Input")
            {
                string op1 = temp.getOp1();
                int sizeOp1 = op1.length();
                int index1 = checkRegister(op1, sizeOp1) - 1;
                DataStorage value;
                string inputLiteral = "";
                getline(cin, inputLiteral);
                int checkLiteral = checkOperand2(inputLiteral, value);
                if (checkLiteral == 0)
                {
                    int addressError = this->ip - 1;
                    InvalidInput e = InvalidInput(addressError);
                    throw e;
                    break;
                }
                else if (checkLiteral == 1)
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
                else
                {
                    int addressError = this->ip - 1;
                    TypeMismatch e = TypeMismatch(addressError);
                    throw e;
                    break;
                }
            }
            else if (opcode == "Output")
            {
                string op1 = temp.getOp1();
                int sizeOp1 = op1.length();
                if (op1[0] == 'R')
                {
                    int index1 = checkRegister(op1, sizeOp1) - 1;
                    int check1 = this->Register[index1].getTypeData();
                    if (check1 == 1)
                    {
                        cout << this->Register[index1].getDataInt();
                    }
                    else if (check1 == 2)
                    {
                        cout << this->Register[index1].getDataFloat();
                    }
                    else if (check1 == 3)
                    {
                        if (this->Register[index1].getDataBool())
                        {
                            cout << "true";
                        }
                        else
                            cout << "false";
                    }
                    else if (check1 == 4)
                    {
                        cout << this->Register[index1].getAddress() << 'A';
                    }
                }

                else if (op1[0] != 'R')
                {
                    DataStorage value;
                    int checkLiteral = checkOperand2(op1, value);
                    if (checkLiteral == 1)
                    {
                        cout << value.getDataInt();
                    }
                    else if (checkLiteral == 2)
                    {
                        cout << value.getDataFloat();
                    }
                    else if (checkLiteral == 3)
                    {
                        if (value.getDataBool())
                        {
                            cout << "true";
                        }
                        else
                            cout << "false";
                    }
                    else if (checkLiteral == 4)
                    {
                        cout << value.getAddress() << 'A';
                    }
                }
            }
        }
    }
};

Instruction Instruction::getElementInstruction(string str)
{
    //erase '\r'
    string Code = eraseChar(str, '\r');
    int i = 0;
    while (Code[i] != '\0')
    {

        while (!(Code[i] == ' ' || Code[i] == '\0'))
        {
            this->nameOpcode += Code[i];
            i++;
        }
        if (!(Code[i] == '\0'))
        {
            ++i;
            while (!(Code[i] == ',' || Code[i] == '\0'))
            {
                this->op1 += Code[i];
                i++;
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

            break;
    }
    return *this;
}