#ifndef __DO_AST__
#define __DO_AST__ 1

// Ukljucivanje standardnih biblioteka
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
#include "Helpers.hpp"


// Upotreba Clangovog imenskog prostora
using namespace clang;

// Posetilac koji sve petlje pretvara u do-while
class DoASTVisitor : public RecursiveASTVisitor<DoASTVisitor>, public Helpers {
public:
  // Inicijalizacija prepisivaca i konteksta
  DoASTVisitor(Rewriter &R, ASTContext &A)
    :Helpers(R, A) {}

  // Pretvaranje while petlji u do-while
  bool VisitWhileStmt(WhileStmt *s);
  
  // Pretvaranje for petlji u do-while
  bool VisitForStmt(ForStmt *s);
  
  // Prekid obilaska kod while petlje
  bool TraverseWhileStmt(WhileStmt* s);
  
  // Prekid obilaska kod for petlje
  bool TraverseForStmt(ForStmt* s);
};

// Klasa za obradu dobijenog AST stabla
class DoASTConsumer : public ASTConsumer {
public:
  // Konstruktor inicijalizuje posetioca
  // prepisivacem i kontekstom parsiranja
  DoASTConsumer(Rewriter &R, ASTContext &A)
    : Visitor(R, A) {}

  // Svaka deklaracija obradjuje se zasebno
  bool HandleTopLevelDecl(DeclGroupRef DR) override;

private:
  // Privatno cuvanje posetioca stabla
  DoASTVisitor Visitor;
};

#endif