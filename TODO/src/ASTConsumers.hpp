#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <string>
#include <system_error>

// Ukljucivanje Clangovih biblioteka
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/PrettyPrinter.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Parse/ParseAST.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "llvm/Support/raw_ostream.h"

// Upotreba Clangovog imenskog prostora
using namespace clang;


std::string stampaj(Stmt* s) {
  	std::string stmt;
  	llvm::raw_string_ostream stream(stmt);
  	s->printPretty(stream, nullptr, PrintingPolicy(LangOptions()));
  	stream.flush();
  	stmt.pop_back();
  	return stmt;
  }
  // Tekstualna zamena koda
void zameni(Stmt* stari, Stmt* novi) {
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
static int  labelCounter = 0;

