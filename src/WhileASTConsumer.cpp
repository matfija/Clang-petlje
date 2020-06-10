#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <string>
#include <system_error>
#include "WhileASTConsumer.hpp"


// Pretvaranje do-while petlji u for
 bool WhileASTVisitor::VisitDoStmt(DoStmt *s) {
   // Nastavljanje dalje
   return true;
 }
  
 // Pretvaranje while petlji u for
 bool WhileASTVisitor::VisitForStmt(ForStmt *s) {
   // Nastavljanje dalje
   return true;
 }
  
 // Prekid obilaska kod do petlje
 bool WhileASTVisitor::TraverseDoStmt(DoStmt* s) {
   return WalkUpFromDoStmt(s);
 }
  
 // Prekid obilaska kod while petlje
 bool WhileASTVisitor::TraverseForStmt(ForStmt* s) {
   return WalkUpFromForStmt(s);
 }

bool WhileASTConsumer::HandleTopLevelDecl(DeclGroupRef DR) {
   for (auto &x: DR)
     Visitor.TraverseDecl(x);
   return true;
 }