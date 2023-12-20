#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <unordered_set>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <stack>
#include <vector>

using namespace std;

bool multiline = false;
string lexeme = "";
int state = 0;
int commentedline[100];
int commentindex=0;
int assignmentline[100];
int assignmentindex=0;
int linestoPrint[100];
int linetoPrintindex=0;
unordered_set<string> keywords = {
    "abstract", "and", "array", "as", "break", "callable", "case", "catch", "class", "clone",
    "const", "continue", "declare", "default", "die", "do", "echo", "else", "elseif", "empty",
    "enddeclare", "endfor", "endforeach", "endif", "endswitch", "endwhile", "eval", "exit", "extends",
    "final", "for", "foreach", "function", "global", "goto", "if", "implements", "include", "include_once",
    "instanceof", "insteadof", "interface", "isset", "list", "namespace", "new", "or", "print", "private",
    "protected", "public", "require", "require_once", "return", "static", "switch", "throw", "trait",
    "try", "unset", "use", "var", "while", "xor", "yield", "yield from"
};

bool is_Letter(char c) {
    return isalpha(c) || c == '_';
}

bool is_Digit(char c) {
    return isdigit(c);
}

bool is_Delimiter(char c) {
    return isspace(c) || c == '\n' || c == '\t';
}

double pow(double a,double b)
{
    double ans=1;
    for(int i=0;i<b;i++)
    {
        ans*=a;
    }
    return ans;
}

bool is_Operator(char c)
{
    return c=='+'||c=='-'||c=='*'||c=='/';
}

