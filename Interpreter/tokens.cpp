#include "tokens.h"

bool arr[4]={0};
vector<double> print;
Tokens onScrenn;
vector <string> privMem;


Token::Token(Operators op)
{
    tokens = Type_Tokens::Operator;
    operators = op;
}
Token::Token(double num)
{
    tokens = Type_Tokens::Number;
    number = num;
}
Token::Token(string op)
{
    if(arr[0]) {
        tokens = Type_Tokens::KeyWord;
        l = op; }
    if(arr[1]) {
        tokens = Type_Tokens::Variable;
        var = op;
    }
    if(arr[2]) {
        tokens = Type_Tokens::Function;
        func = op;
    }
    if(arr[3]) {
        tokens = Type_Tokens::Operator;
        doubleOp = op;
    }
}
Type_Tokens Token::GetTypeTokens() const
{
    return tokens;
}
string Token::GetString() const {
    return l;
}
Operators Token::GetOperator() const {
    return operators;
}
string Token::GetVar() const {
    return var;
}
string Token::GetFunc() const {
    return func;
}
double Token::GetNum() const {
    return number;
}
string Token::GetOneAfter() const {
    QString d = QString::number(number,'f', 1);
    return d.toStdString();
}





//----------------------------------------------------------------------------------------------------------------------------







bool Make_tokens::IsNumber(int i) const {
    string t = ListTemp[i].toStdString();
    string::const_iterator it = t.begin();
    if(*it=='-' && isdigit(*++it)) {
        while (it != t.end() && isdigit(*it)) ++it;
  } else {
        while (it != t.end() && isdigit(*it)) ++it;
}
    return !t.empty() && it == t.end();
}

bool Make_tokens::IsOperator(int i)  {
    tmp=ListTemp[i].toStdString();
    auto all = { Operators::plus, Operators::minus, Operators::multiple, Operators::divide, Operators::left_paren,
                 Operators::right_paren, Operators::equal, Operators::less, Operators::op_R,
               Operators::op_L, Operators::greater, Operators::semicolon, Operators::Excl };
    return any_of(all.begin(), all.end(), [this](Operators o) {return tmp[0] == static_cast<char>(o); });
}
bool Make_tokens::IsKeyWord(int i) const {
    for(int j=0; j<KeyWords.size(); ++j) {
        if(ListTemp[i]==KeyWords[j])
            return true;
    }
    return false;
}
bool Make_tokens::IsVar(int i) const {
    for(int j=0; j<int(v.size()); ++j) {
        if(ListTemp[i].toStdString()==v[unsigned(j)])
            return true;
    }
    return false;
   }
bool Make_tokens::IsFunc(int i) const {
    for(int j=0; j<int(f.size()); ++j) {
        if(ListTemp[i].toStdString()==f[unsigned(j)])
            return true;
    }
    return false;
   }
