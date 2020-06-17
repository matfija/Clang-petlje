#include "While2DoConsumer.hpp"

// Posetilac koji while pretvara u do
bool While2DoVisitor::VisitWhileStmt(WhileStmt *s) {
  // Odgovarajuca do-while verzija
  DoStmt petlja(s->getBody(), s->getCond(),
                SourceLocation(), SourceLocation(), SourceLocation());
  
  // If iskaz za proveru uslova petlje
  IfStmt uslov(TheASTContext, SourceLocation(),
               false, nullptr, nullptr, s->getCond(), &petlja);
  
  // Tekstualna zamena koda
  zameni(s, &uslov);
  
  return true;
}

// Prekid obilaska kod while petlje
bool While2DoVisitor::TraverseWhileStmt(WhileStmt *s) {
  return WalkUpFromWhileStmt(s);
}
