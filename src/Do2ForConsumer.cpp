#include "Do2ForConsumer.hpp"

// Pretvaranje do-while petlji u for
bool Do2ForVisitor::VisitDoStmt(DoStmt *s) {
  // Nastavljanje dalje
  return true;
}

// Prekid obilaska kod do petlje
bool Do2ForVisitor::TraverseDoStmt(DoStmt* s) {
  return WalkUpFromDoStmt(s);
}

bool Do2ForConsumer::HandleTopLevelDecl(DeclGroupRef DR) {
  for (auto &x: DR)
    Visitor.TraverseDecl(x);
  return true;
}
