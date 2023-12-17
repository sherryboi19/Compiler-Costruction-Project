Running Manual
--------------

1. Open cmd by typing cmd in address bar of file explorer and hit 'Enter'.
2. On cmd. Run following Commands One by One:
	bison -d syntaxpart.y
	flex lexicalpart.l
	gcc lex.yy.c syntaxpart.tab.c
3. An exe file named a.exe will be genrated.
4. Open 'project.cpp' File and compile it using g++ command
	g++ project.cpp -o project
5. Type code to be tested in 'source.txt' file.
6. Open folder where all the files are stored.
7. Run project.exe

Output
------

token.txt         Containing All the Generated Tokens
identifier.txt    Containing All the Identifiers
symbol.txt        Containing All the Variables,DataType,Value,Linenum

Console Output
--------------

All the lines Containing echo keyword