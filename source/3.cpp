#include <iostream>
#include <fstream>
#include <string>
using namespace std;

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
        numlines++;
        fileInput.get(c);
    }
    return numlines;
}
int main()
{
    int a = 0;
    int *p = &a;
    cout << countNCode("prog5.txt") << endl;
    
    return 0;
}