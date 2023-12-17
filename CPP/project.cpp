#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <unordered_set>
#include <fstream>
#include <cstdio>
#include <cstdlib>

using namespace std;

bool multiline = false;
string lexeme = "";
int state = 0;
int commentedline[100];
int commentindex=0;
int assignmentline[100];
int assignmentindex=0;

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

void Lexical(string str,int linenum,ofstream &tokenout,ofstream &identiferout)
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
            }
            else if(is_Digit(c))
            {
                state=2;
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
                state=0;
                lexeme=c;
                tokenout<<"Tokken <"<<" BiOperator , "<<lexeme<<" > " <<endl;
                lexeme="";
                flag=0;
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
                cout<<"INVALID TOKEN AT LINE : "<<linenum<<endl;
                flag=0;
                lexeme="";
                break;
            }
        break;

        case 1:
            if(!(is_Letter(c) || is_Digit(c)|| c=='_'))
            {

                state=0;

                if(keywords.count(lexeme) > 0)
                {
                    tokenout<<"Tokken <"<<" KeyWord , "<<lexeme<<" > " <<endl;
                }
                else
                {
                    tokenout<<"Tokken <"<<" Identity , "<<lexeme<<" > " <<endl;
                    //check id identifier is not in file then add it
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
    }
    if(flag)
        lexeme=lexeme+c;

    i++;
    }

}

void SymbolTable(string line,int linenum)
{
    ofstream sybolout("symbol.txt",ios::app);
    int i=0;
    char c='\0';
    string lexeme="",lexemeval="",type;
    for(int j=0;j<commentindex;j++)
    {
        if(linenum==commentedline[j])
        {
            return;
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
                            
                            if(line[i]=='"')
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
                                    cout<<"ERROR STRING NOT CLOSED AT LINE : "<<linenum<<endl;
                                    return;
                                }
                            }
                            else if(is_Digit(line[i]))
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
                            else if(line[i]=='$')
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
                                    cout<<"ERROR VARIABLE NOT DECLARED AT LINE : "<<linenum<<endl;
                                    return;
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
    
}

int main() {
    // Test the lexical analyzer with a PHP code snippet
    ifstream file("source.txt");
    if (!file) 
    {
        cout << "Unable to open Input File." << endl;
        return 1;
    }
    ofstream tokenout("mytoken.txt");
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
        Lexical(line,linenum,tokenout,identiferout);        
    }
    tokenout.close();
    identiferout.close();
    file.close();
    ifstream file2("source.txt");
    if (!file2) 
    {
        cout << "Unable to open Input File." << endl;
        return 1;
    }
    linenum=0;
    while (getline(file2, line)) {
        linenum++;
        SymbolTable(line,linenum); 
    }
    file2.close();

    const char* command = "a.exe";

    // Open a pipe to read the output of the command
    FILE* pipe = _popen(command, "r");

    if (!pipe) {
        cerr << "popen() failed!" << endl;
        return EXIT_FAILURE;
    }

    // Buffer to store the output
    char buffer[128];
    string result = "";

    // Read the output of the command
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != nullptr)
            result += buffer;
    }

    // Close the pipe
    _pclose(pipe);

    // Print the output
    cout <<result << endl;

    return EXIT_SUCCESS;
        
    return 0;
}
