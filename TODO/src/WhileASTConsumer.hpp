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
#include "ASTConsumers.hpp"

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
   return true;
  }
  
  // Pretvaranje for petlji u while
bool VisitForStmt(ForStmt *s) {
    Stmt* telo;
    if (s->getInc() != nullptr) {
      telo = CompoundStmt::Create(TheASTContext,
                 std::vector<Stmt*>{s->getBody(), s->getInc()},
                 SourceLocation(), SourceLocation());
    } else {
      telo = s->getBody();
    }
    
    /*Ukoliko postoji continu naredba */
    Stmt* blok;
    ContinueStmt continuestmt = ContinueStmt(SourceLocation());
    if (s->getInc() != nullptr) {
      blok = CompoundStmt::Create(TheASTContext,
                 std::vector<Stmt*>{s->getInc(), &continuestmt},
                 SourceLocation(), SourceLocation());
    } else {
      blok = &continuestmt;
    }
    
    
    Expr* cond = s->getCond();
    if (cond == nullptr) {
      const auto tip = TheASTContext.IntTy;
      llvm::APInt APValue(TheASTContext.getTypeSize(tip), 1);
      cond = IntegerLiteral::Create(TheASTContext, APValue,
                                    tip, SourceLocation());
    }else{
        cond = s->getCond();
    }
    
    WhileStmt petlja(TheASTContext, s->getConditionVariable(), cond, telo, SourceLocation());
    Stmt* initpet;
    if (s->getInit() != nullptr) {
      initpet = CompoundStmt::Create(TheASTContext,
                    std::vector<Stmt*>{s->getInit(), &petlja},
                    SourceLocation(), SourceLocation());
    }else{
        initpet = &petlja;
    }

    
    zameni(s, initpet);
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

