
// Generated from /Users/joakimstien/code/cish/cish/grammar/CM.g4 by ANTLR 4.13.2


#include "CMListener.h"
#include "CMVisitor.h"

#include "CMParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct CMParserStaticData final {
  CMParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  CMParserStaticData(const CMParserStaticData&) = delete;
  CMParserStaticData(CMParserStaticData&&) = delete;
  CMParserStaticData& operator=(const CMParserStaticData&) = delete;
  CMParserStaticData& operator=(CMParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag cmParserOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
std::unique_ptr<CMParserStaticData> cmParserStaticData = nullptr;

void cmParserInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (cmParserStaticData != nullptr) {
    return;
  }
#else
  assert(cmParserStaticData == nullptr);
#endif
  auto staticData = std::make_unique<CMParserStaticData>(
    std::vector<std::string>{
      "root", "rootBlock", "rootItem", "expression", "expr", "exprAtom", 
      "statement", "expressionStatement", "returnStatement", "ifStatement", 
      "elseStatement", "forStatement", "forInitializer", "forIterator", 
      "whileStatement", "doWhileStatement", "assignment", "arithmeticAssignment", 
      "variableDeclaration", "structDeclaration", "structFieldDeclaration", 
      "functionDeclaration", "functionDefinition", "functionCall", "expressionList", 
      "identifierList", "functionParameter", "identifier", "typeIdentifier", 
      "typeName", "sizeofTerm", "stringLiteral", "systemInclude", "sysModuleName"
    },
    std::vector<std::string>{
      "", "'('", "')'", "'.'", "'->'", "'['", "']'", "'++'", "'--'", "'!'", 
      "'~'", "'&'", "'sizeof'", "'<<'", "'>>'", "'^'", "'|'", "'return'", 
      "'if'", "'else'", "'for'", "'while'", "'do'", "'+='", "'-='", "'*='", 
      "'/='", "'%='", "'<<='", "'>>='", "'&='", "'^='", "'|='", "','", "'bool'", 
      "'char'", "'int'", "'long'", "'short'", "'float'", "'double'", "'void'", 
      "'#include'", "'*'", "'||'", "'&&'", "'=='", "'!='", "'>='", "'<='", 
      "'>'", "'<'", "'+'", "'-'", "", "'/'", "'%'", "'{'", "'}'", "';'", 
      "'='", "'NULL'", "'const'", "'struct'"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "Asterisk", "Or", "And", "Equals", 
      "NEquals", "GTEquals", "LTEquals", "GT", "LT", "Add", "Subtract", 
      "Multiply", "Divide", "Modulus", "OBrace", "CBrace", "SColon", "Assign", 
      "Null", "Const", "Struct", "Integer", "Floating", "Boolean", "String", 
      "Char", "Identifier", "Comment", "Space", "SysModuleName"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,72,431,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,7,
  	28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,1,0,1,0,1,0,1,1,
  	5,1,73,8,1,10,1,12,1,76,9,1,1,2,1,2,1,2,1,2,1,2,1,2,1,2,3,2,85,8,2,1,
  	3,1,3,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,
  	1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,3,4,116,8,4,1,4,1,4,1,4,1,
  	4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,
  	1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,5,4,151,8,4,10,4,12,4,
  	154,9,4,1,5,1,5,1,5,1,5,3,5,160,8,5,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,
  	6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,3,6,178,8,6,1,7,1,7,1,7,1,8,1,8,3,8,185,
  	8,8,1,8,1,8,1,9,1,9,1,9,1,9,1,9,1,9,5,9,195,8,9,10,9,12,9,198,9,9,1,9,
  	1,9,3,9,202,8,9,1,9,1,9,1,9,1,9,1,9,1,9,3,9,210,8,9,3,9,212,8,9,1,10,
  	1,10,1,10,5,10,217,8,10,10,10,12,10,220,9,10,1,10,1,10,1,10,3,10,225,
  	8,10,1,11,1,11,1,11,3,11,230,8,11,1,11,1,11,3,11,234,8,11,1,11,1,11,3,
  	11,238,8,11,1,11,1,11,1,11,5,11,243,8,11,10,11,12,11,246,9,11,1,11,1,
  	11,1,11,1,11,3,11,252,8,11,1,11,1,11,3,11,256,8,11,1,11,1,11,3,11,260,
  	8,11,1,11,1,11,3,11,264,8,11,1,12,1,12,3,12,268,8,12,1,13,1,13,1,13,3,
  	13,273,8,13,1,14,1,14,1,14,1,14,1,14,1,14,1,14,1,14,1,14,1,14,1,14,1,
  	14,5,14,287,8,14,10,14,12,14,290,9,14,1,14,1,14,3,14,294,8,14,1,15,1,
  	15,1,15,5,15,299,8,15,10,15,12,15,302,9,15,1,15,1,15,1,15,1,15,1,15,1,
  	15,1,15,1,16,1,16,1,16,1,16,1,17,1,17,1,17,1,17,1,18,1,18,1,18,1,18,3,
  	18,323,8,18,1,19,1,19,1,19,1,19,5,19,329,8,19,10,19,12,19,332,9,19,1,
  	19,1,19,1,19,1,20,1,20,1,20,1,20,1,21,1,21,1,21,1,21,1,21,1,21,1,21,1,
  	22,1,22,1,22,1,22,1,22,1,22,1,22,5,22,355,8,22,10,22,12,22,358,9,22,1,
  	22,1,22,1,23,1,23,1,23,1,23,1,23,1,24,1,24,1,24,5,24,370,8,24,10,24,12,
  	24,373,9,24,3,24,375,8,24,1,25,1,25,1,25,5,25,380,8,25,10,25,12,25,383,
  	9,25,3,25,385,8,25,1,26,1,26,3,26,389,8,26,1,27,1,27,1,28,3,28,394,8,
  	28,1,28,1,28,5,28,398,8,28,10,28,12,28,401,9,28,1,29,1,29,1,29,1,29,1,
  	29,1,29,1,29,1,29,1,29,1,29,3,29,413,8,29,1,30,1,30,1,30,1,30,1,30,1,
  	30,1,30,3,30,422,8,30,1,31,1,31,1,32,1,32,1,32,1,33,1,33,1,33,0,1,8,34,
  	0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,
  	50,52,54,56,58,60,62,64,66,0,10,2,0,43,43,55,56,1,0,52,53,1,0,13,14,1,
  	0,48,51,1,0,46,47,2,0,11,11,15,16,1,0,44,45,1,0,3,4,3,0,61,61,64,66,68,
  	68,1,0,23,32,476,0,68,1,0,0,0,2,74,1,0,0,0,4,84,1,0,0,0,6,86,1,0,0,0,
  	8,115,1,0,0,0,10,159,1,0,0,0,12,177,1,0,0,0,14,179,1,0,0,0,16,182,1,0,
  	0,0,18,211,1,0,0,0,20,224,1,0,0,0,22,263,1,0,0,0,24,267,1,0,0,0,26,272,
  	1,0,0,0,28,293,1,0,0,0,30,295,1,0,0,0,32,310,1,0,0,0,34,314,1,0,0,0,36,
  	318,1,0,0,0,38,324,1,0,0,0,40,336,1,0,0,0,42,340,1,0,0,0,44,347,1,0,0,
  	0,46,361,1,0,0,0,48,374,1,0,0,0,50,384,1,0,0,0,52,386,1,0,0,0,54,390,
  	1,0,0,0,56,393,1,0,0,0,58,412,1,0,0,0,60,421,1,0,0,0,62,423,1,0,0,0,64,
  	425,1,0,0,0,66,428,1,0,0,0,68,69,3,2,1,0,69,70,5,0,0,1,70,1,1,0,0,0,71,
  	73,3,4,2,0,72,71,1,0,0,0,73,76,1,0,0,0,74,72,1,0,0,0,74,75,1,0,0,0,75,
  	3,1,0,0,0,76,74,1,0,0,0,77,85,3,44,22,0,78,85,3,42,21,0,79,80,3,36,18,
  	0,80,81,5,59,0,0,81,85,1,0,0,0,82,85,3,64,32,0,83,85,3,38,19,0,84,77,
  	1,0,0,0,84,78,1,0,0,0,84,79,1,0,0,0,84,82,1,0,0,0,84,83,1,0,0,0,85,5,
  	1,0,0,0,86,87,3,8,4,0,87,7,1,0,0,0,88,89,6,4,-1,0,89,90,5,1,0,0,90,91,
  	3,8,4,0,91,92,5,2,0,0,92,116,1,0,0,0,93,94,5,7,0,0,94,116,3,8,4,17,95,
  	96,5,8,0,0,96,116,3,8,4,16,97,98,5,53,0,0,98,116,3,8,4,15,99,100,5,9,
  	0,0,100,116,3,8,4,14,101,102,5,10,0,0,102,116,3,8,4,13,103,104,5,1,0,
  	0,104,105,3,56,28,0,105,106,5,2,0,0,106,107,3,8,4,12,107,116,1,0,0,0,
  	108,109,5,43,0,0,109,116,3,8,4,11,110,111,5,11,0,0,111,116,3,8,4,10,112,
  	113,5,12,0,0,113,116,3,60,30,0,114,116,3,10,5,0,115,88,1,0,0,0,115,93,
  	1,0,0,0,115,95,1,0,0,0,115,97,1,0,0,0,115,99,1,0,0,0,115,101,1,0,0,0,
  	115,103,1,0,0,0,115,108,1,0,0,0,115,110,1,0,0,0,115,112,1,0,0,0,115,114,
  	1,0,0,0,116,152,1,0,0,0,117,118,10,8,0,0,118,119,7,0,0,0,119,151,3,8,
  	4,9,120,121,10,7,0,0,121,122,7,1,0,0,122,151,3,8,4,8,123,124,10,6,0,0,
  	124,125,7,2,0,0,125,151,3,8,4,7,126,127,10,5,0,0,127,128,7,3,0,0,128,
  	151,3,8,4,6,129,130,10,4,0,0,130,131,7,4,0,0,131,151,3,8,4,5,132,133,
  	10,3,0,0,133,134,7,5,0,0,134,151,3,8,4,4,135,136,10,2,0,0,136,137,7,6,
  	0,0,137,151,3,8,4,3,138,139,10,21,0,0,139,140,7,7,0,0,140,151,3,54,27,
  	0,141,142,10,20,0,0,142,143,5,5,0,0,143,144,3,8,4,0,144,145,5,6,0,0,145,
  	151,1,0,0,0,146,147,10,19,0,0,147,151,5,7,0,0,148,149,10,18,0,0,149,151,
  	5,8,0,0,150,117,1,0,0,0,150,120,1,0,0,0,150,123,1,0,0,0,150,126,1,0,0,
  	0,150,129,1,0,0,0,150,132,1,0,0,0,150,135,1,0,0,0,150,138,1,0,0,0,150,
  	141,1,0,0,0,150,146,1,0,0,0,150,148,1,0,0,0,151,154,1,0,0,0,152,150,1,
  	0,0,0,152,153,1,0,0,0,153,9,1,0,0,0,154,152,1,0,0,0,155,160,7,8,0,0,156,
  	160,5,69,0,0,157,160,3,46,23,0,158,160,3,62,31,0,159,155,1,0,0,0,159,
  	156,1,0,0,0,159,157,1,0,0,0,159,158,1,0,0,0,160,11,1,0,0,0,161,178,3,
  	18,9,0,162,163,3,32,16,0,163,164,5,59,0,0,164,178,1,0,0,0,165,166,3,36,
  	18,0,166,167,5,59,0,0,167,178,1,0,0,0,168,169,3,34,17,0,169,170,5,59,
  	0,0,170,178,1,0,0,0,171,178,3,16,8,0,172,178,3,22,11,0,173,178,3,28,14,
  	0,174,178,3,30,15,0,175,178,3,14,7,0,176,178,5,59,0,0,177,161,1,0,0,0,
  	177,162,1,0,0,0,177,165,1,0,0,0,177,168,1,0,0,0,177,171,1,0,0,0,177,172,
  	1,0,0,0,177,173,1,0,0,0,177,174,1,0,0,0,177,175,1,0,0,0,177,176,1,0,0,
  	0,178,13,1,0,0,0,179,180,3,6,3,0,180,181,5,59,0,0,181,15,1,0,0,0,182,
  	184,5,17,0,0,183,185,3,6,3,0,184,183,1,0,0,0,184,185,1,0,0,0,185,186,
  	1,0,0,0,186,187,5,59,0,0,187,17,1,0,0,0,188,189,5,18,0,0,189,190,5,1,
  	0,0,190,191,3,6,3,0,191,192,5,2,0,0,192,196,5,57,0,0,193,195,3,12,6,0,
  	194,193,1,0,0,0,195,198,1,0,0,0,196,194,1,0,0,0,196,197,1,0,0,0,197,199,
  	1,0,0,0,198,196,1,0,0,0,199,201,5,58,0,0,200,202,3,20,10,0,201,200,1,
  	0,0,0,201,202,1,0,0,0,202,212,1,0,0,0,203,204,5,18,0,0,204,205,5,1,0,
  	0,205,206,3,6,3,0,206,207,5,2,0,0,207,209,3,12,6,0,208,210,3,20,10,0,
  	209,208,1,0,0,0,209,210,1,0,0,0,210,212,1,0,0,0,211,188,1,0,0,0,211,203,
  	1,0,0,0,212,19,1,0,0,0,213,214,5,19,0,0,214,218,5,57,0,0,215,217,3,12,
  	6,0,216,215,1,0,0,0,217,220,1,0,0,0,218,216,1,0,0,0,218,219,1,0,0,0,219,
  	221,1,0,0,0,220,218,1,0,0,0,221,225,5,58,0,0,222,223,5,19,0,0,223,225,
  	3,12,6,0,224,213,1,0,0,0,224,222,1,0,0,0,225,21,1,0,0,0,226,227,5,20,
  	0,0,227,229,5,1,0,0,228,230,3,24,12,0,229,228,1,0,0,0,229,230,1,0,0,0,
  	230,231,1,0,0,0,231,233,5,59,0,0,232,234,3,6,3,0,233,232,1,0,0,0,233,
  	234,1,0,0,0,234,235,1,0,0,0,235,237,5,59,0,0,236,238,3,26,13,0,237,236,
  	1,0,0,0,237,238,1,0,0,0,238,239,1,0,0,0,239,240,5,2,0,0,240,244,5,57,
  	0,0,241,243,3,12,6,0,242,241,1,0,0,0,243,246,1,0,0,0,244,242,1,0,0,0,
  	244,245,1,0,0,0,245,247,1,0,0,0,246,244,1,0,0,0,247,264,5,58,0,0,248,
  	249,5,20,0,0,249,251,5,1,0,0,250,252,3,24,12,0,251,250,1,0,0,0,251,252,
  	1,0,0,0,252,253,1,0,0,0,253,255,5,59,0,0,254,256,3,6,3,0,255,254,1,0,
  	0,0,255,256,1,0,0,0,256,257,1,0,0,0,257,259,5,59,0,0,258,260,3,26,13,
  	0,259,258,1,0,0,0,259,260,1,0,0,0,260,261,1,0,0,0,261,262,5,2,0,0,262,
  	264,3,12,6,0,263,226,1,0,0,0,263,248,1,0,0,0,264,23,1,0,0,0,265,268,3,
  	36,18,0,266,268,3,32,16,0,267,265,1,0,0,0,267,266,1,0,0,0,268,25,1,0,
  	0,0,269,273,3,34,17,0,270,273,3,32,16,0,271,273,3,6,3,0,272,269,1,0,0,
  	0,272,270,1,0,0,0,272,271,1,0,0,0,273,27,1,0,0,0,274,275,5,21,0,0,275,
  	276,5,1,0,0,276,277,3,6,3,0,277,278,5,2,0,0,278,279,3,12,6,0,279,294,
  	1,0,0,0,280,281,5,21,0,0,281,282,5,1,0,0,282,283,3,6,3,0,283,284,5,2,
  	0,0,284,288,5,57,0,0,285,287,3,12,6,0,286,285,1,0,0,0,287,290,1,0,0,0,
  	288,286,1,0,0,0,288,289,1,0,0,0,289,291,1,0,0,0,290,288,1,0,0,0,291,292,
  	5,58,0,0,292,294,1,0,0,0,293,274,1,0,0,0,293,280,1,0,0,0,294,29,1,0,0,
  	0,295,296,5,22,0,0,296,300,5,57,0,0,297,299,3,12,6,0,298,297,1,0,0,0,
  	299,302,1,0,0,0,300,298,1,0,0,0,300,301,1,0,0,0,301,303,1,0,0,0,302,300,
  	1,0,0,0,303,304,5,58,0,0,304,305,5,21,0,0,305,306,5,1,0,0,306,307,3,6,
  	3,0,307,308,5,2,0,0,308,309,5,59,0,0,309,31,1,0,0,0,310,311,3,6,3,0,311,
  	312,5,60,0,0,312,313,3,6,3,0,313,33,1,0,0,0,314,315,3,6,3,0,315,316,7,
  	9,0,0,316,317,3,6,3,0,317,35,1,0,0,0,318,319,3,56,28,0,319,322,3,54,27,
  	0,320,321,5,60,0,0,321,323,3,6,3,0,322,320,1,0,0,0,322,323,1,0,0,0,323,
  	37,1,0,0,0,324,325,5,63,0,0,325,326,3,54,27,0,326,330,5,57,0,0,327,329,
  	3,40,20,0,328,327,1,0,0,0,329,332,1,0,0,0,330,328,1,0,0,0,330,331,1,0,
  	0,0,331,333,1,0,0,0,332,330,1,0,0,0,333,334,5,58,0,0,334,335,5,59,0,0,
  	335,39,1,0,0,0,336,337,3,56,28,0,337,338,3,54,27,0,338,339,5,59,0,0,339,
  	41,1,0,0,0,340,341,3,56,28,0,341,342,3,54,27,0,342,343,5,1,0,0,343,344,
  	3,50,25,0,344,345,5,2,0,0,345,346,5,59,0,0,346,43,1,0,0,0,347,348,3,56,
  	28,0,348,349,3,54,27,0,349,350,5,1,0,0,350,351,3,50,25,0,351,352,5,2,
  	0,0,352,356,5,57,0,0,353,355,3,12,6,0,354,353,1,0,0,0,355,358,1,0,0,0,
  	356,354,1,0,0,0,356,357,1,0,0,0,357,359,1,0,0,0,358,356,1,0,0,0,359,360,
  	5,58,0,0,360,45,1,0,0,0,361,362,3,54,27,0,362,363,5,1,0,0,363,364,3,48,
  	24,0,364,365,5,2,0,0,365,47,1,0,0,0,366,371,3,6,3,0,367,368,5,33,0,0,
  	368,370,3,6,3,0,369,367,1,0,0,0,370,373,1,0,0,0,371,369,1,0,0,0,371,372,
  	1,0,0,0,372,375,1,0,0,0,373,371,1,0,0,0,374,366,1,0,0,0,374,375,1,0,0,
  	0,375,49,1,0,0,0,376,381,3,52,26,0,377,378,5,33,0,0,378,380,3,52,26,0,
  	379,377,1,0,0,0,380,383,1,0,0,0,381,379,1,0,0,0,381,382,1,0,0,0,382,385,
  	1,0,0,0,383,381,1,0,0,0,384,376,1,0,0,0,384,385,1,0,0,0,385,51,1,0,0,
  	0,386,388,3,56,28,0,387,389,3,54,27,0,388,387,1,0,0,0,388,389,1,0,0,0,
  	389,53,1,0,0,0,390,391,5,69,0,0,391,55,1,0,0,0,392,394,5,62,0,0,393,392,
  	1,0,0,0,393,394,1,0,0,0,394,395,1,0,0,0,395,399,3,58,29,0,396,398,5,43,
  	0,0,397,396,1,0,0,0,398,401,1,0,0,0,399,397,1,0,0,0,399,400,1,0,0,0,400,
  	57,1,0,0,0,401,399,1,0,0,0,402,413,5,34,0,0,403,413,5,35,0,0,404,413,
  	5,36,0,0,405,413,5,37,0,0,406,413,5,38,0,0,407,413,5,39,0,0,408,413,5,
  	40,0,0,409,413,5,41,0,0,410,411,5,63,0,0,411,413,3,54,27,0,412,402,1,
  	0,0,0,412,403,1,0,0,0,412,404,1,0,0,0,412,405,1,0,0,0,412,406,1,0,0,0,
  	412,407,1,0,0,0,412,408,1,0,0,0,412,409,1,0,0,0,412,410,1,0,0,0,413,59,
  	1,0,0,0,414,415,5,1,0,0,415,416,3,60,30,0,416,417,5,2,0,0,417,422,1,0,
  	0,0,418,422,3,56,28,0,419,422,3,10,5,0,420,422,3,8,4,0,421,414,1,0,0,
  	0,421,418,1,0,0,0,421,419,1,0,0,0,421,420,1,0,0,0,422,61,1,0,0,0,423,
  	424,5,67,0,0,424,63,1,0,0,0,425,426,5,42,0,0,426,427,3,66,33,0,427,65,
  	1,0,0,0,428,429,5,72,0,0,429,67,1,0,0,0,39,74,84,115,150,152,159,177,
  	184,196,201,209,211,218,224,229,233,237,244,251,255,259,263,267,272,288,
  	293,300,322,330,356,371,374,381,384,388,393,399,412,421
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  cmParserStaticData = std::move(staticData);
}

}

CMParser::CMParser(TokenStream *input) : CMParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

CMParser::CMParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  CMParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *cmParserStaticData->atn, cmParserStaticData->decisionToDFA, cmParserStaticData->sharedContextCache, options);
}

