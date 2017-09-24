#include "llvm/Pass.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/DebugLoc.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/Transforms/Instrumentation.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

#define DEBUG_TYPE "instrument-instruction-accesses"

STATISTIC(Instrumented, "Loads instrumented");


namespace {
    class InstStatistics : public FunctionPass,
    public InstVisitor<InstStatistics> {
        const DataLayout *TD;
        IRBuilder<> *Builder;

        PointerType *VoidPtrTy;
        IntegerType *SizeTy;

        Function *VisitInstFunction;
        Function *StatInstFunction;

        public:
        static char ID;
        InstStatistics(): FunctionPass(ID) { }
        virtual bool doInitialization(Module &M);
        virtual bool runOnFunction(Function &F);

        virtual void getAnalysisUsage(AnalysisUsage &AU) const {
            AU.setPreservesCFG();
        }

        virtual StringRef getPassName() const {
            return "InstStatistics";
        }

        // Visitor methods
        void visitInstruction(Instruction &I);
    };
} /* namespace */


bool InstStatistics::doInitialization(Module &M) {
    Type *VoidTy = Type::getVoidTy(M.getContext());
    VoidPtrTy = Type::getInt8PtrTy(M.getContext());
    SizeTy = IntegerType::getInt64Ty(M.getContext());
    std::vector<Type*> Params = {SizeTy};
    M.getOrInsertFunction("__visit", FunctionType::get(VoidTy, Params, false));
    M.getOrInsertFunction("__statistics", FunctionType::get(VoidTy, false));

    errs() << "doInitialization done \n";
    return true;
}

bool InstStatistics::runOnFunction(Function &F) {

    VisitInstFunction = F.getParent()->getFunction("__visit");
    assert(VisitInstFunction && "__visit function has disappeared!\n");

    StatInstFunction = F.getParent()->getFunction("__statistics");
    assert(StatInstFunction && "__statistics function has disappeared!\n");

    TD = &F.getParent()->getDataLayout();
    IRBuilder<> TheBuilder(F.getContext());
    Builder = &TheBuilder;

    for (auto &BB : F) {
        visit(BB);
        errs() << "Insert statistics for " << F.getName() << "\n";
        Builder->SetInsertPoint(&BB.back());
        CallInst *CI = Builder->CreateCall(StatInstFunction);
    }


    // Modified
    return true;
}

void InstStatistics::visitInstruction(Instruction &I) {
    auto Node = I.getPrevNode();
    if (Node == nullptr) {
        errs() << "Insert after " << I.getOpcode() << "\n";;
        Node = I.getNextNode();
    } else {
        errs() << "Insert before " << I.getOpcode() << "\n";
    }
    if (Node == nullptr) {
        errs() << "Don't know where to insert " << I << "\n";
        return;
    }
    Builder->SetInsertPoint(Node);
    Value* Args[] = {ConstantInt::get(SizeTy, I.getOpcode())};
    CallInst *CI = Builder->CreateCall(VisitInstFunction, Args);

    // Copy debug information if it is present.
    //if (MDNode *MD = I.getMetadata("dbg")) {
    //    CI->setMetadata("dbg", MD);
    //}
    ++Instrumented;
}
namespace llvm {
    FunctionPass *createInstStatisticsPass();
    void initializeInstStatisticsPass(llvm::PassRegistry&);
}

FunctionPass *createInstStatisticsPass() {
    return new InstStatistics();
}

char InstStatistics::ID = 0;
INITIALIZE_PASS(InstStatistics, "instrument-instruction-accesses",
        "Instrument all instruction accesses", false, false)

// Automatically enable the pass.
// http://adriansampson.net/blog/clangpass.html
static void registerPass(const PassManagerBuilder &,
                         legacy::PassManagerBase &PM) {
  PM.add(new InstStatistics());
}
static RegisterStandardPasses
  RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                 registerPass);
