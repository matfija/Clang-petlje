#include "Helpers.hpp"

// Tekstualna reprezentacija naredbe
std::string Helpers::stampaj(Stmt* s) {
  std::string stmt;
  llvm::raw_string_ostream stream(stmt);
  s->printPretty(stream, nullptr, PrintingPolicy(LangOptions()));
  stream.flush();
  stmt.pop_back();
  return stmt;
}

// Tekstualna zamena koda
void Helpers::zameni(Stmt* stari, Stmt* novi) {
  // Tekstualna reprezentacija novog iskaza
  std::string stmt = stampaj(novi);
  
  // Granicne oznake u kodu
  const auto start = stari->getSourceRange().getBegin();
  const auto end = stari->getSourceRange().getEnd();
  
  // Dohvatanje poslednjeg tokena
  Token tok;
  Lexer::getRawToken(end, tok, TheRewriter.getSourceMgr(),
                     TheRewriter.getLangOpts());
  std::string ime = tok.getName();
  
  // Racunanje offseta osim ukoliko je kraj slozene naredbe;
  // tada je tacka-zapeta ili zatvorena zagrada suvisni token
  const auto offset = Lexer::MeasureTokenLength(end,
                          TheRewriter.getSourceMgr(),
                          TheRewriter.getLangOpts())
                          + (ime != "r_brace" && ime != "semi");
  
  // Promena teksta na izracunatom mestu
  SourceRange sr(start, end.getLocWithOffset(offset));
  TheRewriter.ReplaceText(sr, stmt);
}
