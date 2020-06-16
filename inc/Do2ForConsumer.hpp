#ifndef __DO2FOR__
#define __DO2FOR__

#include "Helpers.hpp"

// Posetilac koji do pretvara u for
class Do2ForVisitor : public RecursiveASTVisitor<Do2ForVisitor>, public Helpers {
public:
  // Inicijalizacija prepisivaca i konteksta
  Do2ForVisitor(Rewriter &R, ASTContext &A)
    : Helpers(R, A) {}
  
  // Pretvaranje do-while petlji u for
  bool VisitDoStmt(DoStmt *s);
  
  // Prekid obilaska kod do petlje
  bool TraverseDoStmt(DoStmt* s);
  
  // Nacin obrade deklaracije
  bool TraverseDecl(Decl *d);

private:
  // Privatno cuvanje tekuce deklaracije
  Decl* tekdek;
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
