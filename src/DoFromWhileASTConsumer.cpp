#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <string>
#include <system_error>
#include "DoFromWhileASTConsumer.hpp"

// Upotreba Clangovog imenskog prostora
using namespace clang;


bool DoFromWhileASTVisitor::VisitWhileStmt(WhileStmt *s) {
    // Odgovarajuca do-while verzija
    DoStmt petlja(s->getBody(), s->getCond(),
                  SourceLocation(), SourceLocation(), SourceLocation());
    
    // If iskaz za proveru uslova petlje
    IfStmt uslov(TheASTContext, SourceLocation(),
                 false, nullptr, nullptr, s->getCond(), &petlja);
    
    // Tekstualna zamena koda
    zameni(s, &uslov);
    //radi();
    // Nastavljanje dalje
    return true;
}

// Prekid obilaska kod while petlje
bool DoFromWhileASTVisitor::TraverseWhileStmt(WhileStmt* s) {
    return WalkUpFromWhileStmt(s);
}

bool DoFromWhileASTConsumer::HandleTopLevelDecl(DeclGroupRef DR) {
    for (auto &x: DR)
      Visitor.TraverseDecl(x);
    return true;
}