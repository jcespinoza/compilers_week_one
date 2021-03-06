#ifndef _AST_H
#define _AST_H

#define DEF_OPERATOR_EXPR(name) \
  class name##Expr : public BinaryExpr{\
  public:\
    name##Expr(Expr* LeftSide, Expr *RightSide): BinaryExpr(LeftSide, RightSide){} \
    SynthMIPS GenerateCode(Scope& scope); \
  }

#include <list>
#include "stdio.h"
#include <iostream>
#include <map>
#include <string>

using namespace std;

enum ExprKind {
  Add,
  Sub,
  Mult,
  Div,
  Id,
  Num
};

typedef struct SynthMIPS {
  string Place;
  string Code;
  int Constant;
  bool hasValue;
} SynthMIPS;

class Scope {
public:
  Scope(){
    registers["$t0"] = 0;
    registers["$t1"] = 0;
    registers["$t2"] = 0;
    registers["$t3"] = 0;
    registers["$t4"] = 0;
    registers["$t5"] = 0;
    registers["$t6"] = 0;
    registers["$t7"] = 0;
    registers["$t8"] = 0;
    registers["$t9"] = 0;
    // registers["$s0"] = 0;
    // registers["$s1"] = 0;
    // registers["$s2"] = 0;
    // registers["$s3"] = 0;
    // registers["$s4"] = 0;
    // registers["$s5"] = 0;
    // registers["$s6"] = 0;
    // registers["$s7"] = 0;
  }

  map<string, bool> registers;

  void releaseRegister(string name);

  string getFreeRegister();
};


class Expr{
public:
  virtual SynthMIPS GenerateCode(Scope& scope) = 0;
};

class BinaryExpr: public Expr{
protected:
  BinaryExpr(Expr* LeftSide, Expr *RightSide){
    this->LeftSide = LeftSide;
    this->RightSide = RightSide;
  }
public:
  Expr* LeftSide, *RightSide;
};

DEF_OPERATOR_EXPR(Add);
DEF_OPERATOR_EXPR(Sub);
DEF_OPERATOR_EXPR(Mult);
DEF_OPERATOR_EXPR(Div);

class NumExpr : public Expr{
public:
  NumExpr(int value){
    this->value = value;
  }
  SynthMIPS GenerateCode(Scope& scope);
  void releaseRegisters();
  int value;
};

class IdExpr: public Expr{
public:
  IdExpr(char* varName){ this->varName.append( varName ); }

  string varName;
  SynthMIPS GenerateCode(Scope& scope);
  void releaseRegisters();
};

class Statement{
public:
  virtual SynthMIPS GenerateCode(Scope& scope) = 0;
};

class StatementList : public Statement{
public:
  StatementList(){

  }
  SynthMIPS GenerateCode(Scope& scope);
  void addStatement(Statement* st){ statements.push_back(st); }

  list<Statement*> statements;
};

class ExprStatement : public Statement{
public:
  ExprStatement(Expr* expression){
    this->expression = expression;
  }
  SynthMIPS GenerateCode(Scope& scope);
  Expr* expression;
};

class AssignmentStatement: public Statement{
public:
  AssignmentStatement(Expr* expression){
    this->expression = expression;
  }
  SynthMIPS GenerateCode(Scope& scope);
  Expr* expression;
};

class IdAssignmentSt: public AssignmentStatement{
public:
  IdAssignmentSt(char* varName, Expr* expression): AssignmentStatement(expression){
    this->expression = expression;
    this->varName.append( varName );
  }

  SynthMIPS GenerateCode(Scope& scope);

  string varName;
};

class PrintStatement: public Statement {
public:
  PrintStatement(Expr* ex){
    this->expression = ex;
  }
  SynthMIPS GenerateCode(Scope& scope);
  Expr* expression;
};


class Root {
public:
  Root(StatementList* statements){
    this->statementList = statements;
  }
  string GenerateCode(Scope& scope);
  StatementList* statementList;
};

#endif //_AST_H
