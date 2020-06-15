#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <string>
#include <system_error>
#include "WhileFromDoASTConsumer.hpp"
 #include "clang/AST/ExprOpenMP.h"
 #include "clang/Sema/DeclSpec.h"
 #include "clang/AST/ASTContext.h"
 #include "clang/AST/DeclCXX.h"
 #include "clang/AST/DeclObjC.h"
 #include "clang/AST/DeclTemplate.h"
 #include "clang/AST/StmtVisitor.h"
 #include "clang/Lex/Token.h"



using namespace clang;

// Pretvaranje do-while petlji u for
 bool WhileFromDoASTVisitor::VisitDoStmt(DoStmt *s) {

   Expr* cond = s->getCond();
   WhileStmt petlja(TheASTContext, nullptr, cond, s->getBody(), SourceLocation());
   //const SourceManager *SM = result.SourceManager;

//Use LLVM's lexer to get source text.
   //llvm::StringRef ref = Lexer::getSourceText(CharSourceRange::getCharRange(range), *SM, LangOptions());
   //cout << ref.str() << endl;
   
   
   //llvm::outs() << Rewriter::ConvertToString(s->getBody()) << "\n";
   zameni(s, &petlja);
   return true;
 }
  
 // Prekid obilaska kod do petlje
 bool WhileFromDoASTVisitor::TraverseDoStmt(DoStmt* s) {
   return WalkUpFromDoStmt(s);
 }

bool WhileFromDoASTConsumer::HandleTopLevelDecl(DeclGroupRef DR) {
   for (auto &x: DR)
     Visitor.TraverseDecl(x);
   return true;
 }