int Lexical(string str,int linenum,ofstream &tokenout,ofstream &identiferout)
{    
    int i=0;
    if(!multiline)
    {
        lexeme="";
    }
    else
    {
        commentedline[commentindex]=linenum;
        commentindex++;
        lexeme=lexeme+' ';
    }
    int flag=1;
    char c='\0';

    while(str[i]!='\0')
    {
        c=str[i];
        flag=1;
        switch(state)
        {
        case 0:
            if(is_Letter(c))
            {
                state=1;
            }
            else if(c=='$')
            {
                if(is_Letter(str[i+1]))
                {
                    state=1;
                }
                else
                {
                    cout<<"\033[31mERROR INVALID TOKEN AT LINE : "<<linenum<<"\033[0m"<<endl;
                    flag=0;
                    lexeme="";
                    tokenout.close();
                    ifstream tokenin("tokens.txt");
                    string line;
                    ofstream temp("updatedtokens.txt");
                    while (getline(tokenin, line)) 
                    {
                        if(line=="-----------------------------------"||line=="|        TOKENS GGENERATED        |"||line=="-----------------------------------")
                        {
                            temp<<line<<endl;
                        }
                    }
                    temp.close();
                    tokenin.close();
                    remove("tokens.txt");
                    rename("updatedtokens.txt","tokens.txt");
                    return 1;
                }
            }
            else if(is_Digit(c))
            {
                state=2;
            }
            else if(c=='.')
            {
                state=0;
                lexeme=c;
                tokenout<<"Tokken <"<<" StrConcatinate , "<<lexeme<<" > " <<endl;
                lexeme="";
                flag=0;
            }
            else if(c=='"')
            {
                state=4;
            }
            else if(c=='\'')
            {
                state=9;
            }
            else if(c=='=')
            {
                state=7;
            }
            else if(c=='+')
            {
                state=5;               
            }
            else if(c=='-')
            {
                state=6;                
            }
            else if(c=='*')
            {
                state=19;
            }
            else if(c=='/')
            {
                state=11;
            }
            else if(c=='?')
            {
                state=18;
            }
            else if(c=='<')
            {
                state=3;
            }
            else if(c=='>')
            {
                state=0;
                lexeme=c;
                tokenout<<"Tokken <"<<" CondOperator , "<<lexeme<<" > " <<endl;
                lexeme="";
                flag=0;
            }
            else if(c=='!')
            {
                state=0;
                lexeme=c;
                tokenout<<"Tokken <"<<" UniOperator , "<<lexeme<<" > " <<endl;
                lexeme="";
                flag=0;
            }
            else if(c=='&')
            {
                state=0;
                lexeme=c;
                tokenout<<"Tokken <"<<" UniOperator , "<<lexeme<<" > " <<endl;
                lexeme="";
                flag=0;
            }
            else if(c == ';')
            {
                state=0;
                lexeme=c;
                tokenout<<"Tokken <"<<" SemiColon , "<<lexeme<<" > " <<endl;
                lexeme="";
                flag=0;
            }
            else if(c == '[' || c == '{' || c == '(' || c == ')' || c == '}' || c == ']')
            {
                state=0;
                lexeme=c;
                tokenout<<"Tokken <"<<" Bracket , "<<lexeme<<" > " <<endl;
                lexeme="";
                flag=0;
            }
            else if(is_Delimiter(c))
            {
               flag=0;
            }
            else
            {
                cout<<"\033[31mERROR INVALID TOKEN "<<c<<" AT LINE : "<<linenum<<"\033[0m"<<endl;
                flag=0;
                lexeme="";
                tokenout.close();
                ifstream tokenin("tokens.txt");
                string line;
                ofstream temp("updatedtokens.txt");
                while (getline(tokenin, line)) 
                {
                    if(line=="-----------------------------------"||line=="|        TOKENS GGENERATED        |"||line=="-----------------------------------")
                    {
                        temp<<line<<endl;
                    }
                }
                temp.close();
                tokenin.close();
                remove("tokens.txt");
                rename("updatedtokens.txt","tokens.txt");
                return 1;
            }
        break;

        case 1:
            if(!(is_Letter(c) || is_Digit(c)|| c=='_'))
            {

                state=0;

                if(keywords.count(lexeme) > 0)
                {
                    if(lexeme=="echo")
                    {
                        linestoPrint[linetoPrintindex]=linenum;
                        linetoPrintindex++;
                    }
                    tokenout<<"Tokken <"<<" KeyWord , "<<lexeme<<" > " <<endl;
                }
                else
                {
                    if(lexeme[0]!='$')
                    {
                        if(lexeme=="true"||lexeme=="false")
                        {
                            tokenout<<"Tokken <"<<" Boolean , "<<lexeme<<" > " <<endl;
                        }
                        else
                        {
                            tokenout<<"Tokken <"<<" Function , "<<lexeme<<" > " <<endl;
                        }
                    }
                    else
                    {
                        tokenout<<"Tokken <"<<" Identity , "<<lexeme<<" > " <<endl;
                        ifstream identiferin("identifier.txt");
                        string line;
                        bool found=false;
                        while (getline(identiferin, line)) 
                        {
                            if(line==lexeme)
                            {
                                found=true;
                                break;
                            }
                        }
                        if(!found)
                        {
                            identiferout<<lexeme<<endl;
                        }
                    }
                }
                lexeme="";

                continue;
            }
        break;

        case 2:
            if(!is_Digit(c) && c != '.')
            {
                state=0;
                tokenout<<"Tokken <"<<" IntLiteral , "<<lexeme<<" > " <<endl;
                lexeme="";
                continue;
            }
            if(c=='.')
            {
                state=8;
            }
        break;

        case 3:
            if(c=='=')
            {
                state=0;
                lexeme=lexeme+c;
                tokenout<<"Tokken <"<<" CondOperator , "<<lexeme<<" > " <<endl;
                lexeme="";
                flag=0;
            }
            else if(c=='?')
            {
                state=15;
            }
            else
            {
                state=0;
                tokenout<<"Tokken <"<<" CondOperator , "<<lexeme<<" > " <<endl;
                lexeme="";
            }
        break;

        case 4:
            if(c=='"')
            {
                state=0;
                tokenout<<"Tokken <"<<" StrLiteral , "<<lexeme.substr(1,string::npos)<<" > " <<endl;
                lexeme="";
                flag=0;
            }
        break;

        case 5:
            if(c=='+')
            {
                state=0;
                lexeme=lexeme+c;
                tokenout<<"Tokken <"<<" UniOperator , "<<lexeme<<" > " <<endl;
                lexeme="";
                flag=0;
            }
            else
            {
                state=0;
                tokenout<<"Tokken <"<<" BiOperator , "<<lexeme<<" > " <<endl;
                lexeme="";
                continue;
            }
        break;
        case 6:
            if(c=='-')
            {
                state=0;
                lexeme=lexeme+c;
                tokenout<<"Tokken <"<<" UniOperator , "<<lexeme<<" > " <<endl;
                lexeme="";
                flag=0;
            }
            else
            {
                state=0;
                tokenout<<"Tokken <"<<" BiOperator , "<<lexeme<<" > " <<endl;
                lexeme="";
                continue;
            }
        break;
        case 7:
            if(c=='=')
            {
                state=0;
                lexeme=lexeme+c;
                tokenout<<"Tokken <"<<" CondOperator , "<<lexeme<<" > " <<endl;
                lexeme="";
                flag=0;
            }
            else
            {
                state=0;
                assignmentline[assignmentindex]=linenum;
                assignmentindex++;
                tokenout<<"Tokken <"<<" BiOperator , "<<lexeme<<" > " <<endl;
                lexeme="";
                continue;
            }
        break;
        case 8:
            if(!is_Digit(c))
            {
                state=0;
                tokenout<<"Tokken <"<<" FloatLiteral , "<<lexeme<<" > " <<endl;
                lexeme="";
                continue;
            }
        break;
        case 9:
            state=10;
        break;
        case 10:
            if(c=='\'')
            {
                state=0;
                tokenout<<"Tokken <"<<" CharLiteral , "<<lexeme.substr(1,string::npos)<<" > " <<endl;
                lexeme="";
                flag=0;
            }
        break;
        case 11:
            if(c=='/')
            {
                state=12;
            }
            else if(c=='*')
            {
                commentedline[commentindex]=linenum;
                commentindex++;
                multiline=true;
                state=13;
            }
            else
            {
                state=0;
                tokenout<<"Tokken <"<<" BiOperator , "<<lexeme<<" > " <<endl;
                lexeme="";
                continue;
            }
        break;
        case 12:
            if(str[i+1]=='\0')
            {
                state=0;
                lexeme=lexeme+c;
                commentedline[commentindex]=linenum;
                commentindex++;
                tokenout<<"Tokken <"<<" Comment , "<<lexeme.substr(2,lexeme.length()-2)<<" > " <<endl;
                lexeme="";
                flag=0;
            }
        break;
        case 13:
            if(c=='*')
            {
                state=14;
            }
            else
            {
                state=13;
            }
        break;
        case 14:
            if(c=='/')
            {
                state=0;
                tokenout<<"Tokken <"<<" Comment , "<<lexeme.substr(2,lexeme.length()-3)<<" > " <<endl;
                lexeme="";
                flag=0;
                multiline=false;
            }
            else
            {
                state=13;
            }
        break;
        case 15:
            if(c=='p')
            {
                state=16;
            }
        break;
        case 16:
            if(c=='h')
            {
                state=17;
            }
        break;
        case 17:
            if(c=='p')
            {
                state=0;
                lexeme=lexeme+c;
                tokenout<<"Tokken <"<<" Starting , "<<lexeme<<" > " <<endl;
                lexeme="";
                flag=0;
            }
        break;
        case 18:
            if(c=='>')
            {
                state=0;
                lexeme+=c;
                tokenout<<"Tokken <"<<" Closing , "<<lexeme<<" > " <<endl;
                lexeme="";
                flag=0;
            }
        break;
        case 19:
            if(c=='*')
            {
                state=0;
                lexeme=lexeme+c;
                tokenout<<"Tokken <"<<" BiOperator , "<<lexeme<<" > " <<endl;
                lexeme="";
                flag=0;
            }
            else
            {
                state=0;
                assignmentline[assignmentindex]=linenum;
                assignmentindex++;
                tokenout<<"Tokken <"<<" BiOperator , "<<lexeme<<" > " <<endl;
                lexeme="";
                continue;
            }
        break;
    }
    if(flag)
        lexeme=lexeme+c;

    i++;
    }
    return 0;
}

