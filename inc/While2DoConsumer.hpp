#ifndef __WHILE2DO__
#define __WHILE2DO__

#include "LoopConsumer.hpp"

// Posetilac koji while pretvara u do
class While2DoVisitor : public LoopVisitor<While2DoVisitor> {
public:
  // Inicijalizacija prepisivaca i konteksta
  While2DoVisitor(Rewriter &R, ASTContext &A)
    : LoopVisitor(R, A) {}

  // Pretvaranje while petlji u do-while
  bool VisitWhileStmt(WhileStmt *s);
  
  // Prekid obilaska kod while petlje
  bool TraverseWhileStmt(WhileStmt *s);
};

#endif
