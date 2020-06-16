#ifndef __FOR2DO__
#define __FOR2DO__

#include "Helpers.hpp"

// Posetilac koji for pretvara u do
class For2DoVisitor : public RecursiveASTVisitor<For2DoVisitor>, public Helpers {
public:
  // Inicijalizacija prepisivaca i konteksta
  For2DoVisitor(Rewriter &R, ASTContext &A)
    :Helpers(R, A) {}
  
  // Pretvaranje for petlji u do-while
  bool VisitForStmt(ForStmt *s);
  
  // Prekid obilaska kod for petlje
  bool TraverseForStmt(ForStmt* s);
};

// Klasa za obradu dobijenog AST stabla
class For2DoConsumer : public ASTConsumer {
public:
  // Konstruktor inicijalizuje posetioca
  // prepisivacem i kontekstom parsiranja
  For2DoConsumer(Rewriter &R, ASTContext &A)
    : Visitor(R, A) {}

  // Svaka deklaracija obradjuje se zasebno
  bool HandleTopLevelDecl(DeclGroupRef DR) override;

private:
  // Privatno cuvanje posetioca stabla
  For2DoVisitor Visitor;
};

#endif
