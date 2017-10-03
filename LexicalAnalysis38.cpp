/**
"Lexical Analysis"
Compiler Design Laboratory
Md. Farhan Sadique
Student ID: 130238
**/
/**
Input: Source program with or without space after each lexeme. There can be comments in input. Input needs to be a valid program. Input ends when 'eof' is given after a newline.
Output: List of Lexemes with their token name and attribute value and symbol table.
**C++11 required to compile the program**
**/
#include<iostream>
#include<string>
#include<vector>
using namespace std;
void checkToken();
void checkSingleToken();
void tokensCalculation();
void symbolTableCalculation();
bool checkKeyword(string str);
bool checkOperator(string str);
bool checkSymbol(string str);
bool isNumber(string str);

string keywords[] = { "bool", "char", "int", "long", "float", "double", "void", "return", "if", "else", "while", "for",
                      "break", "switch", "case", "cin", "cout","string"
                    };
string operators[] = { "+", "-", "*", "/", "++", "--", "=", "+=", "-=", "*=", "/=", "==", "!=", ">", "<", ">=", "<=", "&&", "||", "<<", ">>"
                     };
string symbols[] = { "(", ")", "{", "}", ";","," };

string program = "";    //for containing the input program as a single string.
string lexeme = "";     //for maintaining operation with each lexeme.
string x;               //for storing each line of input program
vector<string> lexemes;     //for saving the lexemes.
vector<string> tokenName;     //for saving the token name of the lexemes
vector<string> attributeValue;    //for saving the attribute value of the lexemes
vector<string> symbolTableSymbol;   //for saving the symbols of the symbol table
vector<string> symbolTableDataType;     //for saving the data type of the symbols

int lexemeFound = 0;
string tempLexeme[2];       //this program starts processing of lexeme when 2 lexemes are found. So, a array of size=2 is taken for saving previous 2 lexemes.
string tempId = "";
string singleCharLexeme = "";
int flag = 0;

