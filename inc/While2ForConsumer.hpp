#ifndef __WHILE2FOR__
#define __WHILE2FOR__

#include "Helpers.hpp"

// Posetilac koji while pretvara u for
class While2ForVisitor : public RecursiveASTVisitor<While2ForVisitor>, public Helpers {
public:
  // Inicijalizacija prepisivaca i konteksta
  While2ForVisitor(Rewriter &R, ASTContext &A)
    : Helpers(R, A) {}
  
  // Pretvaranje while petlji u for
  bool VisitWhileStmt(WhileStmt *s);

  // Prekid obilaska kod while petlje
  bool TraverseWhileStmt(WhileStmt* s);
};

// Klasa za obradu dobijenog AST stabla
class While2ForConsumer : public ASTConsumer {
public:
  // Konstruktor inicijalizuje posetioca
  // prepisivacem i kontekstom parsiranja
  While2ForConsumer(Rewriter &R, ASTContext &A)
    : Visitor(R, A) {}

  // Svaka deklaracija obradjuje se zasebno
  bool HandleTopLevelDecl(DeclGroupRef DR) override;

private:
  // Privatno cuvanje posetioca stabla
  While2ForVisitor Visitor;
};

#endif
