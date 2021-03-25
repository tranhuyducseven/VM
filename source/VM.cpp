#include "VM.h"
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
                if (1 <= index <= 9)
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
                if (10 <= index <= 15)
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
int checkOperand2_Arithmetic(string string, int sizeofString)
{
    return 1;
}
// // DataStorage Class
// /////////////////////////////////////////////////////////
// /////////////////////////////////////////////////////////
DataStorage::DataStorage()
{
    this->dataInt = 0;
    this->dataFloat = 0;
    this->dataBool = false;
};
int DataStorage::getDataInt()
{
    return this->dataInt;
};
float DataStorage::getDataFLoat()
{
    return this->dataFloat;
};
bool DataStorage::getDataBool()
{
    return this->dataBool;
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
    this->codes = new string[this->nCode];
    for (int i = 0; i < this->nCode; i++)
    {
        this->codes[i] = "\0";
    }

    readCode(filename);
    int numlines = this->nCode;
    cout<<"numlines"<<numlines<<endl;
    Instruction *instructions = new Instruction[numlines];
    for (int i = 0; i < numlines; i++)
    {
        instructions[i] = instructions[i].getElementInstruction(this->codes[i]);
    }
    VM *newVM = new VM(instructions, 0, numlines);
    for (int i = 0; i < numlines; i++)
    {
        cout << "instr[" << i << "]" << newVM->instr[0].getNameOpcode() << "xxx" << endl;
    }
    newVM->Register = new DataStorage[15];
    newVM->staticMemory = new DataStorage[65536];
    newVM->stack = new int[1000];
    newVM->cpu();
    delete[] instructions;
    delete newVM;
}
void VM::cpu()
{
    int lengthOfCode = this->nCode;
    while (this->ip < lengthOfCode)
    {
        Instruction temp = this->instr[ip];
        string opcode = temp.getNameOpcode();

        cout << "opcode is:" << opcode << endl;
        ip++;
        this->ip++;
        if (opcode == "Add" || opcode == "Minus" || opcode == "Mul" || opcode == "Div")
        {
            if (temp.getNOperands() == 2)
            {
                string op1 = temp.getOp1();
                int sizeOp1 = sizeof(op1);
                int index1 = checkRegister(op1, sizeOp1); //return R index;
                if (1 <= index1 <= 15)
                {
                    index1 = index1 - 1;
                }
                else
                {
                    cout << "Wrong R" << endl;
                }
                string op2 = temp.getOp2();
                int sizeOp2 = sizeof(op2);
                //check Operand2 is register or literal
                // if register
                if (op2[0] == ' ')
                {
                    if (op2[1] == 'R')
                    {
                        int index2 = checkRegister(op2, sizeOp2); //return R index;
                        if (1 <= index1 <= 15)
                        {
                            index2 = index2 - 1;
                            // demo
                            ////
                            ////////////////////
                            this->Register[index2].dataInt = 1;
                            ///////////
                            int int_dest = this->Register[index1].dataInt;
                            int int_src = this->Register[index2].dataInt;
                            float float_dest = this->Register[index1].dataFloat;
                            float float_src = this->Register[index2].dataFloat;
                            if (opcode == "Add")
                            {
                                if (int_dest && int_src)
                                {
                                    this->Register[index1].dataInt += this->Register[index2].dataInt;
                                    cout << this->Register[index1].dataInt << endl
                                         << this->Register[index1].dataFloat << endl
                                         << this->Register[index1].dataBool << endl;
                                }
                                else if (int_dest && float_src)
                                {
                                    this->Register[index1].dataFloat = this->Register[index1].dataInt + this->Register[index2].dataFloat;
                                    this->Register[index1].dataInt = 0;
                                }
                                else if (float_dest && int_src)
                                {
                                    this->Register[index1].dataFloat += this->Register[index2].dataInt;
                                }
                            }
                            else if (opcode == "Minus")
                            {
                                if (int_dest && int_src)
                                {
                                    this->Register[index1].dataInt -= this->Register[index2].dataInt;
                                }
                                else if (int_dest && float_src)
                                {
                                    this->Register[index1].dataFloat = this->Register[index1].dataInt - this->Register[index2].dataFloat;
                                    this->Register[index1].dataInt = 0;
                                }
                                else if (float_dest && int_src)
                                {
                                    this->Register[index1].dataFloat -= this->Register[index2].dataInt;
                                }
                            }
                            else if (opcode == "Mul")
                            {
                                if (int_dest && int_src)
                                {
                                    this->Register[index1].dataInt *= this->Register[index2].dataInt;
                                }
                                else if (int_dest && float_src)
                                {
                                    this->Register[index1].dataFloat = this->Register[index1].dataInt * this->Register[index2].dataFloat;
                                    this->Register[index1].dataInt = 0;
                                }
                                else if (float_dest && int_src)
                                {
                                    this->Register[index1].dataFloat *= this->Register[index2].dataInt;
                                }
                            }
                            else if (opcode == "Div")
                            {
                                if (int_dest && int_src)
                                {
                                    this->Register[index1].dataInt /= this->Register[index2].dataInt;
                                }
                                else if (int_dest && float_src)
                                {
                                    this->Register[index1].dataFloat = this->Register[index1].dataInt / this->Register[index2].dataFloat;
                                    this->Register[index1].dataInt = 0;
                                }
                                else if (float_dest && int_src)
                                {
                                    this->Register[index1].dataFloat /= this->Register[index2].dataInt;
                                }
                            }
                        }
                    }
                    else
                    {
                        cout << "Wrong R" << endl;
                    }
                }
                else if (op2[1] != 'R')
                {
                    int checkOp2 = checkOperand2_Arithmetic(op2, sizeOp2);
                    if (checkOp2 == 0)
                    {
                        cout << "HELLO, Op1 checked int";
                    }
                    else if (checkOp2 == 1)
                    {
                        cout << "Hello, Op2 check int";
                    }
                }
            }
            else
            {
                ////// no real Noprandes;
                cout << "Wrong N operands;" << endl;
            }
        }
        else
        {
            //Wrong  operands misMatch Add Minus...
            cout << "Wrong operands" << endl;
        }
    }
};
Instruction Instruction::getElementInstruction(string str)
{
    //Erase '/r'
    string Code;
    for (size_t i = 0; i < str.size(); i++)
    {
        char currentChar = str[i];
        if (currentChar != '\r')
            Code += currentChar;
    }
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