double evaluateExpression(string expression,int line) {
    stack<double> operandStack;
    stack<string> operatorStack;

    stringstream ss(expression);
    string token;

    while (getline(ss, token, ' ')) {
        if (token[0]=='$')
        {
            token=token.substr(1,string::npos);
            ifstream identiferin("symbol.txt");
            string line2;
            bool found=false;
            while (getline(identiferin, line2)) 
            {
                string tlex,ttype,tlexval;
                stringstream ss(line2);
                getline(ss,tlex,',');
                getline(ss,ttype,',');
                getline(ss,tlexval,',');
                if(tlex==token&&(ttype=="Int"||ttype=="Float"))
                {
                    found=true;
                    token=tlexval;
                    break;
                }
            }
            identiferin.close();
            if(!found)
            {
                cout << "\033[31mERROR VARIABLE "<< token << " NOT DECLARED AT LINE : "<<line <<"\033[0m"<<endl;
                return 0.0;
            }
            else
            {
                operandStack.push(stod(token));
            }
        }
        else if (is_Digit(token[0]) || (token[0] == '-' && is_Digit(token[1]))) {
            operandStack.push(stod(token));
        } else if (is_Operator(token[0])) {
            while (!operatorStack.empty() && operatorStack.top() != "(" && ((token == "+" || token == "-") && (operatorStack.top() == "*" || operatorStack.top() == "/" || operatorStack.top() == "**")) || (token == "*" && (operatorStack.top() == "/" || operatorStack.top() == "**")) || (token == "/" && operatorStack.top() == "**")) {
                string op = operatorStack.top();
                operatorStack.pop();

                double operand2 = operandStack.top();
                operandStack.pop();

                double operand1 = operandStack.top();
                operandStack.pop();

                if (op == "+") {
                    operandStack.push(operand1 + operand2);
                } else if (op == "-") {
                    operandStack.push(operand1 - operand2);
                } else if (op == "*") {
                    operandStack.push(operand1 * operand2);
                } else if (op == "**") {
                    operandStack.push(pow(operand1,operand2));
                } else if (op == "/") {
                    if (operand2 != 0) {
                        operandStack.push(operand1 / operand2);
                    } else {
                        cout << "\033[31mERROR DIVISION BY 0 AT LINE : "<<line <<"\033[0m"<< endl;
                        return 0.0;
                    }
                }
            }
            string tmp="";
            if(token[0]=='*'&&token[1]=='*')
            {
                tmp="**";
            }
            else
            {
                tmp=token[0];
            }
            operatorStack.push(tmp);
        } else if (token[0] == '(') {
            operatorStack.push("(");
        } else if (token[0] == ')') {
            while (!operatorStack.empty() && operatorStack.top() != "(") {
                // Pop operators and perform operations until '(' is encountered
                string op = operatorStack.top();
                operatorStack.pop();

                double operand2 = operandStack.top();
                operandStack.pop();

                double operand1 = operandStack.top();
                operandStack.pop();

                if (op == "+") {
                    operandStack.push(operand1 + operand2);
                } else if (op == "-") {
                    operandStack.push(operand1 - operand2);
                } else if (op == "*") {
                    operandStack.push(operand1 * operand2);
                } else if (op == "**") {
                    operandStack.push(pow(operand1,operand2));
                } else if (op == "/") {
                    if (operand2 != 0) {
                        operandStack.push(operand1 / operand2);
                    } else {
                        cout << "\033[31mERROR DIVISION BY 0 AT LINE : "<<line <<"\033[0m"<< endl;
                        return 0.0;
                    }
                }
            }

            if (!operatorStack.empty() && operatorStack.top() == "(") {
                // Pop '('
                operatorStack.pop();
            } else {
                cout << "\033[31mERROR MISMATCHED PARANTHESIS AT LINE : " <<line <<"\033[0m"<< endl;
                return 0.0;
            }
        } else {
            cout << "\033[31mERROR INVALID TOKEN AT LINE : " <<line <<"\033[0m"<< endl;
            return 0.0;
        }
    }

    // Pop remaining operators and perform operations
    while (!operatorStack.empty()) {
        string op = operatorStack.top();
        operatorStack.pop();

        double operand2 = operandStack.top();
        operandStack.pop();

        double operand1 = operandStack.top();
        operandStack.pop();

        if (op == "+") {
            operandStack.push(operand1 + operand2);
        } else if (op == "-") {
            operandStack.push(operand1 - operand2);
        } else if (op == "*") {
            operandStack.push(operand1 * operand2);
        } else if (op == "**") {
            operandStack.push(pow(operand1,operand2));
        } else if (op == "/") {
            if (operand2 != 0) {
                operandStack.push(operand1 / operand2);
            } else {
                cout << "\033[31mERROR DIVISION BY 0 AT LINE : "<<line <<"\033[0m"<< endl;
                return 0.0;
            }
        }
    }

    // Result should be on the operand stack
    if (!operandStack.empty()) {
        return operandStack.top();
    } else {
        cout << "\033[31mEMPTY OPERAND STACK\033[0m" << endl;
        return 0.0;
    }
}