int main()
{
    cout<<"Source Program:"<<endl;
    /**input starts**/
    while (getline(cin, x))   //takes a string input with/without spaces. When newline is given, body of the loop executes.
    {
        int commentIndex = -1;       //for saving starting index of a comment like this  //......
        int commentStartIndex = -1;   //for saving starting index of a comment like this /*......*/
        int commentEndIndex = -1;      //for saving end index of a comment like this /*......*/
        if (x == "eof")                 //When eof is given as a input, input ends.
            break;
        //starting index of comment calculation. comment like  //.....
        for (int i = 0; i<x.size() - 1; i++)
        {
            if (x[i] == '/' && x[i + 1] == '/')
            {
                commentIndex = i;              //if comment is present in current line(x), than the starting index of the comment is saved.
                break;
            }
        }
        //starting and end index of comment calculation. comment like  /*........*/
        for (int i = 0; i<x.size() - 1; i++)
        {
            if (x[i] == '/' && x[i + 1] == '*')
            {
                commentStartIndex = i;                  //if comment is present in current line(x), than the starting index of the comment is saved.  /*...*/
                for (int j = i + 2; x.size() - 1; j++)
                {
                    if (x[j] == '*' && x[j + 1] == '/')
                    {
                        commentEndIndex = j + 1;        //end index of the comment is saved.  /*....*/
                        i = x.size();                   //i becomes the end index of the current program line(x). Because of ending outer for loop.
                        break;
                    }
                }
            }
        }
        //comment found starting with // and it's removal process
        if (commentIndex != -1)
        {
            for (int j = 0; j<commentIndex; j++)
            {
                program += x[j];                  //current line is added to program until the starting index of comment(//...) begin.
            }

        }
        //comment found starting with /* and ending with */ and it's removal process
        else if (commentStartIndex != -1)
        {
            for (int j = 0; j<commentStartIndex; j++)
            {
                program += x[j];               //current line is added to program until the starting index of comment(/*...*/) begin.
            }
            if (commentEndIndex != -1)
            {
                for (int j = commentEndIndex + 1; j<x.size(); j++)
                {
                    program += x[j];            //current line is added to program from the [end index of comment(/*...*/)+1] to the last index of the line.
                }
            }
            //comment started with /* but there is no end of the comment with */
            else
            {
                cout << "Compile Error!";
            }
        }
        //no comment in the line, so total line taken
        else
        {
            program += x;
        }
        program += " "; //space added among the lines instead of newline.
    }
    /**input ends**/

    /**token calculation starts**/
    for (int i = 0; i<program.length(); i++)
    {
        //removing extra spaces if there is
        if (program[i] == ' ')  //When a space is found, that means a lexeme is found
        {
            while (program[i] == ' ' && i != program.length())      //loop for removing extra space
            {
                i++;            //i is forwarded until consecutive spaces is found in program.
            }
            //Now i is at a position where a new lexeme starts
            i--;            //i is decremented because in for loop definition i is incremented again. Now i defines a space.
        }
        //no space, single character is added to lexeme and singleCharLexeme
        else
        {
            singleCharLexeme += program[i];   //singleCharLexeme contains only a single character.
            lexeme += program[i];             //lexeme contains the whole lexeme.
        }
        //checking the lexeme(immediately after a identifier) for separating the identifier as a different lexeme
        if ((checkKeyword(singleCharLexeme) || checkOperator(singleCharLexeme) || checkSymbol(singleCharLexeme)) && lexeme.size()>1)
        {
            for (int x = 0; x<lexeme.size() - 1; x++)
            {
                tempId += lexeme[x];       //lexeme(identifier) is added to tempId without the last character(which is a new lexeme)
            }
            flag = 1;            //flag turns to 1 to indicate a lexeme(identifier) is found.
            i--;                 //i is decremented for getting the last character(new lexeme after the identifier) again in next turn of the loop.
            lexeme = tempId;     //tempId(identifier) is added as a lexeme
            tempId="";
        }
        singleCharLexeme = "";  //empty the singleCharLexeme for taking next character

        //if a lexeme is found
        if (checkKeyword(lexeme) || checkOperator(lexeme) || checkSymbol(lexeme) || flag == 1)
        {
            flag = 0;
            //if number of found lexemes is less than 2, than the current lexeme added to templexeme array.
            if (lexemeFound<2)
            {
                tempLexeme[lexemeFound] = lexeme;
                lexemeFound++;
            }
            //if number of found lexemes is equal to 2, than the previous 2 lexemes are checked for token calculation
            else
            {
                checkToken();   //checking if the lexeme is a keyword or operator or symbol or identifier or number.
            }
            lexeme = "";     //empty the lexeme for taking next lexeme
        }
    }
    //after calculating all lexemes, there can be some lexemes in templexeme array. Because in previous loop token is calculated only when lexemeFound>2.
    if (lexemeFound>0)
    {
        //one lexeme remains
        if(lexemeFound == 1)
        {
            checkSingleToken();  //checking if the lexeme is a keyword or operator or symbol or identifier or number.
        }
        //2 lexeme remains
        else if (lexemeFound == 2)
        {
            checkToken();   //checking if the lexeme is a keyword or operator or symbol or identifier or number.
        }
        //last lexeme of the 2 lexeme
        if (lexemeFound == 2)
        {
            checkSingleToken();  //checking if the lexeme is a keyword or operator or symbol or identifier or number.
        }
    }
    symbolTableCalculation();  //symbol table for output
    tokensCalculation();       //Tokens for output

    /**Output**/
    cout <<"\n\nTokens:"<<endl;
    cout <<"\nLexemes-Token Name-Attribute Value\n"<<endl;
    for (int i = 0; i<lexemes.size(); i++)
    {
        cout << lexemes[i] << " - " << tokenName[i] << " - " << attributeValue[i]<<endl;
    }
    cout <<"\n\nSymbol Table:"<<endl;
    cout <<"\nSymbol-Token-DataType-Pointer to symbol table entry\n"<<endl;
    for (int i = 0; i<symbolTableSymbol.size(); i++)
    {
        cout << symbolTableSymbol[i] << " - " << "id" << " - " << symbolTableDataType[i]<<" - "<<i<<endl;
    }
    return 0;
}

