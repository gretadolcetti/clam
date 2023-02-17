#pragma once

#include <crab/domains/apron_domains.hpp>
#include <crab/domains/decoupled_domains.hpp>
#include "crab_defs.hh"

namespace clam {

using BASE(dec_int_pkset_pplite_domain_t) =
  crab::domains::decoupled_domain<
    crab::domains::apron_asc_dsc_pair<
      // Ascending domain: Apron's intervals
      crab::domains::apron_domain<
        number_t, region_subdom_varname_t,
        crab::domains::apron_domain_id_t::APRON_INT
        >,
      // Descending domain: PPLite's sets of polyhedra
      crab::domains::apron_domain<
        number_t, region_subdom_varname_t,
        crab::domains::apron_domain_id_t::APRON_PPLITE_FPSET
        >
      >>;
using dec_int_pkset_pplite_domain_t =
  RGN_FUN(ARRAY_FUN(BOOL_NUM(BASE(dec_int_pkset_pplite_domain_t))));
} // end namespace clam