bool Make_tokens::IsDoubleOperator(int i)  {
    for(int j=0; j<doubleOper.size(); ++j ) {
        if(ListTemp[i]==doubleOper[j])
            return true;
    }
    return false;
}
void Make_tokens::Tokenize() {

    for(int i=0; i<ListTemp.size(); ++i) {
         if(IsDoubleOperator(i)) {
            ScanDoubleOperator(i);
        }
        else if (IsKeyWord(i)) {
            ScanKeyWord(i);

        }
        else if (IsVar(i)) {
            ScanVar(i);
        }
        else if (IsFunc(i)) {
            ScanFunc(i);
        }
        else if(IsNumber(i)) {
            ScanNumber(i);
        }
        else if(IsOperator(i)) {
             ScanOperator();
        }
    }

}
void Make_tokens::TokenizerWords(const string &s) {
    QString temp=QString::fromStdString(s);
    QStringList strList=temp.split(" ");
    ListTemp = strList;
}
void Make_tokens::ScanNumber(int i) {
       res.push_back(ListTemp[i].toDouble());
}
void Make_tokens::ScanOperator() {
    res.push_back(static_cast<Operators>(tmp[0]));
}
void Make_tokens::ScanKeyWord(int &i) {
    arr[0]=1;
    arr[1]=arr[2]=arr[3]=0;

    string func;
    res.push_back(ListTemp[i].toStdString());
    QString t;
    if(ListTemp[i].toStdString()=="VAR") {
       ++i;
     arr[1]=1;
     arr[0]=arr[2]=arr[3]=0;
     int k = i;
     ++k;
     if(ListTemp[k]=="=") {
         --k;
         t= ListTemp[k].toUpper();
         res.push_back(t.toStdString());
         v.push_back(ListTemp[i].toStdString());
     } else {
         res.push_back(ListTemp[i].toStdString());
         v.push_back(t.toStdString());
}
    }
    if(ListTemp[i].toStdString()=="FUNCTION") {
     ++i;
     arr[2]=1;
     arr[1]=arr[0]=arr[3]=0;
     res.push_back(ListTemp[i].toStdString());
     f.push_back(ListTemp[i].toStdString());
    }
}
void Make_tokens::ScanVar(int i) {
    arr[1]=1;
    arr[0]=arr[2]=arr[3]=0;
    ++i;
    if(ListTemp[i]=="=") {
        --i;
        QString t= ListTemp[i].toUpper();
        res.push_back(t.toStdString());
    } else {
         --i;
        res.push_back(ListTemp[i].toStdString());
    }
}
void Make_tokens::ScanFunc(int i) {
    arr[2]=1;
    arr[1]=arr[0]=arr[3]=0;
    res.push_back(ListTemp[i].toStdString());
}
void Make_tokens::ScanDoubleOperator(int i) {
    arr[3]=1;
    arr[1]=arr[0]=arr[2]=0;

    if (ListTemp[i]=="==") {
        res.push_back(static_cast<Operators>(Operators::equal_to));
    } else if (ListTemp[i]=="<=") {
        res.push_back(static_cast<Operators>(Operators::less_equal));
    } else if (ListTemp[i]==">=") {
        res.push_back(static_cast<Operators>(Operators::greater_equal));
    } else if (ListTemp[i]=="!=") {
        res.push_back(static_cast<Operators>(Operators::not_equal_to));
    }
}

void Make_tokens::FindPrivate() {
    for(int i=0; i<int(res.size()); ++i) {
        if(res[unsigned(i)].GetString()=="PRIVATE") {
            while(res[unsigned(i)].GetOperator()!=Operators::op_R) {
                ++i;
                if(res[unsigned(i)].GetTypeTokens()==Type_Tokens::Variable) {
                    privMem.push_back(res[unsigned(i)].GetVar());
                }
            }
            break;
        }
    }
    QString t, m;
    for(int i=0; i<int(res.size()); ++i) {
        if(res[unsigned(i)].GetString()=="PRIVATE") {
            while(res[unsigned(i)].GetOperator()!=Operators::op_R) {
                ++i; }
        }
        if(res[unsigned(i)].GetString()=="END") {
            endK = true;
        }
        if(res[unsigned(i)].GetString()=="FUNCTION" && endK) {
            while(res[unsigned(i)].GetOperator()!=Operators::op_R) {
                ++i; }
        }
        if(res[unsigned(i)].GetTypeTokens()==Type_Tokens::Variable) {
            for(int j=0; j<int(privMem.size()); ++j) {
                t = QString::fromStdString(res[unsigned(i)].GetVar());
                t=t.toLower();
                m = QString::fromStdString(privMem[unsigned(j)]);
                m=m.toLower();
                if(t==m) {
                    throw logic_error(m.toStdString()+" is a private member");
                }
            }
        }
    }
}

Tokens& Make_tokens::Result()  {
    return res;
}
Tokens Tokenize(const string &s) {
    Make_tokens tokenizer;
    tokenizer.TokenizerWords(s);
    tokenizer.Tokenize();
    tokenizer.FindPrivate();
    return tokenizer.Result();
}









//----------------------------------------------------------------------------------------------------------------------------






string ToStringOP(const Operators &op) {
    return{ static_cast<char>(op) };
}

string ToString(const Token &token) {
    switch(token.GetTypeTokens()) {
        case Type_Tokens::Number:
            return token.GetOneAfter();
        case Type_Tokens::Operator:
            return ToStringOP(static_cast<Operators>(token));
        case Type_Tokens::KeyWord:
            return token.GetString();
        case Type_Tokens::Variable:
            return token.GetVar();
        case Type_Tokens::Function:
            return token.GetFunc();

    }
}










//----------------------------------------------------------------------------------------------------------------------------







void Parser::Parse() {
    for(; current != end; ++current) {
        ParseCurrentToken();
    }
    PopToOutputUntil([this]() {return StackHasNoOperators(); });
    PopToOutputUntil([this]() {return StackOperators(); });
}

const Tokens& Parser::Result() const {
    return output;
}