//function for checking a lexeme to calculate token
void checkToken()
{
    //if the previous 2 lexemes combinedly generate a lexeme like 2 lexemes '+' and '+' combinedly makes '++'(a new lexeme).
    if (checkKeyword(tempLexeme[0] + tempLexeme[1]))
    {
        lexemes.push_back(tempLexeme[0] + tempLexeme[1]);
        tokenName.push_back("keyword");
        lexemeFound = 1;                  //'lexeme' variable contains a new lexeme, so lexemeFound turns to 1.
        tempLexeme[0] = lexeme;         //The new lexeme is added to tempLexeme array at index=0.
    }
    else if (checkOperator(tempLexeme[0] + tempLexeme[1]))
    {
        lexemes.push_back(tempLexeme[0] + tempLexeme[1]);
        tokenName.push_back("operator");
        lexemeFound = 1;
        tempLexeme[0] = lexeme;
    }
    else if (checkSymbol(tempLexeme[0] + tempLexeme[1]))
    {
        lexemes.push_back(tempLexeme[0] + tempLexeme[1]);
        tokenName.push_back("special symbol");
        lexemeFound = 1;
        tempLexeme[0] = lexeme;
    }
    //if among the previous 2 lexemes, first lexeme is a valid lexeme itself without combining with the 2nd lexeme.
    else if (checkKeyword(tempLexeme[0]))
    {
        lexemes.push_back(tempLexeme[0]);
        tokenName.push_back("keyword");
        lexemeFound = 2;          //templexeme contains a lexeme which is not calculated yet and 'lexeme' variable contains a new lexeme. So, it is 2.
        tempLexeme[0] = tempLexeme[1];  //lexeme at 0 index is calculated, so lexeme at 1 index goes to 0 index
        tempLexeme[1] = lexeme;       //new lexeme is added to index=1
    }
    else if (checkOperator(tempLexeme[0]))
    {
        lexemes.push_back(tempLexeme[0]);
        tokenName.push_back("operator");
        lexemeFound = 2;
        tempLexeme[0] = tempLexeme[1];
        tempLexeme[1] = lexeme;
    }
    else if (checkSymbol(tempLexeme[0]))
    {
        lexemes.push_back(tempLexeme[0]);
        tokenName.push_back("special symbol");
        lexemeFound = 2;
        tempLexeme[0] = tempLexeme[1];
        tempLexeme[1] = lexeme;
    }
    else if (isNumber(tempLexeme[0]))
    {
        lexemes.push_back(tempLexeme[0]);
        tokenName.push_back("number");
        lexemeFound = 2;
        tempLexeme[0] = tempLexeme[1];
        tempLexeme[1] = lexeme;
    }
    else
    {
        lexemes.push_back(tempLexeme[0]);
        tokenName.push_back("id");
        lexemeFound = 2;
        tempLexeme[0] = tempLexeme[1];
        tempLexeme[1] = lexeme;
    }
}
//function for checking lexeme which is not a combining one(like 2 lexemes '+' and '+' combinedly makes '++'(a new lexeme))
void checkSingleToken()
{
    if (checkKeyword(tempLexeme[0]))
    {
        lexemes.push_back(tempLexeme[0]);
        tokenName.push_back("keyword");
    }
    else if (checkOperator(tempLexeme[0]))
    {
        lexemes.push_back(tempLexeme[0]);
        tokenName.push_back("operator");
    }
    else if (checkSymbol(tempLexeme[0]))
    {
        lexemes.push_back(tempLexeme[0]);
        tokenName.push_back("special symbol");
    }
    else if (isNumber(tempLexeme[0]))
    {
        lexemes.push_back(tempLexeme[0]);
        tokenName.push_back("number");
    }
    else
    {
        lexemes.push_back(tempLexeme[0]);
        tokenName.push_back("id");
    }
}

