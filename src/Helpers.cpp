#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <string>
#include <system_error>
#include "Helpers.hpp"

using namespace clang;

std::string Helpers::stampaj(Stmt* s) {
  	std::string stmt;
  	llvm::raw_string_ostream stream(stmt);
  	s->printPretty(stream, nullptr, PrintingPolicy(LangOptions()));
  	stream.flush();
  	stmt.pop_back();
  	return stmt;
}

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
    
    // Racunanje offseta osim ukoliko je kraj slozene naredbe,
    // sto znaci sledi tacka-zapeta kao suvisni token
    const auto offset = Lexer::MeasureTokenLength(end,
                            TheRewriter.getSourceMgr(),
                            TheRewriter.getLangOpts())
                            + (ime != "r_brace" && ime != "semi");
    
    // Promena teksta na izracunatom mestu
    SourceRange sr(start, end.getLocWithOffset(offset));
    TheRewriter.ReplaceText(sr, stmt);
  }