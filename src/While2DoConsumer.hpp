#ifndef __DOWHILE_AST__
#define __DOWHILE_AST__ 1

#include "Helpers.hpp"

// Posetilac koji sve petlje pretvara u do-while
class While2DoVisitor : public RecursiveASTVisitor<While2DoVisitor>, public Helpers {
public:
  // Inicijalizacija prepisivaca i konteksta
  While2DoVisitor(Rewriter &R, ASTContext &A)
    :Helpers(R, A) {}

  // Pretvaranje while petlji u do-while
  bool VisitWhileStmt(WhileStmt *s);
  
  // Prekid obilaska kod while petlje
  bool TraverseWhileStmt(WhileStmt* s);
};

// Klasa za obradu dobijenog AST stabla
class While2DoConsumer : public ASTConsumer {
public:
  // Konstruktor inicijalizuje posetioca
  // prepisivacem i kontekstom parsiranja
  While2DoConsumer(Rewriter &R, ASTContext &A)
    : Visitor(R, A) {}

  // Svaka deklaracija obradjuje se zasebno
  bool HandleTopLevelDecl(DeclGroupRef DR) override;

private:
  // Privatno cuvanje posetioca stabla
  While2DoVisitor Visitor;
};

#endif
