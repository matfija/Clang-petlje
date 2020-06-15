#ifndef __FORDO_AST__
#define __FORDO_AST__ 1

#include "Helpers.hpp"

// Posetilac koji sve petlje pretvara u for
class Do2ForVisitor : public RecursiveASTVisitor<Do2ForVisitor>, public Helpers {
public:
  // Inicijalizacija prepisivaca i konteksta
  Do2ForVisitor(Rewriter &R, ASTContext &A)
    : Helpers(R, A)
    {}
  
  // Pretvaranje do-while petlji u for
  bool VisitDoStmt(DoStmt *s);
  
  // Prekid obilaska kod do petlje
  bool TraverseDoStmt(DoStmt* s);

};

// Klasa za obradu dobijenog AST stabla
class Do2ForConsumer : public ASTConsumer {
public:
  // Konstruktor inicijalizuje posetioca
  // prepisivacem i kontekstom parsiranja
  Do2ForConsumer(Rewriter &R, ASTContext &A)
    : Visitor(R, A) {}

  // Svaka deklaracija obradjuje se zasebno
  bool HandleTopLevelDecl(DeclGroupRef DR) override;
private:
  // Privatno cuvanje posetioca stabla
  Do2ForVisitor Visitor;
};

#endif