//function for checking if a lexeme is a keyword
bool checkKeyword(string str)
{
    for (string key : keywords)  //foreach loop runs for every value of 'keywords' array.[it exists in C++11]
    {
        if (key == str)
        {
            return true;
        }
    }
    return false;
}
//function for checking if a lexeme is a operator
bool checkOperator(string str)
{
    for (string key : operators)
    {
        if (key == str)
        {
            return true;
        }
    }
    return false;
}
//function for checking if a lexeme is a symbol
bool checkSymbol(string str)
{
    for (string key : symbols)
    {
        if (key == str)
        {
            return true;
        }
    }
    return false;
}
//function for checking if a lexeme is a number
bool isNumber(string str)
{
    int length = str.length();
    int k = 0;
    for (char c : str)     //foreach loop to check if every element of the lexeme(str) is digit.
    {
        if (isdigit(c))    //checking if the current character is a digit.
            k++;            //if the current character is a digit, then variable 'k' counts it.
        else if (c == '.')  //checking if the current character is a '.'.
            k++;            //if the current character is a '.', then variable 'k' counts it.
    }
    if (k == length)        //if every character of the lexeme is digit or '.', than k is equal to the length of the lexeme(str).
        return true;
    return false;
}
//function for symbol table calculation
void symbolTableCalculation()
{
    for(int i=0; i<lexemes.size(); i++)
    {
        flag = 0;
        if(tokenName[i] == "id")
        {
            //checking if current id is previously found or not
            for(int j=0; j<symbolTableSymbol.size(); j++)
            {
                if(lexemes[i] == symbolTableSymbol[j])
                    flag = 1;
            }
            //if current id is not found previously.
            if(flag == 0)
            {
                symbolTableSymbol.push_back(lexemes[i]);
                //if it is not a function name. There is a '(' after a function name and before a function name there is a 'keyword'
                if(lexemes[i+1] != "(" && tokenName[i-1]=="keyword")
                {
                    symbolTableDataType.push_back(lexemes[i-1]);    //a variable's data type is it's previous lexeme (int temp; where int is the data type of temp)
                }
                //variable separated by comma
                else if(lexemes[i-1] == ",")
                {
                    symbolTableDataType.push_back(symbolTableDataType[symbolTableDataType.size()-1]); //the previous variable's data type = current variable's data type.
                }
                //it is a function name
                else
                {
                    symbolTableDataType.push_back("_");  //function name has no data type.
                }
            }
        }
    }
}
//function for Tokens calculation
void tokensCalculation()
{
    for(int i=0; i<lexemes.size(); i++)
    {
        if(tokenName[i] == "keyword")
        {
            tokenName[i] = lexemes[i];      //if it is a keyword, than it's tokenName is itself.
            attributeValue.push_back(" - "); //if it is a keyword, than it does not have a attributeValue.
        }
        else if(tokenName[i] == "number")
        {
            attributeValue.push_back("constant");  //if it is a number, than it's tokenName is number and attributeValue is constant.
        }
        else if(tokenName[i] == "id")
        {
            if(lexemes[i+1] != "(")  //if it is an variable, than it's tokenName is id and attributeValue is "pointer to symbol table entry".
                attributeValue.push_back("pointer to symbol table entry");
            else                    //if it is a function name, than it's tokenName is id and attributeValue is empty.
                attributeValue.push_back(" - ");
        }
        else if(tokenName[i] == "operator")  //giving every operator it's own name as attribute value.
        {
            if(lexemes[i] == "+")
                attributeValue.push_back("addition");
            else if(lexemes[i] == "-")
                attributeValue.push_back("subtraction");
            else if(lexemes[i] == "*")
                attributeValue.push_back("multiplication");
            else if(lexemes[i] == "/")
                attributeValue.push_back("division");
            else if(lexemes[i] == "++")
                attributeValue.push_back("increment");
            else if(lexemes[i] == "--")
                attributeValue.push_back("decrement");
            else if(lexemes[i] == "=")
                attributeValue.push_back("assignment");
            else if(lexemes[i] == "+=")
                attributeValue.push_back("addition and assignment");
            else if(lexemes[i] == "-=")
                attributeValue.push_back("subtraction and assignment");
            else if(lexemes[i] == "*=")
                attributeValue.push_back("multiplication and assignment");
            else if(lexemes[i] == "/=")
                attributeValue.push_back("division and assignment");
            else if(lexemes[i] == "==")
                attributeValue.push_back("Relation Operator");
            else if(lexemes[i] == "!=")
                attributeValue.push_back("Relation Operator");
            else if(lexemes[i] == ">")
                attributeValue.push_back("Relation Operator");
            else if(lexemes[i] == "<")
                attributeValue.push_back("Relation Operator");
            else if(lexemes[i] == ">=")
                attributeValue.push_back("Relation Operator");
            else if(lexemes[i] == "<=")
                attributeValue.push_back("Relation Operator");
            else if(lexemes[i] == "&&")
                attributeValue.push_back("Relation Operator");
            else if(lexemes[i] == "||")
                attributeValue.push_back("Relation Operator");
            else if(lexemes[i] == "<<")
                attributeValue.push_back("Stream extraction operator");
            else if(lexemes[i] == ">>")
                attributeValue.push_back("Stream insertion operator");
        }
        else if(tokenName[i] == "special symbol")  //giving every symbol it's own name as attribute value.
        {
            if(lexemes[i] == "(")
                attributeValue.push_back("opening braces");
            else if(lexemes[i] == ")")
                attributeValue.push_back("closing braces");
            else if(lexemes[i] == "{")
                attributeValue.push_back("left curly braces");
            else if(lexemes[i] == "}")
                attributeValue.push_back("right curly braces");
            else if(lexemes[i] == ";")
                attributeValue.push_back("semicolon");
            else if(lexemes[i] == ",")
                attributeValue.push_back("comma");
        }
    }
}
