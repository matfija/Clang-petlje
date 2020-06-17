#include "PrepForConsumer.hpp"

// Dodavanje koraka for petlje pre continue
bool PrepForVisitor::VisitContinueStmt(ContinueStmt *s) {
  // Inicijalizacija nove naredbe
  Stmt *stmt = s;

  // Prolazak kroz roditelje tekuceg continue
  auto rod = TheASTContext.getParents(*s);
  while (!rod.empty()) {
    // Izdvajanje glavnog roditelja
    const auto *r = rod.begin()->get<Stmt>();
    
    // Odustajanje ako je do ili while
    if (isa<DoStmt>(r) || isa<WhileStmt>(r))
      return true;
    
    // Uzimanje roditelja koji je for
    if (auto *rr = dyn_cast<ForStmt>(r)) {
      // Odustajanje ako nema inkrement
      if (!rr->getInc())
        return true;
      
      // Pravljenje nove naredbe
      stmt = CompoundStmt::Create(TheASTContext,
                 std::vector<Stmt *>{(Stmt *)rr->getInc(), s},
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
