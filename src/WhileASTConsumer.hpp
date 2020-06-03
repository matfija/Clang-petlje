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

// Upotreba Clangovog imenskog prostora
using namespace clang;

// Posetilac koji sve petlje pretvara u while
class WhileASTVisitor : public RecursiveASTVisitor<WhileASTVisitor> {
public:
  // Inicijalizacija prepisivaca i konteksta
  WhileASTVisitor(Rewriter &R, ASTContext &A)
    : TheRewriter(R), TheASTContext(A) {}
  
  // Pretvaranje do-while petlji u while
  bool VisitDoStmt(DoStmt *s) {
    // Nastavljanje dalje
    return true;
  }
  
  // Pretvaranje for petlji u while
  bool VisitForStmt(ForStmt *s) {
    // Nastavljanje dalje
    return true;
  }
  
  // Prekid obilaska kod do petlje
  bool TraverseDoStmt(DoStmt* s) {
    return WalkUpFromDoStmt(s);
  }
  
  // Prekid obilaska kod for petlje
  bool TraverseForStmt(ForStmt* s) {
    return WalkUpFromForStmt(s);
  }

private:
  // Privatno cuvanje prepisivaca i konteksta
  Rewriter &TheRewriter;
  ASTContext &TheASTContext;
};

// Klasa za obradu dobijenog AST stabla
class WhileASTConsumer : public ASTConsumer {
public:
  // Konstruktor inicijalizuje posetioca
  // prepisivacem i kontekstom parsiranja
  WhileASTConsumer(Rewriter &R, ASTContext &A)
    : Visitor(R, A) {}

  // Svaka deklaracija obradjuje se zasebno
  bool HandleTopLevelDecl(DeclGroupRef DR) override {
    for (auto &x: DR)
      Visitor.TraverseDecl(x);
    return true;
  }

private:
  // Privatno cuvanje posetioca stabla
  WhileASTVisitor Visitor;
};

