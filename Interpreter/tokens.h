#ifndef TOKENS_H
#define TOKENS_H
#include <vector>
#include <string>
#include <QStringList>
#include <map>
#include <functional>
#include <cmath>

using namespace std;

extern bool arr[4];
extern vector<double> print;
extern vector <string> privMem;

enum class Operators: char {
    plus = '+',
    minus = '-',
    multiple = '*',
    divide = '/',
    left_paren = '(',
    right_paren = ')',
    equal = '=',
    greater = '>',
    less = '<',
    op_L = '{',
    op_R = '}',
    semicolon = ';',
    Excl = '!',
    equal_to,
    not_equal_to,
    greater_equal,
    less_equal
};
enum class Type_Tokens {
    Operator,
    Number,
    KeyWord,
    Variable,
    Function
};
class Token {
private:
    Type_Tokens tokens;
    Operators operators;
    string l;
    string var;
    string func;
    string doubleOp;
    double number;
public:
    Type_Tokens GetTypeTokens() const;
    string GetString() const;
    Operators GetOperator() const;
    string GetVar() const;
    string GetFunc() const;
    double GetNum() const;
    string GetOneAfter() const;
    Token(Operators op);
    Token(double num);
    Token(string key_words);
    operator Operators() const
    {
        return operators;
    }
    operator double() const
    {
        return number;
    }
    operator string() const
    {
        return l;
    }

    friend bool operator==(const Token &left, const Token &right)
    {
         if(left.tokens == right.tokens) {
            switch(left.tokens) {
                   case Type_Tokens::Operator:
                        return left.operators == right.operators;
                   case Type_Tokens::Number:
                        return left.number - right.number == 0.0;
                   case Type_Tokens::KeyWord:
                        return left.operators == right.operators;
                   case Type_Tokens::Variable:
                        return left.var == right.var;
                   case Type_Tokens::Function:
                        return left.func == right.func;
                }
            }
         return false;
     }
};

typedef vector<Token> Tokens;
extern Tokens onScrenn;






//----------------------------------------------------------------------------------------------------------------------------







class Make_tokens {
private:
    QStringList ListTemp;
    string tmp;
    QStringList KeyWords = { "IF", "ELSE", "WHILE", "VAR", "SQRT", "COS", "SIN", "TAN",
                             "ACOS", "ASIN", "ATAN", "FUNCTION", "PRINT", "RETURN", "END", "PRIVATE"};
    QStringList doubleOper = {"==", "<=", ">=", "!="};
    vector<string> v;
    vector<string> f;
    Tokens res;
    bool endK = false;
    void ScanNumber(int i);
    void ScanOperator();
    void ScanKeyWord(int &i);
    void ScanVar(int i);
    void ScanFunc(int i);
    void ScanDoubleOperator(int i);
public:
    void TokenizerWords(const string &expr);
    bool IsNumber(int i) const;
    bool IsOperator(int i);
    bool IsKeyWord(int i) const;
    bool IsVar(int i) const;
    bool IsFunc(int i) const;
    bool IsDoubleOperator(int i);
    void Tokenize();
    void FindPrivate();
    Tokens &Result();
};
Tokens Tokenize(const string &expr);







//----------------------------------------------------------------------------------------------------------------------------












class Parser {
public:
    Parser(const Tokens &tokens) : current(tokens.cbegin()), end(tokens.cend()) {}
    void Parse();
    const Tokens &Result() const;
private:
    Tokens::const_iterator current;
    Tokens::const_iterator end;
    Tokens output;
    Tokens stack;
    void ParseCurrentToken();
    bool StackHasNoOperators() const;
    bool StackOperators() const;
    void ParseOperator();
    void PushCurrentToStack();
    void PopLeftParen();
    void PopParen();
    bool OperatorWithLessPrecedenceOnTop() const;
    bool LeftParenOnTop();
    bool OpOnTop();
    bool LeftOnTop();
    bool KeyWordOnTop();
    void ParseNumber();
    template<class T>
    void PopToOutputUntil(T whenToEnd);

};
int PrecedenceOf(Operators op);
Tokens Parse(const Tokens &tokens);









//----------------------------------------------------------------------------------------------------------------------------








class Interpret {
public:
    Interpret(const Tokens &tokens) : tree(tokens.cbegin()), end(tokens.cend()) {}
    void Loop();
 private:
    Tokens::const_iterator tree;
    Tokens::const_iterator end;
    Tokens::const_iterator tempTree;
    Tokens::const_iterator tempTreeWhile;
    Tokens::const_iterator idea;
    vector<double> stack;
    string nameFunc;
    map<string, double> m;
    bool enter = false;
    bool inFunc = false;
    bool isCycle = false;
    bool IsWhile = false;
    void TakeTokens();
    void ToStack();
    void VisitOperator(Operators op);
    double PopOperand();
    void MakeOp();
    void GoToVar();
    void  Back();
    void  GoToEq();
    void GoToFunc();
    static const function<double(double, double)> & BinaryFunctionFor(Operators op) {
         static const map<Operators, function<double(double, double)>> functions{
         { Operators::plus, plus<double>() },
         { Operators::minus, minus<double>() },
         { Operators::multiple, multiplies<double>() },
         { Operators::divide, divides<double>() },
         { Operators::greater, greater<double>() },
         { Operators::less, less<double>() },
         { Operators::greater_equal, greater_equal<double>() },
         { Operators::less_equal, less_equal<double>() },
         { Operators::not_equal_to, not_equal_to<double>() },
         { Operators::equal_to, equal_to<double>() } };
         auto found = functions.find(op);
         if(found == functions.cend()) {
         throw logic_error("Operator not found."); }
         return found->second;
    }
public:
    double Result();
};

double Go(const Tokens &tokens);
double InterpreteExperssion(const string &expression);




//----------------------------------------------------------------------------------------------------------------------------

string ToStringOP(const Operators &op);
string ToString(const Token &token);






#endif // TOKENS_H
