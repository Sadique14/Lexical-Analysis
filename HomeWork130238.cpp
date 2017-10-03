/**
Problem: "Finding lexemes from source program."
Input: Source program with one or more space/newline after each lexeme. Input ends when 'eof' is given after a newline.
Output: List of lexemes with their type.
**/
/**
Md. Farhan Sadique
Student ID: 130238
**/
#include<iostream>
#include<string>
#include<vector>
using namespace std;
bool checkKeyword(string str);
bool checkOperator(string str);
bool checkSymbol(string str);
bool isNumber(string str);

string keywords[] = {"bool", "char", "int", "long", "float", "double", "void", "return", "if", "else", "while", "do", "for", "goto",
                    "break", "switch", "case", "cin", "cout"};
string operators[] = {"+", "-", "*", "/", "%", "++", "--", "=", "+=", "-=", "*=", "/=", "==", "!=", ">", "<", ">=", "<=", "&&", "||",
                      "!", "&", "|", "^", "~", "<<", ">>"};
string symbols[] = {"(", ")", "{", "}", ";"};

int main()
{
    cout<<"Source Program:"<<endl;
    string program = "";           //for containing input program as a single string.
    string lexeme = "";            //for maintaining operation with each lexeme.
    string x;                      //for taking a line of input.
    vector<string> lexemes;        //Output: list of lexemes.
    vector<string> types;          //Output: types of corresponding lexemes.
    while (getline(cin,x))         //getline(cin,x)  takes a string input with spaces. When newline is given, body of the loop executes.
    {
        if(x=="eof")              //When eof is given as a input, source program ends.
            break;
        program += x;             //Adding several lines of source program into a single string 'program'
        program += " ";           //When a newline is taken as input, instead of taking the newline space is taken into 'program'
    }
    //Now, the 'program' variable contains the whole input program as a single string with space/spaces after each lexemes. (Newline is taken as space)
    for(int i=0; i<program.length(); i++)   //loop runs character by character. i is used as index of 'program' string.
    {
        if(program[i] == ' ')                //When a space is found, that means a lexeme is found
        {
            while(program[i] == ' ' && i!=program.length())  //loop for removing extra space
            {
                i++;                                         //i is forwarded until consecutive spaces is found in input.
            }
            //Now i is at a position where a new lexeme starts.
            i--;                                      //i is decremented because in for loop definition i is incremented again. Now i defines a space.
                                                      //After this step, i again defines the start of a lexeme when for loop definition increments i.
            lexemes.push_back(lexeme);                //the lexeme found before the space is added to 'lexemes' vector.
            if(checkKeyword(lexeme))                  //checking if the current lexeme is a keyword.
            {
                types.push_back("Keyword");          //if current lexeme is a keyword, then "Keyword" is inserted into 'types' vector.
            }
            else if(checkOperator(lexeme))           //checking if the current lexeme is a operator.
            {
                types.push_back("Operator");         //if current lexeme is a operators, then "Operator" is inserted into 'types' vector.
            }
            else if(checkSymbol(lexeme))             //checking if the current lexeme is a symbol.
            {
                types.push_back("Symbol");            //if current lexeme is a symbol, then "Symbol" is inserted into 'types' vector.
            }
            else
            {
                if(isNumber(lexeme))                 //checking if the current lexeme is a number(constant- int or real).
                {
                    types.push_back("Constant");     //if current lexeme is a constant, then "Constant" is inserted into 'types' vector.
                }
                else
                    types.push_back("Identifier");   //if current lexeme is not a keyword, operator, symbol or constant,
                                                     //then "Identifier" is inserted into 'types' vector.
            }
            lexeme = "";                         //After saving a lexeme and it's type to 'lexemes' and 'types' vector,
                                                 //'lexeme' variable is cleared for taking the next lexeme.
        }
        else                                   //if the current character is not a space, that means it is a character of a lexeme.
        {
            lexeme += program[i];     //current character is added to 'lexeme' variable to form a lexeme.
                                      //When a space is found, this 'lexeme' variable is used inside if condition.
        }
    }

    //  OUTPUT
    cout<<endl;
    for(int i=0; i<lexemes.size(); i++)
    {
        cout<<lexemes[i]<<" - "<<types[i]<<'\n';   //'lexemes' and 'types' vector are being printed, as output.
    }

    return 0;
}
bool checkKeyword(string str)            //function for checking if a lexeme is a keyword.
{
    for(string key:keywords)            //foreach loop to check match between the lexeme(str) with any elements of 'keywords' array.
    {
        if(key == str)
        {
            return true;               //match is found.
        }
    }
    return false;                      //match is not found.
}
bool checkOperator(string str)        //function for checking if a lexeme is a operator.
{
    for(string key:operators)         //foreach loop to check match between the lexeme(str) with any elements of 'operators' array.
    {
        if(key == str)
        {
            return true;              //match is found.
        }
    }
    return false;                     //match is not found.
}
bool checkSymbol(string str)          //function for checking if a lexeme is a symbol.
{
    for(string key:symbols)           //foreach loop to check match between the lexeme(str) with any elements of 'symbols' array.
    {
        if(key == str)
        {
            return true;              //match is found.
        }
    }
    return false;                     //match is not found.
}
bool isNumber(string str)             //function for checking if a lexeme is a constant.
{
    int length = str.length();
    int k = 0;
    for(char c:str)                   //foreach loop to check if every element of the lexeme(str) is digit.
    {
        if(isdigit(c))               //checking if the current character is a digit.
            k++;                     //if the current character is a digit, then variable 'k' counts it.
        else if(c == '.')            //checking if the current character is a '.'.
            k++;                     //if the current character is a '.', then variable 'k' counts it.
    }
    if(k == length)                 //if every character of the lexeme is digit or '.', than k is equal to the length of the lexeme(str).
        return true;                //Current lexeme(str) is a constant
    return false;                   //Current lexeme(str) is a not constant
}
//PROGRAM ENDS