CMParser::~CMParser() {
  delete _interpreter;
}

const atn::ATN& CMParser::getATN() const {
  return *cmParserStaticData->atn;
}

std::string CMParser::getGrammarFileName() const {
  return "CM.g4";
}

const std::vector<std::string>& CMParser::getRuleNames() const {
  return cmParserStaticData->ruleNames;
}

const dfa::Vocabulary& CMParser::getVocabulary() const {
  return cmParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView CMParser::getSerializedATN() const {
  return cmParserStaticData->serializedATN;
}


//----------------- RootContext ------------------------------------------------------------------

CMParser::RootContext::RootContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CMParser::RootBlockContext* CMParser::RootContext::rootBlock() {
  return getRuleContext<CMParser::RootBlockContext>(0);
}

tree::TerminalNode* CMParser::RootContext::EOF() {
  return getToken(CMParser::EOF, 0);
}


size_t CMParser::RootContext::getRuleIndex() const {
  return CMParser::RuleRoot;
}

void CMParser::RootContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterRoot(this);
}

void CMParser::RootContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitRoot(this);
}


std::any CMParser::RootContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitRoot(this);
  else
    return visitor->visitChildren(this);
}

CMParser::RootContext* CMParser::root() {
  RootContext *_localctx = _tracker.createInstance<RootContext>(_ctx, getState());
  enterRule(_localctx, 0, CMParser::RuleRoot);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(68);
    rootBlock();
    setState(69);
    match(CMParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RootBlockContext ------------------------------------------------------------------

CMParser::RootBlockContext::RootBlockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<CMParser::RootItemContext *> CMParser::RootBlockContext::rootItem() {
  return getRuleContexts<CMParser::RootItemContext>();
}

CMParser::RootItemContext* CMParser::RootBlockContext::rootItem(size_t i) {
  return getRuleContext<CMParser::RootItemContext>(i);
}


size_t CMParser::RootBlockContext::getRuleIndex() const {
  return CMParser::RuleRootBlock;
}

void CMParser::RootBlockContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterRootBlock(this);
}

void CMParser::RootBlockContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitRootBlock(this);
}


std::any CMParser::RootBlockContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitRootBlock(this);
  else
    return visitor->visitChildren(this);
}

CMParser::RootBlockContext* CMParser::rootBlock() {
  RootBlockContext *_localctx = _tracker.createInstance<RootBlockContext>(_ctx, getState());
  enterRule(_localctx, 2, CMParser::RuleRootBlock);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(74);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & -4611677239514234880) != 0)) {
      setState(71);
      rootItem();
      setState(76);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RootItemContext ------------------------------------------------------------------

CMParser::RootItemContext::RootItemContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CMParser::FunctionDefinitionContext* CMParser::RootItemContext::functionDefinition() {
  return getRuleContext<CMParser::FunctionDefinitionContext>(0);
}

CMParser::FunctionDeclarationContext* CMParser::RootItemContext::functionDeclaration() {
  return getRuleContext<CMParser::FunctionDeclarationContext>(0);
}

CMParser::VariableDeclarationContext* CMParser::RootItemContext::variableDeclaration() {
  return getRuleContext<CMParser::VariableDeclarationContext>(0);
}

tree::TerminalNode* CMParser::RootItemContext::SColon() {
  return getToken(CMParser::SColon, 0);
}

CMParser::SystemIncludeContext* CMParser::RootItemContext::systemInclude() {
  return getRuleContext<CMParser::SystemIncludeContext>(0);
}

CMParser::StructDeclarationContext* CMParser::RootItemContext::structDeclaration() {
  return getRuleContext<CMParser::StructDeclarationContext>(0);
}


size_t CMParser::RootItemContext::getRuleIndex() const {
  return CMParser::RuleRootItem;
}

void CMParser::RootItemContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterRootItem(this);
}

void CMParser::RootItemContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitRootItem(this);
}


std::any CMParser::RootItemContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitRootItem(this);
  else
    return visitor->visitChildren(this);
}