string evaluateBool(string rhs,int line)
{
    if(rhs=="true")
    {
        return "1";
    }
    if(rhs=="false")
    {
        return "0";
    }
    stringstream ss(rhs);
    vector<string> tokens;
    string token;
    while(getline(ss,token,' '))
    {
        tokens.push_back(token);
    }
    if(tokens.size()!=3)
    {
        cout<<"ERROR NOT A VALID BOOLEAN EXPRESSION AT LINE : "<<line<<endl;
        return "!";
    }
    double operand1,operand2;
    double operands[2];
    int i=0;
    int j=0;
    do
    {

    if (tokens[j][0]=='$')
    {
        token=tokens[j].substr(1,string::npos);
        ifstream identiferin("symbol.txt");
        string line2;
        bool found=false;
        while (getline(identiferin, line2)) 
        {
            string tlex,ttype,tlexval;
            stringstream ss(line2);
            getline(ss,tlex,',');
            getline(ss,ttype,',');
            getline(ss,tlexval,',');
            if(tlex==token&&(ttype=="Int"||ttype=="Float"))
            {
                found=true;
                token=tlexval;
                break;
            }
        }
        identiferin.close();
        if(!found)
        {
            cout << "\033[31mERROR VARIABLE "<<tokens[j]<<" NOT DECLARED AT LINE : "<<line <<"\033[0m"<<endl;
            return "!";
        }
        else
        {
            operands[i]=stof(token);
        }
    }
    else if(is_Digit(tokens[j][0]))
    {
        operands[i]=stof(tokens[j]);
    }
    i++;
    j+=2;
    } while (i<2);
    
    string op = tokens[1];
    if (op == "<") {
        return operands[0] < operands[1] ? "1" : "0";;
    } else if (op == ">") {
        return operands[0] > operands[1] ? "1" : "0";;
    } else if (op == "<=") {
        return operands[0] <= operands[1] ? "1" : "0";;
    } else if (op == ">=") {
        return operands[0] >= operands[1] ? "1" : "0";;
    } else if (op == "==") {
        return operands[0] == operands[1] ? "1" : "0";;
    } else if (op == "!=") {
        return operands[0] != operands[1] ? "1" : "0";;
    } else {
        cout << "\033[31mERROR UNKNOWN OPERATOR "<<op<<" AT LINE " << line <<"\033[0m"<< endl;
        return "!";
    }
    
}

