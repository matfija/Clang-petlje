// Ukljucivanje implementiranih klasa
#include "While2DoConsumer.hpp"
#include "Do2ForConsumer.hpp"
#include "While2ForConsumer.hpp"
#include "For2WhileConsumer.hpp"
#include "For2DoConsumer.hpp"
#include "PrepForConsumer.hpp"

// Enumeracija akcija
enum Akcija {
  While2Do,
  Do2For,
  While2For,
  PrepFor,
  For2While,
  For2Do
};

// Nacin upotrebe programa
std::string stari, novi, petlja;
const auto upotreba = "Upotreba: ./petlje <stari> <novi> <do|while|for>\n";

// Obrada prema zeljenoj akciji
void obradi(const Akcija &akcija) {
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
    switch (akcija) {
      case While2Do:
        TheConsumer = new While2DoConsumer(TheRewriter, TheASTContext);
        break;
      case Do2For:
        TheConsumer = new Do2ForConsumer(TheRewriter, TheASTContext);
        break;
      case While2For:
        TheConsumer = new While2ForConsumer(TheRewriter, TheASTContext);
        break;
      case PrepFor:
        TheConsumer = new PrepForConsumer(TheRewriter, TheASTContext);
        break;
      case For2While:
        TheConsumer = new For2WhileConsumer(TheRewriter, TheASTContext);
        break;
      case For2Do:
        TheConsumer = new For2DoConsumer(TheRewriter, TheASTContext);
        break;
      default:
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
    
    // Priprema je jednoprolazna
    if (akcija == Akcija::PrepFor)
      break;
  }
}

// Boilerplate kod za rad sa AST
int main(int argc, char *argv[]) {
  // Prekid pogresno pokrenutog programa
  if (argc != 4) {
    llvm::errs() << upotreba;
    exit(EXIT_FAILURE);
  }
  
  // Citanje argumenata
  stari = argv[1];
  novi = argv[2];
  petlja = argv[3];
  
  // Prvi deo algoritma
  if (petlja == "do")
    obradi(While2Do);
  else
    obradi(Do2For);
  
  // Drugi deo algoritma
  if (petlja == "for")
    obradi(While2For);
  else
    obradi(PrepFor);
  
  // Treci deo algoritma
  if (petlja == "while")
    obradi(For2While);
  else if (petlja == "do")
    obradi(For2Do);
  
  // Lepo formatiranje novog koda
  std::ostringstream buffer;
  buffer << "clang-format -i " << novi;
  std::system(buffer.str().c_str());
  
  // Normalno zavrsavanje programa
  exit(EXIT_SUCCESS);
}