CMParser::RootItemContext* CMParser::rootItem() {
  RootItemContext *_localctx = _tracker.createInstance<RootItemContext>(_ctx, getState());
  enterRule(_localctx, 4, CMParser::RuleRootItem);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(84);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(77);
      functionDefinition();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(78);
      functionDeclaration();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(79);
      variableDeclaration();
      setState(80);
      match(CMParser::SColon);
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(82);
      systemInclude();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(83);
      structDeclaration();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExpressionContext ------------------------------------------------------------------

CMParser::ExpressionContext::ExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CMParser::ExprContext* CMParser::ExpressionContext::expr() {
  return getRuleContext<CMParser::ExprContext>(0);
}


size_t CMParser::ExpressionContext::getRuleIndex() const {
  return CMParser::RuleExpression;
}

void CMParser::ExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExpression(this);
}

void CMParser::ExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExpression(this);
}


std::any CMParser::ExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitExpression(this);
  else
    return visitor->visitChildren(this);
}

CMParser::ExpressionContext* CMParser::expression() {
  ExpressionContext *_localctx = _tracker.createInstance<ExpressionContext>(_ctx, getState());
  enterRule(_localctx, 6, CMParser::RuleExpression);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(86);
    expr(0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExprContext ------------------------------------------------------------------

CMParser::ExprContext::ExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CMParser::ExprContext::getRuleIndex() const {
  return CMParser::RuleExpr;
}

void CMParser::ExprContext::copyFrom(ExprContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- MINUS_EXPRContext ------------------------------------------------------------------

tree::TerminalNode* CMParser::MINUS_EXPRContext::Subtract() {
  return getToken(CMParser::Subtract, 0);
}

CMParser::ExprContext* CMParser::MINUS_EXPRContext::expr() {
  return getRuleContext<CMParser::ExprContext>(0);
}

CMParser::MINUS_EXPRContext::MINUS_EXPRContext(ExprContext *ctx) { copyFrom(ctx); }

void CMParser::MINUS_EXPRContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMINUS_EXPR(this);
}
void CMParser::MINUS_EXPRContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMINUS_EXPR(this);
}

std::any CMParser::MINUS_EXPRContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitMINUS_EXPR(this);
  else
    return visitor->visitChildren(this);
}
//----------------- MULT_EXPRContext ------------------------------------------------------------------

std::vector<CMParser::ExprContext *> CMParser::MULT_EXPRContext::expr() {
  return getRuleContexts<CMParser::ExprContext>();
}

CMParser::ExprContext* CMParser::MULT_EXPRContext::expr(size_t i) {
  return getRuleContext<CMParser::ExprContext>(i);
}

tree::TerminalNode* CMParser::MULT_EXPRContext::Asterisk() {
  return getToken(CMParser::Asterisk, 0);
}

tree::TerminalNode* CMParser::MULT_EXPRContext::Divide() {
  return getToken(CMParser::Divide, 0);
}

tree::TerminalNode* CMParser::MULT_EXPRContext::Modulus() {
  return getToken(CMParser::Modulus, 0);
}

CMParser::MULT_EXPRContext::MULT_EXPRContext(ExprContext *ctx) { copyFrom(ctx); }

void CMParser::MULT_EXPRContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMULT_EXPR(this);
}
void CMParser::MULT_EXPRContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMULT_EXPR(this);
}

std::any CMParser::MULT_EXPRContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitMULT_EXPR(this);
  else
    return visitor->visitChildren(this);
}
//----------------- NEGATION_EXPRContext ------------------------------------------------------------------

CMParser::ExprContext* CMParser::NEGATION_EXPRContext::expr() {
  return getRuleContext<CMParser::ExprContext>(0);
}

CMParser::NEGATION_EXPRContext::NEGATION_EXPRContext(ExprContext *ctx) { copyFrom(ctx); }

void CMParser::NEGATION_EXPRContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNEGATION_EXPR(this);
}
void CMParser::NEGATION_EXPRContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNEGATION_EXPR(this);
}

std::any CMParser::NEGATION_EXPRContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitNEGATION_EXPR(this);
  else
    return visitor->visitChildren(this);
}
//----------------- PREFIX_INC_EXPRContext ------------------------------------------------------------------

CMParser::ExprContext* CMParser::PREFIX_INC_EXPRContext::expr() {
  return getRuleContext<CMParser::ExprContext>(0);
}

CMParser::PREFIX_INC_EXPRContext::PREFIX_INC_EXPRContext(ExprContext *ctx) { copyFrom(ctx); }

void CMParser::PREFIX_INC_EXPRContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPREFIX_INC_EXPR(this);
}
void CMParser::PREFIX_INC_EXPRContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPREFIX_INC_EXPR(this);
}

std::any CMParser::PREFIX_INC_EXPRContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitPREFIX_INC_EXPR(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ONES_COMPLEMENT_EXPRContext ------------------------------------------------------------------

CMParser::ExprContext* CMParser::ONES_COMPLEMENT_EXPRContext::expr() {
  return getRuleContext<CMParser::ExprContext>(0);
}

CMParser::ONES_COMPLEMENT_EXPRContext::ONES_COMPLEMENT_EXPRContext(ExprContext *ctx) { copyFrom(ctx); }

void CMParser::ONES_COMPLEMENT_EXPRContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterONES_COMPLEMENT_EXPR(this);
}
void CMParser::ONES_COMPLEMENT_EXPRContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitONES_COMPLEMENT_EXPR(this);
}

std::any CMParser::ONES_COMPLEMENT_EXPRContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitONES_COMPLEMENT_EXPR(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ADD_EXPRContext ------------------------------------------------------------------

std::vector<CMParser::ExprContext *> CMParser::ADD_EXPRContext::expr() {
  return getRuleContexts<CMParser::ExprContext>();
}

CMParser::ExprContext* CMParser::ADD_EXPRContext::expr(size_t i) {
  return getRuleContext<CMParser::ExprContext>(i);
}

tree::TerminalNode* CMParser::ADD_EXPRContext::Add() {
  return getToken(CMParser::Add, 0);
}

tree::TerminalNode* CMParser::ADD_EXPRContext::Subtract() {
  return getToken(CMParser::Subtract, 0);
}

CMParser::ADD_EXPRContext::ADD_EXPRContext(ExprContext *ctx) { copyFrom(ctx); }

void CMParser::ADD_EXPRContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterADD_EXPR(this);
}
void CMParser::ADD_EXPRContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitADD_EXPR(this);
}

std::any CMParser::ADD_EXPRContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitADD_EXPR(this);
  else
    return visitor->visitChildren(this);
}
//----------------- AND_EXPRContext ------------------------------------------------------------------

std::vector<CMParser::ExprContext *> CMParser::AND_EXPRContext::expr() {
  return getRuleContexts<CMParser::ExprContext>();
}

CMParser::ExprContext* CMParser::AND_EXPRContext::expr(size_t i) {
  return getRuleContext<CMParser::ExprContext>(i);
}

tree::TerminalNode* CMParser::AND_EXPRContext::And() {
  return getToken(CMParser::And, 0);
}

tree::TerminalNode* CMParser::AND_EXPRContext::Or() {
  return getToken(CMParser::Or, 0);
}

CMParser::AND_EXPRContext::AND_EXPRContext(ExprContext *ctx) { copyFrom(ctx); }

void CMParser::AND_EXPRContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAND_EXPR(this);
}
void CMParser::AND_EXPRContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAND_EXPR(this);
}

std::any CMParser::AND_EXPRContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitAND_EXPR(this);
  else
    return visitor->visitChildren(this);
}
//----------------- POSTFIX_INC_EXPRContext ------------------------------------------------------------------

CMParser::ExprContext* CMParser::POSTFIX_INC_EXPRContext::expr() {
  return getRuleContext<CMParser::ExprContext>(0);
}

CMParser::POSTFIX_INC_EXPRContext::POSTFIX_INC_EXPRContext(ExprContext *ctx) { copyFrom(ctx); }

void CMParser::POSTFIX_INC_EXPRContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPOSTFIX_INC_EXPR(this);
}
void CMParser::POSTFIX_INC_EXPRContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPOSTFIX_INC_EXPR(this);
}

std::any CMParser::POSTFIX_INC_EXPRContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitPOSTFIX_INC_EXPR(this);
  else
    return visitor->visitChildren(this);
}
//----------------- SIZEOF_EXPRContext ------------------------------------------------------------------

CMParser::SizeofTermContext* CMParser::SIZEOF_EXPRContext::sizeofTerm() {
  return getRuleContext<CMParser::SizeofTermContext>(0);
}

CMParser::SIZEOF_EXPRContext::SIZEOF_EXPRContext(ExprContext *ctx) { copyFrom(ctx); }

void CMParser::SIZEOF_EXPRContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSIZEOF_EXPR(this);
}
void CMParser::SIZEOF_EXPRContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSIZEOF_EXPR(this);
}

std::any CMParser::SIZEOF_EXPRContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitSIZEOF_EXPR(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ADDROF_EXPRContext ------------------------------------------------------------------

CMParser::ExprContext* CMParser::ADDROF_EXPRContext::expr() {
  return getRuleContext<CMParser::ExprContext>(0);
}

CMParser::ADDROF_EXPRContext::ADDROF_EXPRContext(ExprContext *ctx) { copyFrom(ctx); }

void CMParser::ADDROF_EXPRContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterADDROF_EXPR(this);
}
void CMParser::ADDROF_EXPRContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitADDROF_EXPR(this);
}

std::any CMParser::ADDROF_EXPRContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitADDROF_EXPR(this);
  else
    return visitor->visitChildren(this);
}
//----------------- BITWISE_EXPRContext ------------------------------------------------------------------

std::vector<CMParser::ExprContext *> CMParser::BITWISE_EXPRContext::expr() {
  return getRuleContexts<CMParser::ExprContext>();
}

CMParser::ExprContext* CMParser::BITWISE_EXPRContext::expr(size_t i) {
  return getRuleContext<CMParser::ExprContext>(i);
}

CMParser::BITWISE_EXPRContext::BITWISE_EXPRContext(ExprContext *ctx) { copyFrom(ctx); }

void CMParser::BITWISE_EXPRContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBITWISE_EXPR(this);
}
void CMParser::BITWISE_EXPRContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBITWISE_EXPR(this);
}

std::any CMParser::BITWISE_EXPRContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitBITWISE_EXPR(this);
  else
    return visitor->visitChildren(this);
}
//----------------- POSTFIX_DEC_EXPRContext ------------------------------------------------------------------

CMParser::ExprContext* CMParser::POSTFIX_DEC_EXPRContext::expr() {
  return getRuleContext<CMParser::ExprContext>(0);
}

CMParser::POSTFIX_DEC_EXPRContext::POSTFIX_DEC_EXPRContext(ExprContext *ctx) { copyFrom(ctx); }

void CMParser::POSTFIX_DEC_EXPRContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPOSTFIX_DEC_EXPR(this);
}
void CMParser::POSTFIX_DEC_EXPRContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPOSTFIX_DEC_EXPR(this);
}

std::any CMParser::POSTFIX_DEC_EXPRContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitPOSTFIX_DEC_EXPR(this);
  else
    return visitor->visitChildren(this);
}
//----------------- DEREF_EXPRContext ------------------------------------------------------------------

tree::TerminalNode* CMParser::DEREF_EXPRContext::Asterisk() {
  return getToken(CMParser::Asterisk, 0);
}

CMParser::ExprContext* CMParser::DEREF_EXPRContext::expr() {
  return getRuleContext<CMParser::ExprContext>(0);
}

CMParser::DEREF_EXPRContext::DEREF_EXPRContext(ExprContext *ctx) { copyFrom(ctx); }

void CMParser::DEREF_EXPRContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDEREF_EXPR(this);
}
void CMParser::DEREF_EXPRContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDEREF_EXPR(this);
}

std::any CMParser::DEREF_EXPRContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitDEREF_EXPR(this);
  else
    return visitor->visitChildren(this);
}
//----------------- EQUALITY_EXPRContext ------------------------------------------------------------------

std::vector<CMParser::ExprContext *> CMParser::EQUALITY_EXPRContext::expr() {
  return getRuleContexts<CMParser::ExprContext>();
}

CMParser::ExprContext* CMParser::EQUALITY_EXPRContext::expr(size_t i) {
  return getRuleContext<CMParser::ExprContext>(i);
}

tree::TerminalNode* CMParser::EQUALITY_EXPRContext::Equals() {
  return getToken(CMParser::Equals, 0);
}

tree::TerminalNode* CMParser::EQUALITY_EXPRContext::NEquals() {
  return getToken(CMParser::NEquals, 0);
}

CMParser::EQUALITY_EXPRContext::EQUALITY_EXPRContext(ExprContext *ctx) { copyFrom(ctx); }

void CMParser::EQUALITY_EXPRContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterEQUALITY_EXPR(this);
}
void CMParser::EQUALITY_EXPRContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitEQUALITY_EXPR(this);
}

