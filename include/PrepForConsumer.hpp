#ifndef __PREPFOR__
#define __PREPFOR__

#include "Helpers.hpp"

// Posetilac koji dodaje korak petlje
class PrepForVisitor : public RecursiveASTVisitor<PrepForVisitor>, public Helpers {
public:
  // Inicijalizacija prepisivaca i konteksta
  PrepForVisitor(Rewriter &R, ASTContext &A)
    : Helpers(R, A) {}
  
  // Dodavanje inkrementa pre continue
  bool VisitContinueStmt(ContinueStmt *s);
};

// Klasa za obradu dobijenog AST stabla
class PrepForConsumer : public ASTConsumer {
public:
  // Konstruktor inicijalizuje posetioca
  // prepisivacem i kontekstom parsiranja
  PrepForConsumer(Rewriter &R, ASTContext &A)
    : Visitor(R, A) {}

  // Svaka deklaracija obradjuje se zasebno
  bool HandleTopLevelDecl(DeclGroupRef DR) override;

private:
  // Privatno cuvanje posetioca stabla
  PrepForVisitor Visitor;
};

#endif
