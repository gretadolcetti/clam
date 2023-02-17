#include <clam/config.h>
#include <clam/CrabDomain.hh>
#include <clam/RegisterAnalysis.hh>
#include <crab/config.h>
#include "dec_int_pkset_pplite.hh"

namespace clam {
#ifdef INCLUDE_ALL_DOMAINS
#ifdef HAVE_PPLITE
REGISTER_DOMAIN(clam::CrabDomain::DECOUPLED_INT_PKSET_PPLITE, dec_int_pkset_pplite_domain)
#else
UNREGISTER_DOMAIN(dec_int_pkset_pplite_domain)
#endif
#else
UNREGISTER_DOMAIN(dec_int_pkset_pplite_domain)
#endif
} // end namespace clam

