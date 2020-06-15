#ifndef __FORDO_AST__
#define __FORDO_AST__ 1

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
class ForFromDoASTVisitor : public RecursiveASTVisitor<ForFromDoASTVisitor>, public Helpers {
public:
  // Inicijalizacija prepisivaca i konteksta
  ForFromDoASTVisitor(Rewriter &R, ASTContext &A)
    : Helpers(R, A)
    {}
  
  // Pretvaranje do-while petlji u for
  bool VisitDoStmt(DoStmt *s);
  
  // Prekid obilaska kod do petlje
  bool TraverseDoStmt(DoStmt* s);

};

// Klasa za obradu dobijenog AST stabla
class ForFromDoASTConsumer : public ASTConsumer {
public:
  // Konstruktor inicijalizuje posetioca
  // prepisivacem i kontekstom parsiranja
  ForFromDoASTConsumer(Rewriter &R, ASTContext &A)
    : Visitor(R, A) {}

  // Svaka deklaracija obradjuje se zasebno
  bool HandleTopLevelDecl(DeclGroupRef DR) override;
private:
  // Privatno cuvanje posetioca stabla
  ForFromDoASTVisitor Visitor;
};

#endif