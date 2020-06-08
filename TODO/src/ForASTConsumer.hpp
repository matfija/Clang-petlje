// Ukljucivanje standardnih biblioteka
#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include <cctype>
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
#include "clang/Basic/IdentifierTable.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Parse/ParseAST.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/SmallVector.h"
#include "clang/Index/USRGeneration.h"
#include "ASTConsumers.hpp"

// Upotreba Clangovog imenskog prostora
using namespace clang;
IdentifierTable it = IdentifierTable(LangOptions(), nullptr);
int i = 0;
// Posetilac koji sve petlje pretvara u for
class ForASTVisitor : public RecursiveASTVisitor<ForASTVisitor> {
public:
  // Inicijalizacija prepisivaca i konteksta
  ForASTVisitor(Rewriter &R, ASTContext &A)
    : TheRewriter(R), TheASTContext(A) {}


   std::string stampaj(Stmt* s) {
  	std::string stmt;
  	llvm::raw_string_ostream stream(stmt);
  	s->printPretty(stream, nullptr, PrintingPolicy(LangOptions()));
  	stream.flush();
  	stmt.pop_back();
  	return stmt;
  }
      
  // Pretvaranje do-while petlji u for
  bool VisitDoStmt(DoStmt *s) {
    
    IdentifierTable it = IdentifierTable(LangOptions(), nullptr);
    std::string ime;
 
    
    ime = "l" + std::to_string(LabelCounter);
    LabelCounter++;
    llvm::StringRef str = StringRef(ime);

    LabelDecl* labelDecl = LabelDecl::Create(TheASTContext, nullptr, SourceLocation(),&it.getOwn(ime));

    LabelStmt lab(SourceLocation(), labelDecl, s->getBody());
     

    Stmt* blok1 = CompoundStmt::Create(TheASTContext,
                    std::vector<Stmt*>{&lab},
                    SourceLocation(), SourceLocation());
     
    ForStmt petlja(TheASTContext,nullptr,s->getCond(), nullptr, nullptr, blok1,
                SourceLocation(), SourceLocation(), SourceLocation());
    
    GotoStmt goTo(labelDecl, SourceLocation(), SourceLocation());
  
    
    Stmt* blok = CompoundStmt::Create(TheASTContext,
                    std::vector<Stmt*>{&goTo, &petlja},
                    SourceLocation(), SourceLocation());
    
    zameni(s, blok);
    return true;
  }
  
  // Pretvaranje while petlji u for
  bool VisitWhileStmt(WhileStmt *s) {
    
    ForStmt petlja(TheASTContext, nullptr, s->getCond(), s->getConditionVariable(), nullptr, s->getBody(),
                SourceLocation(), SourceLocation(), SourceLocation());
    
    zameni(s, &petlja);
    
    return true;
  }
  
  // Prekid obilaska kod do petlje
  bool TraverseDoStmt(DoStmt* s) {
    return WalkUpFromDoStmt(s);
  }
  
  // Prekid obilaska kod while petlje
  bool TraverseWhileStmt(WhileStmt* s) {
    return WalkUpFromWhileStmt(s);
  }

private:
  // Privatno cuvanje prepisivaca i konteksta
  Rewriter &TheRewriter;
  ASTContext &TheASTContext;
};

// Klasa za obradu dobijenog AST stabla
class ForASTConsumer : public ASTConsumer {
public:
  // Konstruktor inicijalizuje posetioca
  // prepisivacem i kontekstom parsiranja
  ForASTConsumer(Rewriter &R, ASTContext &A)
    : Visitor(R, A) {}

  // Svaka deklaracija obradjuje se zasebno
  bool HandleTopLevelDecl(DeclGroupRef DR) override {
    for (auto &x: DR)
      Visitor.TraverseDecl(x);
    return true;
  }

private:
  // Privatno cuvanje posetioca stabla
  ForASTVisitor Visitor;
};