std::any CMParser::EQUALITY_EXPRContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitEQUALITY_EXPR(this);
  else
    return visitor->visitChildren(this);
}
//----------------- SUBSCRIPT_EXPRContext ------------------------------------------------------------------

std::vector<CMParser::ExprContext *> CMParser::SUBSCRIPT_EXPRContext::expr() {
  return getRuleContexts<CMParser::ExprContext>();
}

CMParser::ExprContext* CMParser::SUBSCRIPT_EXPRContext::expr(size_t i) {
  return getRuleContext<CMParser::ExprContext>(i);
}

CMParser::SUBSCRIPT_EXPRContext::SUBSCRIPT_EXPRContext(ExprContext *ctx) { copyFrom(ctx); }

void CMParser::SUBSCRIPT_EXPRContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSUBSCRIPT_EXPR(this);
}
void CMParser::SUBSCRIPT_EXPRContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSUBSCRIPT_EXPR(this);
}

std::any CMParser::SUBSCRIPT_EXPRContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitSUBSCRIPT_EXPR(this);
  else
    return visitor->visitChildren(this);
}
//----------------- EXPR_ATOM___Context ------------------------------------------------------------------

CMParser::ExprAtomContext* CMParser::EXPR_ATOM___Context::exprAtom() {
  return getRuleContext<CMParser::ExprAtomContext>(0);
}

CMParser::EXPR_ATOM___Context::EXPR_ATOM___Context(ExprContext *ctx) { copyFrom(ctx); }

void CMParser::EXPR_ATOM___Context::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterEXPR_ATOM___(this);
}
void CMParser::EXPR_ATOM___Context::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitEXPR_ATOM___(this);
}

std::any CMParser::EXPR_ATOM___Context::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitEXPR_ATOM___(this);
  else
    return visitor->visitChildren(this);
}
//----------------- PREFIX_DEC_EXPRContext ------------------------------------------------------------------

CMParser::ExprContext* CMParser::PREFIX_DEC_EXPRContext::expr() {
  return getRuleContext<CMParser::ExprContext>(0);
}

CMParser::PREFIX_DEC_EXPRContext::PREFIX_DEC_EXPRContext(ExprContext *ctx) { copyFrom(ctx); }

void CMParser::PREFIX_DEC_EXPRContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPREFIX_DEC_EXPR(this);
}
void CMParser::PREFIX_DEC_EXPRContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPREFIX_DEC_EXPR(this);
}

std::any CMParser::PREFIX_DEC_EXPRContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitPREFIX_DEC_EXPR(this);
  else
    return visitor->visitChildren(this);
}
//----------------- TYPE_CAST_EXPRContext ------------------------------------------------------------------

CMParser::TypeIdentifierContext* CMParser::TYPE_CAST_EXPRContext::typeIdentifier() {
  return getRuleContext<CMParser::TypeIdentifierContext>(0);
}

CMParser::ExprContext* CMParser::TYPE_CAST_EXPRContext::expr() {
  return getRuleContext<CMParser::ExprContext>(0);
}

CMParser::TYPE_CAST_EXPRContext::TYPE_CAST_EXPRContext(ExprContext *ctx) { copyFrom(ctx); }

void CMParser::TYPE_CAST_EXPRContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTYPE_CAST_EXPR(this);
}
void CMParser::TYPE_CAST_EXPRContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTYPE_CAST_EXPR(this);
}

std::any CMParser::TYPE_CAST_EXPRContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitTYPE_CAST_EXPR(this);
  else
    return visitor->visitChildren(this);
}
//----------------- BITSHIFT_EXPRContext ------------------------------------------------------------------

std::vector<CMParser::ExprContext *> CMParser::BITSHIFT_EXPRContext::expr() {
  return getRuleContexts<CMParser::ExprContext>();
}

CMParser::ExprContext* CMParser::BITSHIFT_EXPRContext::expr(size_t i) {
  return getRuleContext<CMParser::ExprContext>(i);
}

CMParser::BITSHIFT_EXPRContext::BITSHIFT_EXPRContext(ExprContext *ctx) { copyFrom(ctx); }

void CMParser::BITSHIFT_EXPRContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBITSHIFT_EXPR(this);
}
void CMParser::BITSHIFT_EXPRContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBITSHIFT_EXPR(this);
}

std::any CMParser::BITSHIFT_EXPRContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitBITSHIFT_EXPR(this);
  else
    return visitor->visitChildren(this);
}
//----------------- PAREN_EXPR___Context ------------------------------------------------------------------

CMParser::ExprContext* CMParser::PAREN_EXPR___Context::expr() {
  return getRuleContext<CMParser::ExprContext>(0);
}

CMParser::PAREN_EXPR___Context::PAREN_EXPR___Context(ExprContext *ctx) { copyFrom(ctx); }

void CMParser::PAREN_EXPR___Context::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPAREN_EXPR___(this);
}
void CMParser::PAREN_EXPR___Context::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPAREN_EXPR___(this);
}

std::any CMParser::PAREN_EXPR___Context::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitPAREN_EXPR___(this);
  else
    return visitor->visitChildren(this);
}
//----------------- COMPARE_EXPRContext ------------------------------------------------------------------

std::vector<CMParser::ExprContext *> CMParser::COMPARE_EXPRContext::expr() {
  return getRuleContexts<CMParser::ExprContext>();
}

CMParser::ExprContext* CMParser::COMPARE_EXPRContext::expr(size_t i) {
  return getRuleContext<CMParser::ExprContext>(i);
}

tree::TerminalNode* CMParser::COMPARE_EXPRContext::GTEquals() {
  return getToken(CMParser::GTEquals, 0);
}

tree::TerminalNode* CMParser::COMPARE_EXPRContext::LTEquals() {
  return getToken(CMParser::LTEquals, 0);
}

tree::TerminalNode* CMParser::COMPARE_EXPRContext::GT() {
  return getToken(CMParser::GT, 0);
}

tree::TerminalNode* CMParser::COMPARE_EXPRContext::LT() {
  return getToken(CMParser::LT, 0);
}

CMParser::COMPARE_EXPRContext::COMPARE_EXPRContext(ExprContext *ctx) { copyFrom(ctx); }

void CMParser::COMPARE_EXPRContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCOMPARE_EXPR(this);
}
void CMParser::COMPARE_EXPRContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCOMPARE_EXPR(this);
}

std::any CMParser::COMPARE_EXPRContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitCOMPARE_EXPR(this);
  else
    return visitor->visitChildren(this);
}
//----------------- STRUCT_ACCESS_EXPRContext ------------------------------------------------------------------

CMParser::ExprContext* CMParser::STRUCT_ACCESS_EXPRContext::expr() {
  return getRuleContext<CMParser::ExprContext>(0);
}

CMParser::IdentifierContext* CMParser::STRUCT_ACCESS_EXPRContext::identifier() {
  return getRuleContext<CMParser::IdentifierContext>(0);
}

CMParser::STRUCT_ACCESS_EXPRContext::STRUCT_ACCESS_EXPRContext(ExprContext *ctx) { copyFrom(ctx); }

void CMParser::STRUCT_ACCESS_EXPRContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSTRUCT_ACCESS_EXPR(this);
}
void CMParser::STRUCT_ACCESS_EXPRContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSTRUCT_ACCESS_EXPR(this);
}

std::any CMParser::STRUCT_ACCESS_EXPRContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitSTRUCT_ACCESS_EXPR(this);
  else
    return visitor->visitChildren(this);
}

CMParser::ExprContext* CMParser::expr() {
   return expr(0);
}

CMParser::ExprContext* CMParser::expr(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  CMParser::ExprContext *_localctx = _tracker.createInstance<ExprContext>(_ctx, parentState);
  CMParser::ExprContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 8;
  enterRecursionRule(_localctx, 8, CMParser::RuleExpr, precedence);

    size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(115);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<PAREN_EXPR___Context>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(89);
      match(CMParser::T__0);
      setState(90);
      expr(0);
      setState(91);
      match(CMParser::T__1);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<PREFIX_INC_EXPRContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(93);
      match(CMParser::T__6);
      setState(94);
      expr(17);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<PREFIX_DEC_EXPRContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(95);
      match(CMParser::T__7);
      setState(96);
      expr(16);
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<MINUS_EXPRContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(97);
      match(CMParser::Subtract);
      setState(98);
      expr(15);
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<NEGATION_EXPRContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(99);
      match(CMParser::T__8);
      setState(100);
      expr(14);
      break;
    }

    case 6: {
      _localctx = _tracker.createInstance<ONES_COMPLEMENT_EXPRContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(101);
      match(CMParser::T__9);
      setState(102);
      expr(13);
      break;
    }

    case 7: {
      _localctx = _tracker.createInstance<TYPE_CAST_EXPRContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(103);
      match(CMParser::T__0);
      setState(104);
      typeIdentifier();
      setState(105);
      match(CMParser::T__1);
      setState(106);
      expr(12);
      break;
    }

    case 8: {
      _localctx = _tracker.createInstance<DEREF_EXPRContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(108);
      match(CMParser::Asterisk);
      setState(109);
      expr(11);
      break;
    }

    case 9: {
      _localctx = _tracker.createInstance<ADDROF_EXPRContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(110);
      match(CMParser::T__10);
      setState(111);
      expr(10);
      break;
    }

    case 10: {
      _localctx = _tracker.createInstance<SIZEOF_EXPRContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(112);
      match(CMParser::T__11);
      setState(113);
      sizeofTerm();
      break;
    }

    case 11: {
      _localctx = _tracker.createInstance<EXPR_ATOM___Context>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(114);
      exprAtom();
      break;
    }

    default:
      break;
    }
    _ctx->stop = _input->LT(-1);
    setState(152);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 4, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(150);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 3, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<MULT_EXPRContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(117);

          if (!(precpred(_ctx, 8))) throw FailedPredicateException(this, "precpred(_ctx, 8)");
          setState(118);
          antlrcpp::downCast<MULT_EXPRContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & 108095187149914112) != 0))) {
            antlrcpp::downCast<MULT_EXPRContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(119);
          expr(9);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<ADD_EXPRContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(120);

          if (!(precpred(_ctx, 7))) throw FailedPredicateException(this, "precpred(_ctx, 7)");
          setState(121);
          antlrcpp::downCast<ADD_EXPRContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!(_la == CMParser::Add

          || _la == CMParser::Subtract)) {
            antlrcpp::downCast<ADD_EXPRContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(122);
          expr(8);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<BITSHIFT_EXPRContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(123);

          if (!(precpred(_ctx, 6))) throw FailedPredicateException(this, "precpred(_ctx, 6)");
          setState(124);
          antlrcpp::downCast<BITSHIFT_EXPRContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!(_la == CMParser::T__12

          || _la == CMParser::T__13)) {
            antlrcpp::downCast<BITSHIFT_EXPRContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(125);
          expr(7);
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<COMPARE_EXPRContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(126);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(127);
          antlrcpp::downCast<COMPARE_EXPRContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & 4222124650659840) != 0))) {
            antlrcpp::downCast<COMPARE_EXPRContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(128);
          expr(6);
          break;
        }

        case 5: {
          auto newContext = _tracker.createInstance<EQUALITY_EXPRContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(129);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(130);
          antlrcpp::downCast<EQUALITY_EXPRContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!(_la == CMParser::Equals

          || _la == CMParser::NEquals)) {
            antlrcpp::downCast<EQUALITY_EXPRContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(131);
          expr(5);
          break;
        }

        case 6: {
          auto newContext = _tracker.createInstance<BITWISE_EXPRContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(132);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(133);
          antlrcpp::downCast<BITWISE_EXPRContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & 100352) != 0))) {
            antlrcpp::downCast<BITWISE_EXPRContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(134);
          expr(4);
          break;
        }

        case 7: {
          auto newContext = _tracker.createInstance<AND_EXPRContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(135);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(136);
          antlrcpp::downCast<AND_EXPRContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!(_la == CMParser::Or

          || _la == CMParser::And)) {
            antlrcpp::downCast<AND_EXPRContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(137);
          expr(3);
          break;
        }

        case 8: {
          auto newContext = _tracker.createInstance<STRUCT_ACCESS_EXPRContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(138);

          if (!(precpred(_ctx, 21))) throw FailedPredicateException(this, "precpred(_ctx, 21)");
          setState(139);
          antlrcpp::downCast<STRUCT_ACCESS_EXPRContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!(_la == CMParser::T__2

          || _la == CMParser::T__3)) {
            antlrcpp::downCast<STRUCT_ACCESS_EXPRContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(140);
          identifier();
          break;
        }

        case 9: {
          auto newContext = _tracker.createInstance<SUBSCRIPT_EXPRContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(141);

          if (!(precpred(_ctx, 20))) throw FailedPredicateException(this, "precpred(_ctx, 20)");
          setState(142);
          match(CMParser::T__4);
          setState(143);
          expr(0);
          setState(144);
          match(CMParser::T__5);
          break;
        }

        case 10: {
          auto newContext = _tracker.createInstance<POSTFIX_INC_EXPRContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(146);

          if (!(precpred(_ctx, 19))) throw FailedPredicateException(this, "precpred(_ctx, 19)");
          setState(147);
          match(CMParser::T__6);
          break;
        }

        case 11: {
          auto newContext = _tracker.createInstance<POSTFIX_DEC_EXPRContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(148);

          if (!(precpred(_ctx, 18))) throw FailedPredicateException(this, "precpred(_ctx, 18)");
          setState(149);
          match(CMParser::T__7);
          break;
        }

        default:
          break;
        } 
      }
      setState(154);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 4, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- ExprAtomContext ------------------------------------------------------------------

