#ifndef __HELPERS_HPP__
#define __HELPERS_HPP__ 1

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

class Helpers {
public:
  Helpers(Rewriter &R, ASTContext &A)
    : TheRewriter(R), TheASTContext(A)
      {}
  
  // Dohvatanje tekstualne reprezentacije iskaza
  std::string stampaj(Stmt* s);
  
  // Tekstualna zamena koda
  void zameni(Stmt* stari, Stmt* novi);

protected:
  Rewriter &TheRewriter;
  ASTContext &TheASTContext;
};

#endif
