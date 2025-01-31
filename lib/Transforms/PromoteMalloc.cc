#include "llvm/IR/CallSite.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/Pass.h"

#include "llvm/ADT/iterator_range.h"
#include "llvm/Support/raw_ostream.h"

namespace clam {

using namespace llvm;

class PromoteMalloc : public FunctionPass {

public:
  static char ID;

  PromoteMalloc() : FunctionPass(ID) {}

  virtual bool runOnFunction(Function &F) override {
    if (F.empty())
      return false;

    // -- only promote mallocs in top level functions
    if (!F.getName().equals("main"))
      return false;

    bool changed = false;

    SmallVector<Instruction *, 16> kill;

    for (auto &I : llvm::make_range(inst_begin(F), inst_end(F))) {
      if (!isa<CallInst>(&I))
        continue;

      CallSite CS(&I);
      const Function *fn = CS.getCalledFunction();
      if (!fn && CS.getCalledValue())
        fn = dyn_cast<const Function>(CS.getCalledValue()->stripPointerCasts());

      if (fn && fn->getName().equals("malloc")) {
        if (PointerType *pty = dyn_cast<PointerType>(I.getType())) {
          unsigned addrSpace = 0;
          Value *nv = new AllocaInst(pty->getPointerElementType(), addrSpace,
                                     CS.getArgument(0), "malloc", &I);
          I.replaceAllUsesWith(nv);
          changed = true;
        }
      } else if (fn && fn->getName().equals("free"))
        kill.push_back(&I);
    }

    // -- remove all calls to free(). This is too much, but ensures
    // -- that all promoted mallocs() are not free'ed by mistake
    for (auto *I : kill)
      I->eraseFromParent();

    return changed;
  }

  virtual void getAnalysisUsage(AnalysisUsage &AU) const override {
    // AU.setPreservesAll ();
  }

  virtual StringRef getPassName() const {
    return "Clam: Promote malloc to alloca instructions";
  }
};

char PromoteMalloc::ID = 0;
} // namespace clam

namespace clam {
Pass *createPromoteMallocPass() { return new PromoteMalloc(); }
} // namespace clam

static llvm::RegisterPass<clam::PromoteMalloc>
    X("promote-malloc", "Promote top-level malloc calls to alloca");