CMParser::ExprAtomContext::ExprAtomContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CMParser::ExprAtomContext::getRuleIndex() const {
  return CMParser::RuleExprAtom;
}

void CMParser::ExprAtomContext::copyFrom(ExprAtomContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- LITERAL_EXPRContext ------------------------------------------------------------------

tree::TerminalNode* CMParser::LITERAL_EXPRContext::Char() {
  return getToken(CMParser::Char, 0);
}

tree::TerminalNode* CMParser::LITERAL_EXPRContext::Integer() {
  return getToken(CMParser::Integer, 0);
}

tree::TerminalNode* CMParser::LITERAL_EXPRContext::Floating() {
  return getToken(CMParser::Floating, 0);
}

tree::TerminalNode* CMParser::LITERAL_EXPRContext::Boolean() {
  return getToken(CMParser::Boolean, 0);
}

tree::TerminalNode* CMParser::LITERAL_EXPRContext::Null() {
  return getToken(CMParser::Null, 0);
}

CMParser::LITERAL_EXPRContext::LITERAL_EXPRContext(ExprAtomContext *ctx) { copyFrom(ctx); }

void CMParser::LITERAL_EXPRContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLITERAL_EXPR(this);
}
void CMParser::LITERAL_EXPRContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLITERAL_EXPR(this);
}

std::any CMParser::LITERAL_EXPRContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitLITERAL_EXPR(this);
  else
    return visitor->visitChildren(this);
}
//----------------- FUNC_CALL_EXPRContext ------------------------------------------------------------------

CMParser::FunctionCallContext* CMParser::FUNC_CALL_EXPRContext::functionCall() {
  return getRuleContext<CMParser::FunctionCallContext>(0);
}

CMParser::FUNC_CALL_EXPRContext::FUNC_CALL_EXPRContext(ExprAtomContext *ctx) { copyFrom(ctx); }

void CMParser::FUNC_CALL_EXPRContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFUNC_CALL_EXPR(this);
}
void CMParser::FUNC_CALL_EXPRContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFUNC_CALL_EXPR(this);
}

std::any CMParser::FUNC_CALL_EXPRContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitFUNC_CALL_EXPR(this);
  else
    return visitor->visitChildren(this);
}
//----------------- STR_LITERAL_EXPRContext ------------------------------------------------------------------

CMParser::StringLiteralContext* CMParser::STR_LITERAL_EXPRContext::stringLiteral() {
  return getRuleContext<CMParser::StringLiteralContext>(0);
}

CMParser::STR_LITERAL_EXPRContext::STR_LITERAL_EXPRContext(ExprAtomContext *ctx) { copyFrom(ctx); }

void CMParser::STR_LITERAL_EXPRContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSTR_LITERAL_EXPR(this);
}
void CMParser::STR_LITERAL_EXPRContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSTR_LITERAL_EXPR(this);
}

std::any CMParser::STR_LITERAL_EXPRContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitSTR_LITERAL_EXPR(this);
  else
    return visitor->visitChildren(this);
}
//----------------- VAR_REF_EXPRContext ------------------------------------------------------------------

tree::TerminalNode* CMParser::VAR_REF_EXPRContext::Identifier() {
  return getToken(CMParser::Identifier, 0);
}

CMParser::VAR_REF_EXPRContext::VAR_REF_EXPRContext(ExprAtomContext *ctx) { copyFrom(ctx); }

void CMParser::VAR_REF_EXPRContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterVAR_REF_EXPR(this);
}
void CMParser::VAR_REF_EXPRContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitVAR_REF_EXPR(this);
}

std::any CMParser::VAR_REF_EXPRContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitVAR_REF_EXPR(this);
  else
    return visitor->visitChildren(this);
}
CMParser::ExprAtomContext* CMParser::exprAtom() {
  ExprAtomContext *_localctx = _tracker.createInstance<ExprAtomContext>(_ctx, getState());
  enterRule(_localctx, 10, CMParser::RuleExprAtom);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(159);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<CMParser::LITERAL_EXPRContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(155);
      _la = _input->LA(1);
      if (!(((((_la - 61) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 61)) & 185) != 0))) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<CMParser::VAR_REF_EXPRContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(156);
      match(CMParser::Identifier);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<CMParser::FUNC_CALL_EXPRContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(157);
      functionCall();
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<CMParser::STR_LITERAL_EXPRContext>(_localctx);
      enterOuterAlt(_localctx, 4);
      setState(158);
      stringLiteral();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatementContext ------------------------------------------------------------------

CMParser::StatementContext::StatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CMParser::IfStatementContext* CMParser::StatementContext::ifStatement() {
  return getRuleContext<CMParser::IfStatementContext>(0);
}

CMParser::AssignmentContext* CMParser::StatementContext::assignment() {
  return getRuleContext<CMParser::AssignmentContext>(0);
}

tree::TerminalNode* CMParser::StatementContext::SColon() {
  return getToken(CMParser::SColon, 0);
}

CMParser::VariableDeclarationContext* CMParser::StatementContext::variableDeclaration() {
  return getRuleContext<CMParser::VariableDeclarationContext>(0);
}

CMParser::ArithmeticAssignmentContext* CMParser::StatementContext::arithmeticAssignment() {
  return getRuleContext<CMParser::ArithmeticAssignmentContext>(0);
}

CMParser::ReturnStatementContext* CMParser::StatementContext::returnStatement() {
  return getRuleContext<CMParser::ReturnStatementContext>(0);
}

CMParser::ForStatementContext* CMParser::StatementContext::forStatement() {
  return getRuleContext<CMParser::ForStatementContext>(0);
}

CMParser::WhileStatementContext* CMParser::StatementContext::whileStatement() {
  return getRuleContext<CMParser::WhileStatementContext>(0);
}

CMParser::DoWhileStatementContext* CMParser::StatementContext::doWhileStatement() {
  return getRuleContext<CMParser::DoWhileStatementContext>(0);
}

CMParser::ExpressionStatementContext* CMParser::StatementContext::expressionStatement() {
  return getRuleContext<CMParser::ExpressionStatementContext>(0);
}


size_t CMParser::StatementContext::getRuleIndex() const {
  return CMParser::RuleStatement;
}

void CMParser::StatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStatement(this);
}

void CMParser::StatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStatement(this);
}


std::any CMParser::StatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitStatement(this);
  else
    return visitor->visitChildren(this);
}

CMParser::StatementContext* CMParser::statement() {
  StatementContext *_localctx = _tracker.createInstance<StatementContext>(_ctx, getState());
  enterRule(_localctx, 12, CMParser::RuleStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(177);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 6, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(161);
      ifStatement();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(162);
      assignment();
      setState(163);
      match(CMParser::SColon);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(165);
      variableDeclaration();
      setState(166);
      match(CMParser::SColon);
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(168);
      arithmeticAssignment();
      setState(169);
      match(CMParser::SColon);
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(171);
      returnStatement();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(172);
      forStatement();
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(173);
      whileStatement();
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(174);
      doWhileStatement();
      break;
    }

    case 9: {
      enterOuterAlt(_localctx, 9);
      setState(175);
      expressionStatement();
      break;
    }

    case 10: {
      enterOuterAlt(_localctx, 10);
      setState(176);
      match(CMParser::SColon);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExpressionStatementContext ------------------------------------------------------------------

CMParser::ExpressionStatementContext::ExpressionStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CMParser::ExpressionContext* CMParser::ExpressionStatementContext::expression() {
  return getRuleContext<CMParser::ExpressionContext>(0);
}

tree::TerminalNode* CMParser::ExpressionStatementContext::SColon() {
  return getToken(CMParser::SColon, 0);
}


size_t CMParser::ExpressionStatementContext::getRuleIndex() const {
  return CMParser::RuleExpressionStatement;
}

void CMParser::ExpressionStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExpressionStatement(this);
}

void CMParser::ExpressionStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExpressionStatement(this);
}


std::any CMParser::ExpressionStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitExpressionStatement(this);
  else
    return visitor->visitChildren(this);
}

CMParser::ExpressionStatementContext* CMParser::expressionStatement() {
  ExpressionStatementContext *_localctx = _tracker.createInstance<ExpressionStatementContext>(_ctx, getState());
  enterRule(_localctx, 14, CMParser::RuleExpressionStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(179);
    expression();
    setState(180);
    match(CMParser::SColon);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ReturnStatementContext ------------------------------------------------------------------

CMParser::ReturnStatementContext::ReturnStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CMParser::ReturnStatementContext::SColon() {
  return getToken(CMParser::SColon, 0);
}

CMParser::ExpressionContext* CMParser::ReturnStatementContext::expression() {
  return getRuleContext<CMParser::ExpressionContext>(0);
}


size_t CMParser::ReturnStatementContext::getRuleIndex() const {
  return CMParser::RuleReturnStatement;
}

void CMParser::ReturnStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterReturnStatement(this);
}

void CMParser::ReturnStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitReturnStatement(this);
}


std::any CMParser::ReturnStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitReturnStatement(this);
  else
    return visitor->visitChildren(this);
}

CMParser::ReturnStatementContext* CMParser::returnStatement() {
  ReturnStatementContext *_localctx = _tracker.createInstance<ReturnStatementContext>(_ctx, getState());
  enterRule(_localctx, 16, CMParser::RuleReturnStatement);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(182);
    match(CMParser::T__16);
    setState(184);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 2314859004561465218) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & 63) != 0)) {
      setState(183);
      expression();
    }
    setState(186);
    match(CMParser::SColon);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IfStatementContext ------------------------------------------------------------------

CMParser::IfStatementContext::IfStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CMParser::ExpressionContext* CMParser::IfStatementContext::expression() {
  return getRuleContext<CMParser::ExpressionContext>(0);
}

tree::TerminalNode* CMParser::IfStatementContext::OBrace() {
  return getToken(CMParser::OBrace, 0);
}

tree::TerminalNode* CMParser::IfStatementContext::CBrace() {
  return getToken(CMParser::CBrace, 0);
}

std::vector<CMParser::StatementContext *> CMParser::IfStatementContext::statement() {
  return getRuleContexts<CMParser::StatementContext>();
}

CMParser::StatementContext* CMParser::IfStatementContext::statement(size_t i) {
  return getRuleContext<CMParser::StatementContext>(i);
}

CMParser::ElseStatementContext* CMParser::IfStatementContext::elseStatement() {
  return getRuleContext<CMParser::ElseStatementContext>(0);
}


size_t CMParser::IfStatementContext::getRuleIndex() const {
  return CMParser::RuleIfStatement;
}

void CMParser::IfStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIfStatement(this);
}

void CMParser::IfStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIfStatement(this);
}


std::any CMParser::IfStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitIfStatement(this);
  else
    return visitor->visitChildren(this);
}

CMParser::IfStatementContext* CMParser::ifStatement() {
  IfStatementContext *_localctx = _tracker.createInstance<IfStatementContext>(_ctx, getState());
  enterRule(_localctx, 18, CMParser::RuleIfStatement);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(211);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 11, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(188);
      match(CMParser::T__17);
      setState(189);
      match(CMParser::T__0);
      setState(190);
      expression();
      setState(191);
      match(CMParser::T__1);
      setState(192);
      match(CMParser::OBrace);
      setState(196);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & -1720361880688124030) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 64)) & 63) != 0)) {
        setState(193);
        statement();
        setState(198);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(199);
      match(CMParser::CBrace);
      setState(201);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 9, _ctx)) {
      case 1: {
        setState(200);
        elseStatement();
        break;
      }

      default:
        break;
      }
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(203);
      match(CMParser::T__17);
      setState(204);
      match(CMParser::T__0);
      setState(205);
      expression();
      setState(206);
      match(CMParser::T__1);
      setState(207);
      statement();
      setState(209);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 10, _ctx)) {
      case 1: {
        setState(208);
        elseStatement();
        break;
      }

      default:
        break;
      }
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ElseStatementContext ------------------------------------------------------------------

CMParser::ElseStatementContext::ElseStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CMParser::ElseStatementContext::OBrace() {
  return getToken(CMParser::OBrace, 0);
}