int SymbolTable(string line,int linenum)
{
    ofstream sybolout("symbol.txt",ios::app);
    int i=0,j;
    char c='\0';
    string lexeme="",lexemeval="",type,rightside;
    for(int j=0;j<commentindex;j++)
    {
        if(linenum==commentedline[j])
        {
            return 0;
        }
    }
    for(int j=0;j<assignmentindex;j++)
    {
        if(linenum==assignmentline[j])
        {
            while(line[i]!='\0')
            {
                c=line[i];
                if(c=='$')
                {
                    if(is_Letter(line[i+1]))
                    {
                        i++;
                        while(is_Letter(line[i]) || is_Digit(line[i]) || line[i]=='_')
                        {
                            lexeme=lexeme+line[i];
                            i++;
                        }
                        if(line[i]=='=')
                        {
                            i++;
                            j=i;
                            rightside=line.substr(j,string::npos-1);
                            bool found=false;
                            for(int k=0;k<rightside.length();k++)
                            {
                                if(rightside[k]=='+' || rightside[k]=='-' || rightside[k]=='*' || rightside[k]=='/')
                                {
                                    found=true;
                                    break;
                                }
                            }
                            bool isBool=false;
                            if(rightside=="true"||rightside=="false")
                            {
                                isBool=true;
                            }
                            for(int k=0;k<rightside.length();k++)
                            {
                                if(rightside[k]=='=' && rightside[k+1]=='=')
                                {
                                    isBool=true;
                                    break;
                                }
                                if(rightside[k]=='!' && rightside[k+1]=='=')
                                {
                                    isBool=true;
                                    break;
                                }
                                if(rightside[k]=='<' || (rightside[k]=='<' && rightside[k+1]=='='))
                                {
                                    isBool=true;
                                    break;
                                }
                                if(rightside[k]=='>' || (rightside[k]=='>' && rightside[k+1]=='='))
                                {
                                    isBool=true;
                                    break;
                                }
                            }
                            if(!found&&isBool)
                            {
                                string result=evaluateBool(rightside,linenum);
                                if(result=="!")
                                {
                                    return 1;
                                }
                                lexemeval=result;
                                type="Boolean";
                            }
                            if(found&&!isBool)
                            {
                                double result = evaluateExpression(rightside,linenum);
                                if(result==0.0)
                                {
                                    return 1;
                                }
                                lexemeval=to_string(result);
                                if(lexemeval.find('.')!=string::npos)
                                {
                                    for(int k=lexemeval.length()-1;k>=0;k--)
                                    {
                                        if(lexemeval[k]=='0')
                                        {
                                            lexemeval=lexemeval.substr(0,k);
                                        }
                                        else if(lexemeval[k]=='.')
                                        {
                                            lexemeval=lexemeval.substr(0,k);
                                            break;
                                        }
                                        else
                                        {
                                            break;
                                        }
                                    }
                                }
                                if(lexemeval.find('.')!=string::npos)
                                {
                                    type="Float";
                                }
                                else
                                {
                                    type="Int";
                                }
                            }
                            if(line[i]=='"'&&!found&&!isBool)
                            {
                                i++;
                                while(line[i]!='"' && line[i]!='\0')
                                {
                                    lexemeval=lexemeval+line[i];
                                    type="String";
                                    i++;
                                }
                                if(line[i]=='"')
                                {
                                    i++;
                                }
                                else
                                {
                                    cout<<"\033[31mERROR STRING NOT CLOSED AT LINE : "<<linenum<<"\033[0m"<<endl;
                                    return 1;
                                }
                            }
                            else if(is_Digit(line[i])&&!found&&!isBool)
                            {
                                while(is_Digit(line[i]))
                                {
                                    lexemeval=lexemeval+line[i];
                                    type="Int";
                                   i++;
                                }
                                if(line[i]=='.')
                                {
                                   lexemeval=lexemeval+line[i];
                                    type="Float";
                                    i++;
                                    while(is_Digit(line[i]))
                                   {
                                        lexemeval=lexemeval+line[i];
                                        i++;
                                    }
                               }
                            }
                            else if(line[i]=='$'&&!found&&!isBool)
                            {
                                i++;
                                while(is_Letter(line[i]) || is_Digit(line[i]) || line[i]=='_')
                                {
                                    lexemeval=lexemeval+line[i];
                                    i++;
                                }
                                ifstream identiferin("symbol.txt");
                                string line2;
                                bool found=false;
                                while (getline(identiferin, line2)) 
                                {
                                    string tlex,ttype,tlexval;
                                    stringstream ss(line2);
                                    getline(ss,tlex,',');
                                    getline(ss,ttype,',');
                                    getline(ss,tlexval,',');
                                    if(tlex==lexemeval)
                                    {
                                        found=true;
                                        lexemeval=tlexval;
                                        type=ttype;
                                        break;
                                    }
                                }
                                identiferin.close();
                                if(!found)
                                {
                                    cout<<"\033[31mERROR VARIABLE $"<<lexemeval<<" NOT DECLARED AT LINE : "<<linenum<<"\033[0m"<<endl;
                                    return 1;
                                }
                            }
                            
                        }
                        ifstream identiferin("symbol.txt");
                        string line2;
                        bool found=false;
                        while (getline(identiferin, line2)) 
                        {
                            string tlex,ttype,tlexval;
                            stringstream ss(line2);
                            getline(ss,tlex,',');
                            getline(ss,ttype,',');
                            getline(ss,tlexval,',');
                            if(tlex==lexeme)
                            {
                                found=true;
                                break;
                            }
                        }
                        identiferin.close();
                        if(found)
                        {
                            sybolout.close();
                            ifstream identiferin("symbol.txt");
                            string line2;
                            ofstream temp("updatedsymbol.txt");
                            while (getline(identiferin, line2)) 
                            {
                                string tlex,ttype,tlexval,tline;
                                stringstream ss(line2);
                                getline(ss,tlex,',');
                                getline(ss,ttype,',');
                                getline(ss,tlexval,',');
                                getline(ss,tline,'\n');
                                if(tlex!=lexeme)
                                {
                                    temp<<line2<<endl;
                                }
                                else
                                {
                                    temp<<lexeme<<","<<type<<","<<lexemeval<<","<<linenum<<endl;
                                }
                            }
                            temp.close();
                            identiferin.close();
                            remove("symbol.txt");
                            rename("updatedsymbol.txt","symbol.txt");
                        }
                        else
                        {
                            sybolout<<lexeme<<","<<type<<","<<lexemeval<<","<<linenum<<endl;
                        }
                    }
                }
                i++;
            }
        }
    }
    return 0;
}

