#include "PrepForConsumer.hpp"

// Dodavanje inkrementa pre continue
bool PrepForVisitor::VisitContinueStmt(ContinueStmt *s) {
  // Inicijalizacija nove naredbe i
  // continue kao njenog drugog dela
  Stmt *stmt = s;
  ContinueStmt c{SourceLocation()};

  // Prolazak kroz roditelje tekuceg continue
  auto rod = TheASTContext.getParents(*s);
  while (!rod.empty()) {
    // Izdvajanje glavnog roditelja
    const auto *r = rod.begin()->get<Stmt>();
    
    // Odustajanje ako je do ili while
    if (isa<DoStmt>(r) || isa<WhileStmt>(r))
      return true;
    
    // Uzimanje roditelja koji je for
    if (auto rr = dyn_cast<ForStmt>(r)) {
      // Odustajanje ako nema inkrement
      if (!rr->getInc())
        return true;
      
      // Pravljenje nove naredbe
      stmt = CompoundStmt::Create(TheASTContext,
                 std::vector<Stmt*>{(Stmt*)rr->getInc(), &c},
                 SourceLocation(), SourceLocation());
      break;
    }
    
    // Nastavljanje dalje
    rod = TheASTContext.getParents(*r);
  }
  
  // Tekstualna zamena koda
  zameni(s, stmt);
  
  // Nastavljanje dalje
  return true;
}

// Svaka deklaracija obradjuje se zasebno
bool PrepForConsumer::HandleTopLevelDecl(DeclGroupRef DR) {
  for (auto &x: DR)
    Visitor.TraverseDecl(x);
  return true;
}