tree::TerminalNode* CMParser::ElseStatementContext::CBrace() {
  return getToken(CMParser::CBrace, 0);
}

std::vector<CMParser::StatementContext *> CMParser::ElseStatementContext::statement() {
  return getRuleContexts<CMParser::StatementContext>();
}

CMParser::StatementContext* CMParser::ElseStatementContext::statement(size_t i) {
  return getRuleContext<CMParser::StatementContext>(i);
}


size_t CMParser::ElseStatementContext::getRuleIndex() const {
  return CMParser::RuleElseStatement;
}

void CMParser::ElseStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterElseStatement(this);
}

void CMParser::ElseStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitElseStatement(this);
}


std::any CMParser::ElseStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitElseStatement(this);
  else
    return visitor->visitChildren(this);
}

CMParser::ElseStatementContext* CMParser::elseStatement() {
  ElseStatementContext *_localctx = _tracker.createInstance<ElseStatementContext>(_ctx, getState());
  enterRule(_localctx, 20, CMParser::RuleElseStatement);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(224);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 13, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(213);
      match(CMParser::T__18);
      setState(214);
      match(CMParser::OBrace);
      setState(218);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & -1720361880688124030) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 64)) & 63) != 0)) {
        setState(215);
        statement();
        setState(220);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(221);
      match(CMParser::CBrace);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(222);
      match(CMParser::T__18);
      setState(223);
      statement();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ForStatementContext ------------------------------------------------------------------

CMParser::ForStatementContext::ForStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> CMParser::ForStatementContext::SColon() {
  return getTokens(CMParser::SColon);
}

tree::TerminalNode* CMParser::ForStatementContext::SColon(size_t i) {
  return getToken(CMParser::SColon, i);
}

tree::TerminalNode* CMParser::ForStatementContext::OBrace() {
  return getToken(CMParser::OBrace, 0);
}

tree::TerminalNode* CMParser::ForStatementContext::CBrace() {
  return getToken(CMParser::CBrace, 0);
}

CMParser::ForInitializerContext* CMParser::ForStatementContext::forInitializer() {
  return getRuleContext<CMParser::ForInitializerContext>(0);
}

CMParser::ExpressionContext* CMParser::ForStatementContext::expression() {
  return getRuleContext<CMParser::ExpressionContext>(0);
}

CMParser::ForIteratorContext* CMParser::ForStatementContext::forIterator() {
  return getRuleContext<CMParser::ForIteratorContext>(0);
}

std::vector<CMParser::StatementContext *> CMParser::ForStatementContext::statement() {
  return getRuleContexts<CMParser::StatementContext>();
}

CMParser::StatementContext* CMParser::ForStatementContext::statement(size_t i) {
  return getRuleContext<CMParser::StatementContext>(i);
}


size_t CMParser::ForStatementContext::getRuleIndex() const {
  return CMParser::RuleForStatement;
}

void CMParser::ForStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterForStatement(this);
}

void CMParser::ForStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitForStatement(this);
}


std::any CMParser::ForStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitForStatement(this);
  else
    return visitor->visitChildren(this);
}

CMParser::ForStatementContext* CMParser::forStatement() {
  ForStatementContext *_localctx = _tracker.createInstance<ForStatementContext>(_ctx, getState());
  enterRule(_localctx, 22, CMParser::RuleForStatement);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(263);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 21, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(226);
      match(CMParser::T__19);
      setState(227);
      match(CMParser::T__0);
      setState(229);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & -2296822632999280766) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 64)) & 63) != 0)) {
        setState(228);
        forInitializer();
      }
      setState(231);
      match(CMParser::SColon);
      setState(233);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 2314859004561465218) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 64)) & 63) != 0)) {
        setState(232);
        expression();
      }
      setState(235);
      match(CMParser::SColon);
      setState(237);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 2314859004561465218) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 64)) & 63) != 0)) {
        setState(236);
        forIterator();
      }
      setState(239);
      match(CMParser::T__1);
      setState(240);
      match(CMParser::OBrace);
      setState(244);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & -1720361880688124030) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 64)) & 63) != 0)) {
        setState(241);
        statement();
        setState(246);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(247);
      match(CMParser::CBrace);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(248);
      match(CMParser::T__19);
      setState(249);
      match(CMParser::T__0);
      setState(251);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & -2296822632999280766) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 64)) & 63) != 0)) {
        setState(250);
        forInitializer();
      }
      setState(253);
      match(CMParser::SColon);
      setState(255);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 2314859004561465218) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 64)) & 63) != 0)) {
        setState(254);
        expression();
      }
      setState(257);
      match(CMParser::SColon);
      setState(259);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 2314859004561465218) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 64)) & 63) != 0)) {
        setState(258);
        forIterator();
      }
      setState(261);
      match(CMParser::T__1);
      setState(262);
      statement();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ForInitializerContext ------------------------------------------------------------------

CMParser::ForInitializerContext::ForInitializerContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CMParser::VariableDeclarationContext* CMParser::ForInitializerContext::variableDeclaration() {
  return getRuleContext<CMParser::VariableDeclarationContext>(0);
}

CMParser::AssignmentContext* CMParser::ForInitializerContext::assignment() {
  return getRuleContext<CMParser::AssignmentContext>(0);
}


size_t CMParser::ForInitializerContext::getRuleIndex() const {
  return CMParser::RuleForInitializer;
}

void CMParser::ForInitializerContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterForInitializer(this);
}

void CMParser::ForInitializerContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitForInitializer(this);
}


std::any CMParser::ForInitializerContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitForInitializer(this);
  else
    return visitor->visitChildren(this);
}

CMParser::ForInitializerContext* CMParser::forInitializer() {
  ForInitializerContext *_localctx = _tracker.createInstance<ForInitializerContext>(_ctx, getState());
  enterRule(_localctx, 24, CMParser::RuleForInitializer);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(267);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CMParser::T__33:
      case CMParser::T__34:
      case CMParser::T__35:
      case CMParser::T__36:
      case CMParser::T__37:
      case CMParser::T__38:
      case CMParser::T__39:
      case CMParser::T__40:
      case CMParser::Const:
      case CMParser::Struct: {
        enterOuterAlt(_localctx, 1);
        setState(265);
        variableDeclaration();
        break;
      }

      case CMParser::T__0:
      case CMParser::T__6:
      case CMParser::T__7:
      case CMParser::T__8:
      case CMParser::T__9:
      case CMParser::T__10:
      case CMParser::T__11:
      case CMParser::Asterisk:
      case CMParser::Subtract:
      case CMParser::Null:
      case CMParser::Integer:
      case CMParser::Floating:
      case CMParser::Boolean:
      case CMParser::String:
      case CMParser::Char:
      case CMParser::Identifier: {
        enterOuterAlt(_localctx, 2);
        setState(266);
        assignment();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ForIteratorContext ------------------------------------------------------------------

CMParser::ForIteratorContext::ForIteratorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CMParser::ArithmeticAssignmentContext* CMParser::ForIteratorContext::arithmeticAssignment() {
  return getRuleContext<CMParser::ArithmeticAssignmentContext>(0);
}

CMParser::AssignmentContext* CMParser::ForIteratorContext::assignment() {
  return getRuleContext<CMParser::AssignmentContext>(0);
}

CMParser::ExpressionContext* CMParser::ForIteratorContext::expression() {
  return getRuleContext<CMParser::ExpressionContext>(0);
}


size_t CMParser::ForIteratorContext::getRuleIndex() const {
  return CMParser::RuleForIterator;
}

void CMParser::ForIteratorContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterForIterator(this);
}

void CMParser::ForIteratorContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitForIterator(this);
}


std::any CMParser::ForIteratorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitForIterator(this);
  else
    return visitor->visitChildren(this);
}

CMParser::ForIteratorContext* CMParser::forIterator() {
  ForIteratorContext *_localctx = _tracker.createInstance<ForIteratorContext>(_ctx, getState());
  enterRule(_localctx, 26, CMParser::RuleForIterator);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(272);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 23, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(269);
      arithmeticAssignment();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(270);
      assignment();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(271);
      expression();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- WhileStatementContext ------------------------------------------------------------------

CMParser::WhileStatementContext::WhileStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CMParser::ExpressionContext* CMParser::WhileStatementContext::expression() {
  return getRuleContext<CMParser::ExpressionContext>(0);
}

std::vector<CMParser::StatementContext *> CMParser::WhileStatementContext::statement() {
  return getRuleContexts<CMParser::StatementContext>();
}

CMParser::StatementContext* CMParser::WhileStatementContext::statement(size_t i) {
  return getRuleContext<CMParser::StatementContext>(i);
}

tree::TerminalNode* CMParser::WhileStatementContext::OBrace() {
  return getToken(CMParser::OBrace, 0);
}

tree::TerminalNode* CMParser::WhileStatementContext::CBrace() {
  return getToken(CMParser::CBrace, 0);
}


size_t CMParser::WhileStatementContext::getRuleIndex() const {
  return CMParser::RuleWhileStatement;
}

void CMParser::WhileStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterWhileStatement(this);
}

void CMParser::WhileStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitWhileStatement(this);
}


std::any CMParser::WhileStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitWhileStatement(this);
  else
    return visitor->visitChildren(this);
}

CMParser::WhileStatementContext* CMParser::whileStatement() {
  WhileStatementContext *_localctx = _tracker.createInstance<WhileStatementContext>(_ctx, getState());
  enterRule(_localctx, 28, CMParser::RuleWhileStatement);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(293);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 25, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(274);
      match(CMParser::T__20);
      setState(275);
      match(CMParser::T__0);
      setState(276);
      expression();
      setState(277);
      match(CMParser::T__1);
      setState(278);
      statement();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(280);
      match(CMParser::T__20);
      setState(281);
      match(CMParser::T__0);
      setState(282);
      expression();
      setState(283);
      match(CMParser::T__1);
      setState(284);
      match(CMParser::OBrace);
      setState(288);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & -1720361880688124030) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 64)) & 63) != 0)) {
        setState(285);
        statement();
        setState(290);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(291);
      match(CMParser::CBrace);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DoWhileStatementContext ------------------------------------------------------------------

CMParser::DoWhileStatementContext::DoWhileStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CMParser::DoWhileStatementContext::OBrace() {
  return getToken(CMParser::OBrace, 0);
}

tree::TerminalNode* CMParser::DoWhileStatementContext::CBrace() {
  return getToken(CMParser::CBrace, 0);
}

CMParser::ExpressionContext* CMParser::DoWhileStatementContext::expression() {
  return getRuleContext<CMParser::ExpressionContext>(0);
}

tree::TerminalNode* CMParser::DoWhileStatementContext::SColon() {
  return getToken(CMParser::SColon, 0);
}

std::vector<CMParser::StatementContext *> CMParser::DoWhileStatementContext::statement() {
  return getRuleContexts<CMParser::StatementContext>();
}

CMParser::StatementContext* CMParser::DoWhileStatementContext::statement(size_t i) {
  return getRuleContext<CMParser::StatementContext>(i);
}


size_t CMParser::DoWhileStatementContext::getRuleIndex() const {
  return CMParser::RuleDoWhileStatement;
}

void CMParser::DoWhileStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDoWhileStatement(this);
}

void CMParser::DoWhileStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDoWhileStatement(this);
}


std::any CMParser::DoWhileStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitDoWhileStatement(this);
  else
    return visitor->visitChildren(this);
}

CMParser::DoWhileStatementContext* CMParser::doWhileStatement() {
  DoWhileStatementContext *_localctx = _tracker.createInstance<DoWhileStatementContext>(_ctx, getState());
  enterRule(_localctx, 30, CMParser::RuleDoWhileStatement);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(295);
    match(CMParser::T__21);
    setState(296);
    match(CMParser::OBrace);
    setState(300);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & -1720361880688124030) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & 63) != 0)) {
      setState(297);
      statement();
      setState(302);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(303);
    match(CMParser::CBrace);
    setState(304);
    match(CMParser::T__20);
    setState(305);
    match(CMParser::T__0);
    setState(306);
    expression();
    setState(307);
    match(CMParser::T__1);
    setState(308);
    match(CMParser::SColon);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AssignmentContext ------------------------------------------------------------------

CMParser::AssignmentContext::AssignmentContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<CMParser::ExpressionContext *> CMParser::AssignmentContext::expression() {
  return getRuleContexts<CMParser::ExpressionContext>();
}

CMParser::ExpressionContext* CMParser::AssignmentContext::expression(size_t i) {
  return getRuleContext<CMParser::ExpressionContext>(i);
}

tree::TerminalNode* CMParser::AssignmentContext::Assign() {
  return getToken(CMParser::Assign, 0);
}


size_t CMParser::AssignmentContext::getRuleIndex() const {
  return CMParser::RuleAssignment;
}

void CMParser::AssignmentContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAssignment(this);
}

void CMParser::AssignmentContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAssignment(this);
}