void Parser::ParseCurrentToken() {
    switch(current->GetTypeTokens()) {
        case Type_Tokens::Operator:
            ParseOperator();
            break;
        case Type_Tokens::Number:
            ParseNumber();
            break;
        case Type_Tokens::Variable:
            ParseNumber();
            break;
        case Type_Tokens::KeyWord:
            PushCurrentToStack();
            break;
        case Type_Tokens::Function:
            PushCurrentToStack();
            break;
    }
}

bool Parser::StackHasNoOperators() const {
    if(stack.back() == Token(Operators::left_paren) ) {
        throw logic_error("Closing paren not found.");
    }
    return false;
}

bool Parser::StackOperators() const {
    if(stack.back() == Token(Operators::op_L) ) {
        throw logic_error("Closing paren not found.");
    }
    return false;
}
void Parser::ParseOperator() {
    switch(*current) {
        case Operators::left_paren:
            PushCurrentToStack();
            break;
        case Operators::right_paren:
            PopToOutputUntil([this]() { return LeftParenOnTop(); });
            PopLeftParen();
            PopToOutputUntil([this]() { return KeyWordOnTop(); });
            break;
         case Operators::op_L:
            PushCurrentToStack();
            break;
         case Operators::op_R:
          PopToOutputUntil([this]() { return LeftOnTop(); });
          PopParen();
          PopToOutputUntil([this]() { return KeyWordOnTop(); });
          ParseNumber();
        break;
        default:
            PopToOutputUntil([this]() { return LeftParenOnTop() || OperatorWithLessPrecedenceOnTop()|| LeftOnTop()
                        || OpOnTop(); });
            PushCurrentToStack();
            break;
    }
}

void Parser::PushCurrentToStack() {
    if(current->GetString()=="WHILE")
       ParseNumber();
    return stack.push_back(*current);
}

void Parser::PopLeftParen() {
    if(stack.empty() || stack.back() != Operators::left_paren) {
        throw std::logic_error("Opening paren not found.");
    }
    stack.pop_back();
}
void Parser::PopParen() {
    if(stack.empty() || stack.back() != Operators::op_L) {
        throw std::logic_error("Opening paren not found.");
    }
    stack.pop_back();
}
bool Parser::OperatorWithLessPrecedenceOnTop() const {
    return PrecedenceOf(stack.back()) < PrecedenceOf(*current);
}
bool Parser::LeftParenOnTop()  {
    return static_cast<Operators>(stack.back()) == Operators::left_paren;
}
bool Parser::OpOnTop()  {
    return (static_cast<Operators>(stack.back()) == Operators::equal) ||
            (static_cast<Operators>(stack.back()) == Operators::less) ||
            (static_cast<Operators>(stack.back()) == Operators::greater) ||
            (static_cast<Operators>(stack.back()) == Operators::less) ||
            (static_cast<Operators>(stack.back()) == Operators::greater);
}
bool Parser::LeftOnTop()  {
    return  static_cast<Operators>(stack.back()) == Operators::op_L;
}
bool Parser::KeyWordOnTop()  {
    return !(stack.back().GetTypeTokens() == Type_Tokens::KeyWord);
}
void Parser::ParseNumber() {
    output.push_back(*current);
}
template<class T>
void Parser::PopToOutputUntil(T whenToEnd) {
    while(!stack.empty() && !whenToEnd() ) {
        output.push_back(stack.back());
        stack.pop_back();
    }
}
int PrecedenceOf(Operators op) {
    return (op == Operators::multiple || op == Operators::divide) ? 1 : 0;
}
Tokens Parse(const Tokens &tokens) {
    Parser parser(tokens);
    parser.Parse();
    onScrenn = parser.Result();
    return parser.Result();
}







//----------------------------------------------------------------------------------------------------------------------------




void Interpret::Loop() {
    for(; tree != end; ++tree) {
        if(tree->GetString()=="END")
            break;
        TakeTokens();
    }
  }

void Interpret::TakeTokens() {
    switch (tree->GetTypeTokens()) {
      case Type_Tokens::Operator:
            VisitOperator(*tree);
            break;
      case Type_Tokens::Number:
            ToStack();
            break;
      case Type_Tokens::Variable:
            GoToVar();
            break;
      case Type_Tokens::KeyWord:
            MakeOp();
            break;
      case Type_Tokens::Function:
            nameFunc = tree->GetFunc();
            GoToFunc();
            break;

        }
    }

