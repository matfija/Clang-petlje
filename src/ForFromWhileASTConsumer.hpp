#ifndef __FORWHILE_AST__
#define __FORWHILE_AST__ 1

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

// Posetilac koji sve petlje pretvara u for
class ForFromWhileASTVisitor : public RecursiveASTVisitor<ForFromWhileASTVisitor>, public Helpers {
public:
  // Inicijalizacija prepisivaca i konteksta
  ForFromWhileASTVisitor(Rewriter &R, ASTContext &A)
    : Helpers(R, A)
    {}
  
  // Pretvaranje while petlji u for
  bool VisitWhileStmt(WhileStmt *s);

  // Prekid obilaska kod while petlje
  bool TraverseWhileStmt(WhileStmt* s);
};

// Klasa za obradu dobijenog AST stabla
class ForFromWhileASTConsumer : public ASTConsumer {
public:
  // Konstruktor inicijalizuje posetioca
  // prepisivacem i kontekstom parsiranja
  ForFromWhileASTConsumer(Rewriter &R, ASTContext &A)
    : Visitor(R, A) {}

  // Svaka deklaracija obradjuje se zasebno
  bool HandleTopLevelDecl(DeclGroupRef DR) override;
private:
  // Privatno cuvanje posetioca stabla
  ForFromWhileASTVisitor Visitor;
};

#endif