string getVariableValue(const string &variable,int linenum)
{
    ifstream identifierin("symbol.txt");
    string line;
    bool found = false;
    while (getline(identifierin, line))
    {
        string tlex, ttype, tlexval;
        stringstream ss(line);
        getline(ss, tlex, ',');
        getline(ss, ttype, ',');
        getline(ss, tlexval, ',');
        if (tlex == variable)
        {
            found = true;
            identifierin.close();
            return tlexval;
        }
    }
    identifierin.close();

    if (!found)
    {
        cout << "\033[31mERROR VARIABLE $"<<variable<<" NOT DECLARED AT LINE : "<<linenum<<"\033[0m" << endl;
        return "~";
    }

    return "";
}
vector<string> printing;
int PrintingOutputs(string line,int linenum)
{
    for(int j=0;j<commentindex;j++)
    {
        if(linenum==commentedline[j])
        {
            return 0;
        }
    }
    for(int i=0;i<linetoPrintindex;i++)
    {
        if(linestoPrint[i]==linenum)
        {
            int j=0;
            while(line[j]==' ')
            {
                j++;
            }
            line=line.substr(j,string::npos);
            line=line.substr(4,string::npos);
            while(line[0]==' ')
            {
                line=line.substr(1,string::npos);
            }
            while(line[line.length()-1]==' ')
            {
                line=line.substr(0,line.length()-1);
            }
            size_t pos;
            while ((pos = line.find("$")) != string::npos)
            {
                size_t end_pos = line.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_", pos + 1);
                string variable = line.substr(pos, end_pos - pos);
                variable=variable.substr(1,string::npos);
                line.replace(pos, end_pos - pos, getVariableValue(variable,linenum));
            }
            for(int i=0;i<line.length();i++)
            {
                if(line[i]=='~')
                {
                    cout << "\033[31mERROR CANNOT ECHO AT LINE : "<<linenum<<"\033[0m" << endl;
                    return 1;
                }
            }
            if(line[0]=='"')
            {
                line=line.substr(1,string::npos);
                line=line.substr(0,line.length()-1);
            }
            string temp="";
            for(int i=0;i<line.length();i++)
            {
                if(line[i]=='"' && line[i+1]=='.' && line[i+2]=='"')
                {
                    i+=3;
                }
                else if(line[i]=='.' && line[i+1]=='"')
                {
                    i+=2;
                }
                else if(line[i]=='"'&&line[i+1]=='.')
                {
                    i+=2;
                }

                temp=temp+line[i];
            }
            if(temp[temp.length()-1]=='"')
            {
                temp=temp.substr(0,temp.length()-1);
            }
            line=temp;
            printing.push_back(line);
        }
    }
    return 0;
}