std::any CMParser::AssignmentContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitAssignment(this);
  else
    return visitor->visitChildren(this);
}

CMParser::AssignmentContext* CMParser::assignment() {
  AssignmentContext *_localctx = _tracker.createInstance<AssignmentContext>(_ctx, getState());
  enterRule(_localctx, 32, CMParser::RuleAssignment);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(310);
    expression();
    setState(311);
    match(CMParser::Assign);
    setState(312);
    expression();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ArithmeticAssignmentContext ------------------------------------------------------------------

CMParser::ArithmeticAssignmentContext::ArithmeticAssignmentContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<CMParser::ExpressionContext *> CMParser::ArithmeticAssignmentContext::expression() {
  return getRuleContexts<CMParser::ExpressionContext>();
}

CMParser::ExpressionContext* CMParser::ArithmeticAssignmentContext::expression(size_t i) {
  return getRuleContext<CMParser::ExpressionContext>(i);
}


size_t CMParser::ArithmeticAssignmentContext::getRuleIndex() const {
  return CMParser::RuleArithmeticAssignment;
}

void CMParser::ArithmeticAssignmentContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterArithmeticAssignment(this);
}

void CMParser::ArithmeticAssignmentContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitArithmeticAssignment(this);
}


std::any CMParser::ArithmeticAssignmentContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitArithmeticAssignment(this);
  else
    return visitor->visitChildren(this);
}

CMParser::ArithmeticAssignmentContext* CMParser::arithmeticAssignment() {
  ArithmeticAssignmentContext *_localctx = _tracker.createInstance<ArithmeticAssignmentContext>(_ctx, getState());
  enterRule(_localctx, 34, CMParser::RuleArithmeticAssignment);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(314);
    expression();
    setState(315);
    antlrcpp::downCast<ArithmeticAssignmentContext *>(_localctx)->op = _input->LT(1);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 8581545984) != 0))) {
      antlrcpp::downCast<ArithmeticAssignmentContext *>(_localctx)->op = _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(316);
    expression();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VariableDeclarationContext ------------------------------------------------------------------

CMParser::VariableDeclarationContext::VariableDeclarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CMParser::TypeIdentifierContext* CMParser::VariableDeclarationContext::typeIdentifier() {
  return getRuleContext<CMParser::TypeIdentifierContext>(0);
}

CMParser::IdentifierContext* CMParser::VariableDeclarationContext::identifier() {
  return getRuleContext<CMParser::IdentifierContext>(0);
}

tree::TerminalNode* CMParser::VariableDeclarationContext::Assign() {
  return getToken(CMParser::Assign, 0);
}

CMParser::ExpressionContext* CMParser::VariableDeclarationContext::expression() {
  return getRuleContext<CMParser::ExpressionContext>(0);
}


size_t CMParser::VariableDeclarationContext::getRuleIndex() const {
  return CMParser::RuleVariableDeclaration;
}

void CMParser::VariableDeclarationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterVariableDeclaration(this);
}

void CMParser::VariableDeclarationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitVariableDeclaration(this);
}


std::any CMParser::VariableDeclarationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitVariableDeclaration(this);
  else
    return visitor->visitChildren(this);
}

CMParser::VariableDeclarationContext* CMParser::variableDeclaration() {
  VariableDeclarationContext *_localctx = _tracker.createInstance<VariableDeclarationContext>(_ctx, getState());
  enterRule(_localctx, 36, CMParser::RuleVariableDeclaration);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(318);
    typeIdentifier();
    setState(319);
    identifier();
    setState(322);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CMParser::Assign) {
      setState(320);
      match(CMParser::Assign);
      setState(321);
      expression();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StructDeclarationContext ------------------------------------------------------------------

CMParser::StructDeclarationContext::StructDeclarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CMParser::StructDeclarationContext::Struct() {
  return getToken(CMParser::Struct, 0);
}

CMParser::IdentifierContext* CMParser::StructDeclarationContext::identifier() {
  return getRuleContext<CMParser::IdentifierContext>(0);
}

tree::TerminalNode* CMParser::StructDeclarationContext::OBrace() {
  return getToken(CMParser::OBrace, 0);
}

tree::TerminalNode* CMParser::StructDeclarationContext::CBrace() {
  return getToken(CMParser::CBrace, 0);
}

tree::TerminalNode* CMParser::StructDeclarationContext::SColon() {
  return getToken(CMParser::SColon, 0);
}

std::vector<CMParser::StructFieldDeclarationContext *> CMParser::StructDeclarationContext::structFieldDeclaration() {
  return getRuleContexts<CMParser::StructFieldDeclarationContext>();
}

CMParser::StructFieldDeclarationContext* CMParser::StructDeclarationContext::structFieldDeclaration(size_t i) {
  return getRuleContext<CMParser::StructFieldDeclarationContext>(i);
}


size_t CMParser::StructDeclarationContext::getRuleIndex() const {
  return CMParser::RuleStructDeclaration;
}

void CMParser::StructDeclarationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStructDeclaration(this);
}

void CMParser::StructDeclarationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStructDeclaration(this);
}


std::any CMParser::StructDeclarationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitStructDeclaration(this);
  else
    return visitor->visitChildren(this);
}

CMParser::StructDeclarationContext* CMParser::structDeclaration() {
  StructDeclarationContext *_localctx = _tracker.createInstance<StructDeclarationContext>(_ctx, getState());
  enterRule(_localctx, 38, CMParser::RuleStructDeclaration);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(324);
    match(CMParser::Struct);
    setState(325);
    identifier();
    setState(326);
    match(CMParser::OBrace);
    setState(330);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & -4611681637560745984) != 0)) {
      setState(327);
      structFieldDeclaration();
      setState(332);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(333);
    match(CMParser::CBrace);
    setState(334);
    match(CMParser::SColon);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StructFieldDeclarationContext ------------------------------------------------------------------

CMParser::StructFieldDeclarationContext::StructFieldDeclarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CMParser::TypeIdentifierContext* CMParser::StructFieldDeclarationContext::typeIdentifier() {
  return getRuleContext<CMParser::TypeIdentifierContext>(0);
}

CMParser::IdentifierContext* CMParser::StructFieldDeclarationContext::identifier() {
  return getRuleContext<CMParser::IdentifierContext>(0);
}

tree::TerminalNode* CMParser::StructFieldDeclarationContext::SColon() {
  return getToken(CMParser::SColon, 0);
}


size_t CMParser::StructFieldDeclarationContext::getRuleIndex() const {
  return CMParser::RuleStructFieldDeclaration;
}

void CMParser::StructFieldDeclarationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStructFieldDeclaration(this);
}

void CMParser::StructFieldDeclarationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStructFieldDeclaration(this);
}


std::any CMParser::StructFieldDeclarationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitStructFieldDeclaration(this);
  else
    return visitor->visitChildren(this);
}

CMParser::StructFieldDeclarationContext* CMParser::structFieldDeclaration() {
  StructFieldDeclarationContext *_localctx = _tracker.createInstance<StructFieldDeclarationContext>(_ctx, getState());
  enterRule(_localctx, 40, CMParser::RuleStructFieldDeclaration);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(336);
    typeIdentifier();
    setState(337);
    identifier();
    setState(338);
    match(CMParser::SColon);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionDeclarationContext ------------------------------------------------------------------

CMParser::FunctionDeclarationContext::FunctionDeclarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CMParser::TypeIdentifierContext* CMParser::FunctionDeclarationContext::typeIdentifier() {
  return getRuleContext<CMParser::TypeIdentifierContext>(0);
}

CMParser::IdentifierContext* CMParser::FunctionDeclarationContext::identifier() {
  return getRuleContext<CMParser::IdentifierContext>(0);
}

CMParser::IdentifierListContext* CMParser::FunctionDeclarationContext::identifierList() {
  return getRuleContext<CMParser::IdentifierListContext>(0);
}

tree::TerminalNode* CMParser::FunctionDeclarationContext::SColon() {
  return getToken(CMParser::SColon, 0);
}


size_t CMParser::FunctionDeclarationContext::getRuleIndex() const {
  return CMParser::RuleFunctionDeclaration;
}

void CMParser::FunctionDeclarationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunctionDeclaration(this);
}

void CMParser::FunctionDeclarationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunctionDeclaration(this);
}


std::any CMParser::FunctionDeclarationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitFunctionDeclaration(this);
  else
    return visitor->visitChildren(this);
}

CMParser::FunctionDeclarationContext* CMParser::functionDeclaration() {
  FunctionDeclarationContext *_localctx = _tracker.createInstance<FunctionDeclarationContext>(_ctx, getState());
  enterRule(_localctx, 42, CMParser::RuleFunctionDeclaration);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(340);
    typeIdentifier();
    setState(341);
    identifier();
    setState(342);
    match(CMParser::T__0);
    setState(343);
    identifierList();
    setState(344);
    match(CMParser::T__1);
    setState(345);
    match(CMParser::SColon);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionDefinitionContext ------------------------------------------------------------------

CMParser::FunctionDefinitionContext::FunctionDefinitionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CMParser::TypeIdentifierContext* CMParser::FunctionDefinitionContext::typeIdentifier() {
  return getRuleContext<CMParser::TypeIdentifierContext>(0);
}

CMParser::IdentifierContext* CMParser::FunctionDefinitionContext::identifier() {
  return getRuleContext<CMParser::IdentifierContext>(0);
}

CMParser::IdentifierListContext* CMParser::FunctionDefinitionContext::identifierList() {
  return getRuleContext<CMParser::IdentifierListContext>(0);
}

tree::TerminalNode* CMParser::FunctionDefinitionContext::OBrace() {
  return getToken(CMParser::OBrace, 0);
}

tree::TerminalNode* CMParser::FunctionDefinitionContext::CBrace() {
  return getToken(CMParser::CBrace, 0);
}

std::vector<CMParser::StatementContext *> CMParser::FunctionDefinitionContext::statement() {
  return getRuleContexts<CMParser::StatementContext>();
}

CMParser::StatementContext* CMParser::FunctionDefinitionContext::statement(size_t i) {
  return getRuleContext<CMParser::StatementContext>(i);
}


size_t CMParser::FunctionDefinitionContext::getRuleIndex() const {
  return CMParser::RuleFunctionDefinition;
}

void CMParser::FunctionDefinitionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunctionDefinition(this);
}

void CMParser::FunctionDefinitionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunctionDefinition(this);
}


std::any CMParser::FunctionDefinitionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitFunctionDefinition(this);
  else
    return visitor->visitChildren(this);
}

CMParser::FunctionDefinitionContext* CMParser::functionDefinition() {
  FunctionDefinitionContext *_localctx = _tracker.createInstance<FunctionDefinitionContext>(_ctx, getState());
  enterRule(_localctx, 44, CMParser::RuleFunctionDefinition);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(347);
    typeIdentifier();
    setState(348);
    identifier();
    setState(349);
    match(CMParser::T__0);
    setState(350);
    identifierList();
    setState(351);
    match(CMParser::T__1);
    setState(352);
    match(CMParser::OBrace);
    setState(356);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & -1720361880688124030) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & 63) != 0)) {
      setState(353);
      statement();
      setState(358);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(359);
    match(CMParser::CBrace);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionCallContext ------------------------------------------------------------------

CMParser::FunctionCallContext::FunctionCallContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CMParser::IdentifierContext* CMParser::FunctionCallContext::identifier() {
  return getRuleContext<CMParser::IdentifierContext>(0);
}

CMParser::ExpressionListContext* CMParser::FunctionCallContext::expressionList() {
  return getRuleContext<CMParser::ExpressionListContext>(0);
}


size_t CMParser::FunctionCallContext::getRuleIndex() const {
  return CMParser::RuleFunctionCall;
}

void CMParser::FunctionCallContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunctionCall(this);
}

void CMParser::FunctionCallContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunctionCall(this);
}


std::any CMParser::FunctionCallContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitFunctionCall(this);
  else
    return visitor->visitChildren(this);
}

CMParser::FunctionCallContext* CMParser::functionCall() {
  FunctionCallContext *_localctx = _tracker.createInstance<FunctionCallContext>(_ctx, getState());
  enterRule(_localctx, 46, CMParser::RuleFunctionCall);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(361);
    identifier();
    setState(362);
    match(CMParser::T__0);
    setState(363);
    expressionList();
    setState(364);
    match(CMParser::T__1);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExpressionListContext ------------------------------------------------------------------

CMParser::ExpressionListContext::ExpressionListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<CMParser::ExpressionContext *> CMParser::ExpressionListContext::expression() {
  return getRuleContexts<CMParser::ExpressionContext>();
}

CMParser::ExpressionContext* CMParser::ExpressionListContext::expression(size_t i) {
  return getRuleContext<CMParser::ExpressionContext>(i);
}


size_t CMParser::ExpressionListContext::getRuleIndex() const {
  return CMParser::RuleExpressionList;
}

void CMParser::ExpressionListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExpressionList(this);
}

void CMParser::ExpressionListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExpressionList(this);
}


