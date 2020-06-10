// Ukljucivanje implementiranih klasa
#include "Helpers.hpp"
#include "DoASTConsumer.hpp"
#include "WhileASTConsumer.hpp"
#include "ForASTConsumer.hpp"

// Nacin upotrebe programa
const auto upotreba = "Upotreba: ./petlje <stari> <novi> [do|while|for]\n";

// Boilerplate kod za rad sa AST
int main(int argc, char *argv[]) {
  // Prekid pogresno pokrenutog programa
  if (argc != 4) {
    llvm::errs() << upotreba;
    exit(EXIT_FAILURE);
  }
  
  // Citranje argumenata
  auto stari = std::string(argv[1]);
  const auto novi = std::string(argv[2]);
  const auto petlja = std::string(argv[3]);

  // Parsiranje dokle god ima promena
  for(;;) {
    // Pravljenje i inicijalizacija prevodioca
    CompilerInstance TheCompInst;
    TheCompInst.createDiagnostics();
    auto &TheDiagnostics = TheCompInst.getDiagnostics();
    TheDiagnostics.setSuppressAllDiagnostics();

    // Postavljanje podataka o ciljnoj platformi
    auto TO = std::make_shared<TargetOptions>();
    TO->Triple = llvm::sys::getDefaultTargetTriple();
    auto *TI = TargetInfo::CreateTargetInfo(TheDiagnostics, TO);
    TheCompInst.setTarget(TI);
    
    // Pravljenje upravljaca datoteka
    TheCompInst.createFileManager();
    auto &FileMgr = TheCompInst.getFileManager();
    
    // Pravljenje upravljaca izvora
    TheCompInst.createSourceManager(FileMgr);
    auto &SourceMgr = TheCompInst.getSourceManager();
    
    // Pravljenje pretprocesora
    TheCompInst.createPreprocessor(TU_Module);
    auto &ThePreprocessor = TheCompInst.getPreprocessor();
    
    // Pravljenje AST konteksta
    TheCompInst.createASTContext();

    // Pravljenje prepisivaca koda
    Rewriter TheRewriter;
    TheRewriter.setSourceMgr(SourceMgr, TheCompInst.getLangOpts());

    // Postavljanje prosledjenog fajla za ulazni
    const auto *FileIn = FileMgr.getFile(stari);
    SourceMgr.setMainFileID(
        SourceMgr.createFileID(FileIn, SourceLocation(), SrcMgr::C_User));
    TheCompInst.getDiagnosticClient().BeginSourceFile(
        TheCompInst.getLangOpts(), &ThePreprocessor);

    // Odabir petlje u koju se ostale menjaju
    auto &TheASTContext = TheCompInst.getASTContext();
    ASTConsumer* TheConsumer;
    if (petlja == "do") {
      TheConsumer = new DoASTConsumer(TheRewriter, TheASTContext);
    } else if (petlja == "while") {
      TheConsumer = new WhileASTConsumer(TheRewriter, TheASTContext);
    } else if (petlja == "for") {
      TheConsumer = new ForASTConsumer(TheRewriter, TheASTContext);
    } else {
      llvm::errs() << upotreba;
      exit(EXIT_FAILURE);
    }
    
    // Parsiranje i obrada AST stabla
    ParseAST(ThePreprocessor, TheConsumer, TheASTContext);
    delete TheConsumer;

    // Upisivanje novog koda iz bafera; u slucaju da nema
    // izmena, prosto prepisivanje starog koda
    const auto *RewriteBuf =
        TheRewriter.getRewriteBufferFor(SourceMgr.getMainFileID());
    if (RewriteBuf != nullptr) {
      std::ofstream outFile(novi);
      outFile << std::string(RewriteBuf->begin(), RewriteBuf->end());
    } else if (stari == novi) {
      break;
    } else {
      std::ifstream inFile(stari);
      std::ofstream outFile(novi);
      std::ostringstream buffer;
      buffer << inFile.rdbuf();
      outFile << buffer.str();
      break;
    }
    
    // Zamena starog fajla
    stari = novi;
  }
  
  // Lepo formatiranje novog koda
  std::ostringstream buffer;
  buffer << "clang-format -i " << novi;
  std::system(buffer.str().c_str());
  
  // Normalno zavrsavanje programa
  exit(EXIT_SUCCESS);
}

