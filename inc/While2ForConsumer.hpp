#ifndef __WHILE2FOR__
#define __WHILE2FOR__

#include "LoopConsumer.hpp"

// Posetilac koji while pretvara u for
class While2ForVisitor : public LoopVisitor<While2ForVisitor> {
public:
  // Inicijalizacija prepisivaca i konteksta
  While2ForVisitor(Rewriter &R, ASTContext &A)
    : LoopVisitor(R, A) {}
  
  // Pretvaranje while petlji u for
  bool VisitWhileStmt(WhileStmt *s);

  // Prekid obilaska kod while petlje
  bool TraverseWhileStmt(WhileStmt *s);
};

#endif
