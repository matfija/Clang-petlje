#ifndef __DOFOR_AST__
#define __DOFOR_AST__ 1

#include "Helpers.hpp"

// Posetilac koji sve petlje pretvara u do-while
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
