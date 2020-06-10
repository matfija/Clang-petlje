#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <string>
#include <system_error>
#include "ForASTConsumer.hpp"


// Pretvaranje do-while petlji u for
 bool ForASTVisitor::VisitDoStmt(DoStmt *s) {
   // Nastavljanje dalje
   return true;
 }
  
 // Pretvaranje while petlji u for
 bool ForASTVisitor::VisitWhileStmt(WhileStmt *s) {
   // Nastavljanje dalje
   return true;
 }
  
 // Prekid obilaska kod do petlje
 bool ForASTVisitor::TraverseDoStmt(DoStmt* s) {
   return WalkUpFromDoStmt(s);
 }
  
 // Prekid obilaska kod while petlje
 bool ForASTVisitor::TraverseWhileStmt(WhileStmt* s) {
   return WalkUpFromWhileStmt(s);
 }

bool ForASTConsumer::HandleTopLevelDecl(DeclGroupRef DR) {
   for (auto &x: DR)
     Visitor.TraverseDecl(x);
   return true;
 }