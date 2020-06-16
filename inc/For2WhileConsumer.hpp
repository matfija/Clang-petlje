#ifndef __FOR2WHILE__
#define __FOR2WHILE__

#include "Helpers.hpp"

// Posetilac koji for pretvara u while
class For2WhileVisitor : public RecursiveASTVisitor<For2WhileVisitor>, public Helpers {
public:
  // Inicijalizacija prepisivaca i konteksta
  For2WhileVisitor(Rewriter &R, ASTContext &A)
    : Helpers(R, A) {}
  
  // Pretvaranje for petlji u while
  bool VisitForStmt(ForStmt *s);
  
  // Prekid obilaska kod for petlje
  bool TraverseForStmt(ForStmt* s);
};

// Klasa za obradu dobijenog AST stabla
class For2WhileConsumer : public ASTConsumer {
public:
  // Konstruktor inicijalizuje posetioca
  // prepisivacem i kontekstom parsiranja
  For2WhileConsumer(Rewriter &R, ASTContext &A)
    : Visitor(R, A) {}

  // Svaka deklaracija obradjuje se zasebno
  bool HandleTopLevelDecl(DeclGroupRef DR) override;

private:
  // Privatno cuvanje posetioca stabla
  For2WhileVisitor Visitor;
};

#endif