std::any CMParser::ExpressionListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitExpressionList(this);
  else
    return visitor->visitChildren(this);
}

CMParser::ExpressionListContext* CMParser::expressionList() {
  ExpressionListContext *_localctx = _tracker.createInstance<ExpressionListContext>(_ctx, getState());
  enterRule(_localctx, 48, CMParser::RuleExpressionList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(374);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 2314859004561465218) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & 63) != 0)) {
      setState(366);
      expression();
      setState(371);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == CMParser::T__32) {
        setState(367);
        match(CMParser::T__32);
        setState(368);
        expression();
        setState(373);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IdentifierListContext ------------------------------------------------------------------

CMParser::IdentifierListContext::IdentifierListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<CMParser::FunctionParameterContext *> CMParser::IdentifierListContext::functionParameter() {
  return getRuleContexts<CMParser::FunctionParameterContext>();
}

CMParser::FunctionParameterContext* CMParser::IdentifierListContext::functionParameter(size_t i) {
  return getRuleContext<CMParser::FunctionParameterContext>(i);
}


size_t CMParser::IdentifierListContext::getRuleIndex() const {
  return CMParser::RuleIdentifierList;
}

void CMParser::IdentifierListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIdentifierList(this);
}

void CMParser::IdentifierListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIdentifierList(this);
}


std::any CMParser::IdentifierListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitIdentifierList(this);
  else
    return visitor->visitChildren(this);
}

CMParser::IdentifierListContext* CMParser::identifierList() {
  IdentifierListContext *_localctx = _tracker.createInstance<IdentifierListContext>(_ctx, getState());
  enterRule(_localctx, 50, CMParser::RuleIdentifierList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(384);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & -4611681637560745984) != 0)) {
      setState(376);
      functionParameter();
      setState(381);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == CMParser::T__32) {
        setState(377);
        match(CMParser::T__32);
        setState(378);
        functionParameter();
        setState(383);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionParameterContext ------------------------------------------------------------------

CMParser::FunctionParameterContext::FunctionParameterContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CMParser::TypeIdentifierContext* CMParser::FunctionParameterContext::typeIdentifier() {
  return getRuleContext<CMParser::TypeIdentifierContext>(0);
}

CMParser::IdentifierContext* CMParser::FunctionParameterContext::identifier() {
  return getRuleContext<CMParser::IdentifierContext>(0);
}


size_t CMParser::FunctionParameterContext::getRuleIndex() const {
  return CMParser::RuleFunctionParameter;
}

void CMParser::FunctionParameterContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunctionParameter(this);
}

void CMParser::FunctionParameterContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunctionParameter(this);
}


std::any CMParser::FunctionParameterContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitFunctionParameter(this);
  else
    return visitor->visitChildren(this);
}

CMParser::FunctionParameterContext* CMParser::functionParameter() {
  FunctionParameterContext *_localctx = _tracker.createInstance<FunctionParameterContext>(_ctx, getState());
  enterRule(_localctx, 52, CMParser::RuleFunctionParameter);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(386);
    typeIdentifier();
    setState(388);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CMParser::Identifier) {
      setState(387);
      identifier();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IdentifierContext ------------------------------------------------------------------

CMParser::IdentifierContext::IdentifierContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CMParser::IdentifierContext::Identifier() {
  return getToken(CMParser::Identifier, 0);
}


size_t CMParser::IdentifierContext::getRuleIndex() const {
  return CMParser::RuleIdentifier;
}

void CMParser::IdentifierContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIdentifier(this);
}

void CMParser::IdentifierContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIdentifier(this);
}


std::any CMParser::IdentifierContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitIdentifier(this);
  else
    return visitor->visitChildren(this);
}

CMParser::IdentifierContext* CMParser::identifier() {
  IdentifierContext *_localctx = _tracker.createInstance<IdentifierContext>(_ctx, getState());
  enterRule(_localctx, 54, CMParser::RuleIdentifier);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(390);
    match(CMParser::Identifier);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeIdentifierContext ------------------------------------------------------------------

CMParser::TypeIdentifierContext::TypeIdentifierContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CMParser::TypeNameContext* CMParser::TypeIdentifierContext::typeName() {
  return getRuleContext<CMParser::TypeNameContext>(0);
}

tree::TerminalNode* CMParser::TypeIdentifierContext::Const() {
  return getToken(CMParser::Const, 0);
}

std::vector<tree::TerminalNode *> CMParser::TypeIdentifierContext::Asterisk() {
  return getTokens(CMParser::Asterisk);
}

tree::TerminalNode* CMParser::TypeIdentifierContext::Asterisk(size_t i) {
  return getToken(CMParser::Asterisk, i);
}


size_t CMParser::TypeIdentifierContext::getRuleIndex() const {
  return CMParser::RuleTypeIdentifier;
}

void CMParser::TypeIdentifierContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTypeIdentifier(this);
}

void CMParser::TypeIdentifierContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTypeIdentifier(this);
}


std::any CMParser::TypeIdentifierContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitTypeIdentifier(this);
  else
    return visitor->visitChildren(this);
}

CMParser::TypeIdentifierContext* CMParser::typeIdentifier() {
  TypeIdentifierContext *_localctx = _tracker.createInstance<TypeIdentifierContext>(_ctx, getState());
  enterRule(_localctx, 56, CMParser::RuleTypeIdentifier);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(393);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CMParser::Const) {
      setState(392);
      match(CMParser::Const);
    }
    setState(395);
    typeName();
    setState(399);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 36, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(396);
        match(CMParser::Asterisk); 
      }
      setState(401);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 36, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeNameContext ------------------------------------------------------------------

CMParser::TypeNameContext::TypeNameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CMParser::TypeNameContext::Struct() {
  return getToken(CMParser::Struct, 0);
}

CMParser::IdentifierContext* CMParser::TypeNameContext::identifier() {
  return getRuleContext<CMParser::IdentifierContext>(0);
}


size_t CMParser::TypeNameContext::getRuleIndex() const {
  return CMParser::RuleTypeName;
}

void CMParser::TypeNameContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTypeName(this);
}

void CMParser::TypeNameContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTypeName(this);
}


std::any CMParser::TypeNameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitTypeName(this);
  else
    return visitor->visitChildren(this);
}

CMParser::TypeNameContext* CMParser::typeName() {
  TypeNameContext *_localctx = _tracker.createInstance<TypeNameContext>(_ctx, getState());
  enterRule(_localctx, 58, CMParser::RuleTypeName);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(412);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CMParser::T__33: {
        enterOuterAlt(_localctx, 1);
        setState(402);
        match(CMParser::T__33);
        break;
      }

      case CMParser::T__34: {
        enterOuterAlt(_localctx, 2);
        setState(403);
        match(CMParser::T__34);
        break;
      }

      case CMParser::T__35: {
        enterOuterAlt(_localctx, 3);
        setState(404);
        match(CMParser::T__35);
        break;
      }

      case CMParser::T__36: {
        enterOuterAlt(_localctx, 4);
        setState(405);
        match(CMParser::T__36);
        break;
      }

      case CMParser::T__37: {
        enterOuterAlt(_localctx, 5);
        setState(406);
        match(CMParser::T__37);
        break;
      }

      case CMParser::T__38: {
        enterOuterAlt(_localctx, 6);
        setState(407);
        match(CMParser::T__38);
        break;
      }

      case CMParser::T__39: {
        enterOuterAlt(_localctx, 7);
        setState(408);
        match(CMParser::T__39);
        break;
      }

      case CMParser::T__40: {
        enterOuterAlt(_localctx, 8);
        setState(409);
        match(CMParser::T__40);
        break;
      }

      case CMParser::Struct: {
        enterOuterAlt(_localctx, 9);
        setState(410);
        match(CMParser::Struct);
        setState(411);
        identifier();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SizeofTermContext ------------------------------------------------------------------

CMParser::SizeofTermContext::SizeofTermContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CMParser::SizeofTermContext* CMParser::SizeofTermContext::sizeofTerm() {
  return getRuleContext<CMParser::SizeofTermContext>(0);
}

CMParser::TypeIdentifierContext* CMParser::SizeofTermContext::typeIdentifier() {
  return getRuleContext<CMParser::TypeIdentifierContext>(0);
}

CMParser::ExprAtomContext* CMParser::SizeofTermContext::exprAtom() {
  return getRuleContext<CMParser::ExprAtomContext>(0);
}

CMParser::ExprContext* CMParser::SizeofTermContext::expr() {
  return getRuleContext<CMParser::ExprContext>(0);
}


size_t CMParser::SizeofTermContext::getRuleIndex() const {
  return CMParser::RuleSizeofTerm;
}

void CMParser::SizeofTermContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSizeofTerm(this);
}

void CMParser::SizeofTermContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSizeofTerm(this);
}


std::any CMParser::SizeofTermContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitSizeofTerm(this);
  else
    return visitor->visitChildren(this);
}

CMParser::SizeofTermContext* CMParser::sizeofTerm() {
  SizeofTermContext *_localctx = _tracker.createInstance<SizeofTermContext>(_ctx, getState());
  enterRule(_localctx, 60, CMParser::RuleSizeofTerm);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(421);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 38, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(414);
      match(CMParser::T__0);
      setState(415);
      sizeofTerm();
      setState(416);
      match(CMParser::T__1);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(418);
      typeIdentifier();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(419);
      exprAtom();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(420);
      expr(0);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StringLiteralContext ------------------------------------------------------------------

CMParser::StringLiteralContext::StringLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CMParser::StringLiteralContext::String() {
  return getToken(CMParser::String, 0);
}


size_t CMParser::StringLiteralContext::getRuleIndex() const {
  return CMParser::RuleStringLiteral;
}

void CMParser::StringLiteralContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStringLiteral(this);
}

void CMParser::StringLiteralContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStringLiteral(this);
}


std::any CMParser::StringLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitStringLiteral(this);
  else
    return visitor->visitChildren(this);
}

CMParser::StringLiteralContext* CMParser::stringLiteral() {
  StringLiteralContext *_localctx = _tracker.createInstance<StringLiteralContext>(_ctx, getState());
  enterRule(_localctx, 62, CMParser::RuleStringLiteral);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(423);
    match(CMParser::String);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SystemIncludeContext ------------------------------------------------------------------

CMParser::SystemIncludeContext::SystemIncludeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CMParser::SysModuleNameContext* CMParser::SystemIncludeContext::sysModuleName() {
  return getRuleContext<CMParser::SysModuleNameContext>(0);
}


size_t CMParser::SystemIncludeContext::getRuleIndex() const {
  return CMParser::RuleSystemInclude;
}

void CMParser::SystemIncludeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSystemInclude(this);
}

void CMParser::SystemIncludeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSystemInclude(this);
}


std::any CMParser::SystemIncludeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitSystemInclude(this);
  else
    return visitor->visitChildren(this);
}

CMParser::SystemIncludeContext* CMParser::systemInclude() {
  SystemIncludeContext *_localctx = _tracker.createInstance<SystemIncludeContext>(_ctx, getState());
  enterRule(_localctx, 64, CMParser::RuleSystemInclude);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(425);
    match(CMParser::T__41);
    setState(426);
    sysModuleName();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SysModuleNameContext ------------------------------------------------------------------

CMParser::SysModuleNameContext::SysModuleNameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CMParser::SysModuleNameContext::SysModuleName() {
  return getToken(CMParser::SysModuleName, 0);
}


size_t CMParser::SysModuleNameContext::getRuleIndex() const {
  return CMParser::RuleSysModuleName;
}

void CMParser::SysModuleNameContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSysModuleName(this);
}

void CMParser::SysModuleNameContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<CMListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSysModuleName(this);
}


std::any CMParser::SysModuleNameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CMVisitor*>(visitor))
    return parserVisitor->visitSysModuleName(this);
  else
    return visitor->visitChildren(this);
}

CMParser::SysModuleNameContext* CMParser::sysModuleName() {
  SysModuleNameContext *_localctx = _tracker.createInstance<SysModuleNameContext>(_ctx, getState());
  enterRule(_localctx, 66, CMParser::RuleSysModuleName);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(428);
    match(CMParser::SysModuleName);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

bool CMParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 4: return exprSempred(antlrcpp::downCast<ExprContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool CMParser::exprSempred(ExprContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 8);
    case 1: return precpred(_ctx, 7);
    case 2: return precpred(_ctx, 6);
    case 3: return precpred(_ctx, 5);
    case 4: return precpred(_ctx, 4);
    case 5: return precpred(_ctx, 3);
    case 6: return precpred(_ctx, 2);
    case 7: return precpred(_ctx, 21);
    case 8: return precpred(_ctx, 20);
    case 9: return precpred(_ctx, 19);
    case 10: return precpred(_ctx, 18);

  default:
    break;
  }
  return true;
}

void CMParser::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  cmParserInitialize();
#else
  ::antlr4::internal::call_once(cmParserOnceFlag, cmParserInitialize);
#endif
}