void cleanfile(const string filename)
{
    ofstream temp("temp.txt");
    ifstream file(filename);
    temp.close();
    file.close();
    remove(filename.c_str());
    rename("temp.txt",filename.c_str());
}

int main() {
    int lexres=0,symbolres=0,printres=0,evalres=0;
    ifstream file("source.txt");
    if (!file) 
    {
        cout << "Unable to open Input File." << endl;
        return 1;
    }
    ofstream tokenout("tokens.txt");
    ofstream identiferout("identifier.txt");
    tokenout<<"-----------------------------------"<<endl;
    tokenout<<"|        TOKENS GGENERATED        |"<<endl;
    tokenout<<"-----------------------------------"<<endl; 
    identiferout<<"-------------------"<<endl;
    identiferout<<"|   IDENTIFIERS   |"<<endl;
    identiferout<<"-------------------"<<endl;
    string line;
    int linenum=0;
    while (getline(file, line)) {
        linenum++;
        lexres=Lexical(line,linenum,tokenout,identiferout);
        if(lexres==1)
        {
            cout<<"-----------------------------------"<<endl;
            cout << "\033[31mCode Cannot be Compiled!\033[0m" << endl;
            cout<<"-----------------------------------"<<endl;
            cleanfile("tokens.txt");
            cleanfile("identifier.txt");
            cleanfile("symbol.txt");
            system("pause");
            return 1;
        }        
    }
    tokenout.close();
    identiferout.close();
    file.close();

    const char* command = "a.exe";
    FILE* pipe = _popen(command, "r");
    if (!pipe) {
        cout << "popen() failed!" << endl;
        return EXIT_FAILURE;
    }
    char buffer[128];
    string result = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != nullptr)
            result += buffer;
    }
    _pclose(pipe);
    

    for(int i=0;i<result.length()-1;i++)
    {
        if(result[i]=='!')
        {
            evalres=1;
        }
    }

    ifstream file2("source.txt");
    if (!file2) 
    {
        cout << "Unable to open Input File." << endl;
        return 1;
    }
    linenum=0;
    while (getline(file2, line)) {
        linenum++;
        symbolres = SymbolTable(line,linenum); 
        if(symbolres==1)
        {
            cout<<"-----------------------------------"<<endl;
            cout << "\033[31mCode Cannot be Compiled!\033[0m" << endl;
            cout<<"-----------------------------------"<<endl;
            cleanfile("symbol.txt");
            cleanfile("tokens.txt");
            cleanfile("identifier.txt");
            system("pause");
            return 1;
        }
    }
    file2.close();

    ifstream file3("source.txt");
    if (!file3) 
    {
        cout << "Unable to open Input File." << endl;
        return 1;
    }
    linenum=0;
    while (getline(file3, line)) {
        linenum++;
        printres = PrintingOutputs(line,linenum); 
        if(printres==1)
        {
            cout<<"-----------------------------------"<<endl;
            cout << "\033[31mCode Cannot be Compiled!\033[0m" << endl;
            cout<<"-----------------------------------"<<endl;
            cleanfile("symbol.txt");
            cleanfile("tokens.txt");
            cleanfile("identifier.txt");
            system("pause");
            return 1;
        }
    }
    file3.close();

    if(lexres==0&&symbolres==0&&printres==0&&evalres==0)
    {
        cout<<"-----------------------------------"<<endl;
        cout << result << endl;
        cout<<"-----------------------------------"<<endl;
        for(int i=0;i<printing.size();i++)
        {
            cout<<printing[i]<<endl;
        }
    }
    else
    {
        cout<<"-----------------------------------"<<endl;
        cout << result << endl;
        cout<<"-----------------------------------"<<endl;
        cleanfile("symbol.txt");
        cleanfile("tokens.txt");
        cleanfile("identifier.txt");
    }

    system("pause");
    return 0;
}
