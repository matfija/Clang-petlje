#ifndef __PREPFOR__
#define __PREPFOR__

#include "LoopConsumer.hpp"

// Posetilac koji dodaje korak petlje
class PrepForVisitor : public LoopVisitor<PrepForVisitor> {
public:
  // Inicijalizacija prepisivaca i konteksta
  PrepForVisitor(Rewriter &R, ASTContext &A)
    : LoopVisitor(R, A) {}
  
  // Dodavanje inkrementa pre continue
  bool VisitContinueStmt(ContinueStmt *s);
};

#endif
