#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <string>
#include <system_error>
#include "ForFromDoASTConsumer.hpp"


// Pretvaranje do-while petlji u for
 bool ForFromDoASTVisitor::VisitDoStmt(DoStmt *s) {
   // Nastavljanje dalje
   return true;
 }
  
 // Prekid obilaska kod do petlje
 bool ForFromDoASTVisitor::TraverseDoStmt(DoStmt* s) {
   return WalkUpFromDoStmt(s);
 }

bool ForFromDoASTConsumer::HandleTopLevelDecl(DeclGroupRef DR) {
   for (auto &x: DR)
     Visitor.TraverseDecl(x);
   return true;
 }