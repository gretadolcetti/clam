#pragma once

#include "clam/crab/crab_lang.hh"

namespace clam {

enum class CrabBuilderPrecision {
  NUM,           // only integers and boolean
  SINGLETON_MEM, // NUM + singleton memory objects
  MEM            // NUM + all memory objects
};

/** User-definable parameters to build a Crab CFG **/
struct CrabBuilderParams {
  // Level of abstraction of the CFG
  CrabBuilderPrecision precision_level;
  // Perform dead code elimination, cfg simplifications, etc
  bool simplify;
  // translate precisely calls
  bool interprocedural;
  // Lower singleton aliases (e.g., globals) to scalar ones
  bool lower_singleton_aliases;
  // Remove useless havoc operations
  bool include_useless_havoc;
  // Translate bignums (> 64), otherwise operations with big numbers
  // are havoced.
  bool enable_bignums;
  /// Add reasonable assumptions about pointers (e.g., allocas and
  /// globals cannot be null, external functions do not return
  /// dangling pointers, etc.)
  bool add_pointer_assumptions;
  //// --- printing options
  // print the cfg after it has been built
  bool print_cfg;
  // print the cfg to dot format after it has been built
  bool dot_cfg;

  /*
   * Constructor for internal use. Clam clients should not use this
   * constructor since the number of parameters can change over
   * time.
   */
  CrabBuilderParams(CrabBuilderPrecision _precision_level, bool _simplify,
                    bool _interprocedural, bool _lower_singleton_aliases,
                    bool _include_useless_havoc, bool _enable_bignums,
                    bool _add_pointer_assumptions, bool _print_cfg,
                    bool _dot_cfg)
      : precision_level(_precision_level), simplify(_simplify),
        interprocedural(_interprocedural),
        lower_singleton_aliases(_lower_singleton_aliases),
        include_useless_havoc(_include_useless_havoc),
        enable_bignums(_enable_bignums),
        add_pointer_assumptions(_add_pointer_assumptions),
        print_cfg(_print_cfg), dot_cfg(_dot_cfg) {}

  CrabBuilderParams()
      : precision_level(CrabBuilderPrecision::NUM), simplify(false),
        interprocedural(true), lower_singleton_aliases(false),
        include_useless_havoc(true), enable_bignums(false),
        add_pointer_assumptions(true), print_cfg(false), dot_cfg(false) {}

  bool trackMemory() const {
    return precision_level == CrabBuilderPrecision::MEM;
  }

  bool trackOnlySingletonMemory() const {
    return precision_level == CrabBuilderPrecision::SINGLETON_MEM;
  }

  bool addPointerAssumptions() const {
    return trackMemory() && add_pointer_assumptions;
  }

  /* Set the level of abstraction for Crab programs */
  void setPrecision(CrabBuilderPrecision val) { precision_level = val; }

  void write(llvm::raw_ostream &o) const;
};

} // end namespace clam
