#include "While2ForConsumer.hpp"

// Posetilac koji while pretvara u for
bool While2ForVisitor::VisitWhileStmt(WhileStmt *s) {
  // Formiranje nove for petlje
  ForStmt petlja(TheASTContext, nullptr, s->getCond(), nullptr, nullptr, s->getBody(),
                SourceLocation(), SourceLocation(), SourceLocation());
  
  // Tekstualna zamena koda
  zameni(s, &petlja);
  
  // Nastavljanje dalje
  return true;
}

// Prekid obilaska kod while petlje
bool While2ForVisitor::TraverseWhileStmt(WhileStmt* s) {
  return WalkUpFromWhileStmt(s);
}

// Svaka deklaracija obradjuje se zasebno
bool While2ForConsumer::HandleTopLevelDecl(DeclGroupRef DR) {
  for (auto &x: DR)
    Visitor.TraverseDecl(x);
  return true;
}
