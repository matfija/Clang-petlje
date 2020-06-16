#include "Do2ForConsumer.hpp"

// Posetilac koji do pretvara u for
bool Do2ForVisitor::VisitDoStmt(DoStmt *s) {
  return true;
}

// Prekid obilaska kod do petlje
bool Do2ForVisitor::TraverseDoStmt(DoStmt* s) {
  return WalkUpFromDoStmt(s);
}

// Svaka deklaracija obradjuje se zasebno
bool Do2ForConsumer::HandleTopLevelDecl(DeclGroupRef DR) {
  for (auto &x: DR)
    Visitor.TraverseDecl(x);
  return true;
}
