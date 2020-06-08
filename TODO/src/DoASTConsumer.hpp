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

// Posetilac koji sve petlje pretvara u do-while
class DoASTVisitor : public RecursiveASTVisitor<DoASTVisitor> {
public:
  // Inicijalizacija prepisivaca i konteksta
  DoASTVisitor(Rewriter &R, ASTContext &A)
    : TheRewriter(R), TheASTContext(A) {}
    
  // Pretvaranje while petlji u do-while
  bool VisitWhileStmt(WhileStmt *s) {
    // Odgovarajuca do-while verzija
    DoStmt petlja(s->getBody(), s->getCond(),
                  SourceLocation(), SourceLocation(), SourceLocation());
    
    // If iskaz za proveru uslova petlje
    IfStmt uslov(TheASTContext, SourceLocation(),
                 false, nullptr, nullptr, s->getCond(), &petlja);
    
    // Tekstualna zamena koda
    zameni(s, &uslov);
    
    // Nastavljanje dalje
    return true;
  }
  
  // Pretvaranje for petlji u do-while
  bool VisitForStmt(ForStmt *s) {
    // Slozeni iskaz sa telom i inkrementacijom
    // ili samo telo ako nema inkrementacije
    Stmt* telo;
    if (s->getInc() != nullptr) {
      telo = CompoundStmt::Create(TheASTContext,
                 std::vector<Stmt*>{s->getBody(), s->getInc()},
                 SourceLocation(), SourceLocation());
    } else {
      telo = s->getBody();
    }
    
    // Do-while petlja sa novim telom i uslovom
    // ili beskonacna petlja ako nema uslova
    Expr* cond = s->getCond();
    if (cond == nullptr) {
      const auto tip = TheASTContext.IntTy;
      llvm::APInt APValue(TheASTContext.getTypeSize(tip), 1);
      cond = IntegerLiteral::Create(TheASTContext, APValue,
                                    tip, SourceLocation());
    }
    DoStmt petlja(telo, cond, SourceLocation(),
                  SourceLocation(), SourceLocation());
    
    // If iskaz za proveru uslova petlje
    IfStmt uslov(TheASTContext, SourceLocation(),
                 false, nullptr, nullptr, cond, &petlja);
    
    // Slozeni iskaz sa inicijalizacijom i petljom
    // ili samo petlja ako nema inicijalizacije
    Stmt* initpet;
    if (s->getInit() != nullptr) {
      initpet = CompoundStmt::Create(TheASTContext,
                    std::vector<Stmt*>{s->getInit(), &uslov},
                    SourceLocation(), SourceLocation());
    } else {
      initpet = &uslov;
    }
    
    // Tekstualna zamena koda
    zameni(s, initpet);
    
    // Nastavljanje dalje
    return true;
  }
  
  // Prekid obilaska kod while petlje
  bool TraverseWhileStmt(WhileStmt* s) {
    return WalkUpFromWhileStmt(s);
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
class DoASTConsumer : public ASTConsumer {
public:
  // Konstruktor inicijalizuje posetioca
  // prepisivacem i kontekstom parsiranja
  DoASTConsumer(Rewriter &R, ASTContext &A)
    : Visitor(R, A) {}

  // Svaka deklaracija obradjuje se zasebno
  bool HandleTopLevelDecl(DeclGroupRef DR) override {
    for (auto &x: DR)
      Visitor.TraverseDecl(x);
    return true;
  }

private:
  // Privatno cuvanje posetioca stabla
  DoASTVisitor Visitor;
};

