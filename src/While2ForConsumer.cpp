#include "While2ForConsumer.hpp"

// Pretvaranje while petlji u for
bool While2ForVisitor::VisitWhileStmt(WhileStmt *s) {
  ForStmt petlja(TheASTContext, nullptr, s->getCond(), nullptr, nullptr, s->getBody(),
                SourceLocation(), SourceLocation(), SourceLocation());
  
  zameni(s, &petlja);
  return true;
}

// Prekid obilaska kod while petlje
bool While2ForVisitor::TraverseWhileStmt(WhileStmt* s) {
  return WalkUpFromWhileStmt(s);
}

bool While2ForConsumer::HandleTopLevelDecl(DeclGroupRef DR) {
  for (auto &x: DR)
    Visitor.TraverseDecl(x);
  return true;
}