void Interpret::ToStack() {
     stack.push_back(*tree);
}

void Interpret::VisitOperator(Operators op)  {
    if(op == Operators::op_L || op == Operators::op_R ) {
        if(enter) {
            ++tree;
            if(tree->GetString()!="ELSE") {
                --tree;
                enter = false;
                return;
            }
                while (tree->GetOperator()!=Operators::op_R  && tree != end) {
                    ++tree;
                }
                enter = false;
            }
        if(isCycle) {
            tempTreeWhile--;
            while(tempTreeWhile->GetString()!="WHILE") {
                tempTreeWhile--;
            }
            tree=tempTreeWhile;
        }
        return; }
    if(op== Operators::equal) {
        idea = tree;
        Back();
        return; }
    double second = PopOperand();
    double first = PopOperand();
    stack.push_back(BinaryFunctionFor(op)(first, second));
}

double Interpret::PopOperand() {
     double back = stack.back();
     stack.pop_back();
     return back;
}




void Interpret::MakeOp() {
    if(tree->GetString()=="COS") {
        double k =cos(stack.back());
        stack.pop_back();
        stack.push_back(k);
    }
    if(tree->GetString()=="SIN") {
        double k =sin(stack.back());
        stack.pop_back();
        stack.push_back(k);
    }
    if(tree->GetString()=="TAN") {
        double k =tan(stack.back());
        stack.pop_back();
        stack.push_back(k);
    }
    if(tree->GetString()=="SQRT") {
        double k =sqrt(stack.back());
        stack.pop_back();
        stack.push_back(k);
    }
    if(tree->GetString()=="ACOS") {
        double k =acos(stack.back());
        stack.pop_back();
        stack.push_back(k);
    }
    if(tree->GetString()=="ASIN") {
        double k =asin(stack.back());
        stack.pop_back();
        stack.push_back(k);
    }
    if(tree->GetString()=="ATAN") {
        double k =atan(stack.back());
        stack.pop_back();
        stack.push_back(k);
    }
    if(tree->GetString()=="PRINT") {
        print.push_back(stack.back());
    }
    if(tree->GetString()=="IF") {
        int k =int (stack.back());
        if (k) {
            enter = true;
            stack.pop_back();
        }
        else {
            while (tree->GetOperator()!=Operators::op_R) {
                ++tree;
            }
            enter = false;
            stack.pop_back();
        }
    }
    if(tree->GetString()=="RETURN") {
       tree=tempTree;
    }
    if(tree->GetString()=="WHILE") {
        if(IsWhile) {
      int k = int(stack.back());
      if (!k) {
          while(tree->GetOperator()!=Operators::op_R) {
              ++tree;
          }
          stack.pop_back();
          isCycle = false;
          IsWhile = false;
      }
      else {
            stack.pop_back();
            tempTreeWhile = tree;
            isCycle = true;
      }
    }
        IsWhile = true;
    }
}

void Interpret::GoToVar() {
    QString tem = QString::fromStdString(tree->GetVar());
    tem=tem.toUpper();
    if (m.find(tem.toStdString())!=m.end()) {
        auto t = m.find(tem.toStdString());
        stack.push_back(t->second);
    }
}

void  Interpret::Back() {
    while (idea->GetTypeTokens()!=Type_Tokens::Variable && idea!=end) {
        --idea;
    }
    QString t = QString::fromStdString(idea->GetVar());
    if (t.isUpper()) {
     GoToEq(); }
    else {
        --idea;
         Back();
    }
}
void  Interpret::GoToEq() {
    if(m.find(idea->GetVar())==m.end()) {
       m.insert(pair<string, double>(idea->GetVar(),stack.back()));
       stack.pop_back();

    } else if (m.find(idea->GetVar())!=m.end()) {
        m.find(idea->GetVar())->second = stack.back();
            stack.pop_back();
    }
}
void Interpret::GoToFunc() {
     ++tree;
    if(tree->GetString()=="FUNCTION") {
        --tree;
        return;
    }
     --tree;
     tempTree = tree;
     ++tree;
     while(tree->GetFunc()!=nameFunc && tree!=end) {
         inFunc = true;
         ++tree;
     }
 }
double Interpret::Result()  {
       return stack.empty() ? 0 : stack.back();
}


double Go(const Tokens &tokens) {
       Interpret inter(tokens);
       inter.Loop();
       return inter.Result();
}

double InterpreteExperssion(const string &expression) {
       return Go(Parse(Tokenize(expression)));
}
