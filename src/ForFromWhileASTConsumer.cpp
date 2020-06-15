#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <string>
#include <system_error>
#include "ForFromWhileASTConsumer.hpp"
  
 // Pretvaranje while petlji u for
 bool ForFromWhileASTVisitor::VisitWhileStmt(WhileStmt *s) {
   ForStmt petlja(TheASTContext, nullptr, s->getCond(), s->getConditionVariable(), nullptr, s->getBody(),
                SourceLocation(), SourceLocation(), SourceLocation());
    
   zameni(s, &petlja);
    
   return true;
 }

 // Prekid obilaska kod while petlje
 bool ForFromWhileASTVisitor::TraverseWhileStmt(WhileStmt* s) {
   return WalkUpFromWhileStmt(s);
 }

bool ForFromWhileASTConsumer::HandleTopLevelDecl(DeclGroupRef DR) {
   for (auto &x: DR)
     Visitor.TraverseDecl(x);
   return true;
 }