//! \file xsdata.h
//! A collection of classes for containing the Multi-Group Cross Section data

#ifndef XSDATA_H
#define XSDATA_H

#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <vector>

#include "constants.h"
#include "hdf5_interface.h"
#include "math_functions.h"
#include "random_lcg.h"
#include "scattdata.h"


namespace openmc {

//==============================================================================
// XSDATA contains the temperature-independent cross section data for an MGXS
//==============================================================================

class XsData {
  private:
    void _scatter_from_hdf5(const hid_t xsdata_grp, const int n_pol,
         const int n_azi, const int energy_groups, int scatter_format,
         const int final_scatter_format, const int order_data,
         const int max_order, const int legendre_to_tabular_points);
    void _fissionable_from_hdf5(const hid_t xsdata_grp, const int n_pol,
         const int n_azi, const int energy_groups, const int delayed_groups,
         const bool is_isotropic);
  public:
    // The following quantities have the following dimensions:
    // [angle][incoming group]
    double_2dvec total;
    double_2dvec absorption;
    double_2dvec nu_fission;
    double_2dvec prompt_nu_fission;
    double_2dvec kappa_fission;
    double_2dvec fission;
    double_2dvec inverse_velocity;

    // decay_rate has the following dimensions:
    // [angle][delayed group]
    double_2dvec decay_rate;
    // delayed_nu_fission has the following dimensions:
    // [angle][incoming group][delayed group]
    double_3dvec delayed_nu_fission;
    // chi_prompt has the following dimensions:
    // [angle][incoming group][outgoing group]
    double_3dvec chi_prompt;
    // chi_delayed has the following dimensions:
    // [angle][incoming group][outgoing group][delayed group]
    double_4dvec chi_delayed;
    // scatter has the following dimensions: [angle]
    std::vector<ScattData*> scatter;

    XsData() = default;
    XsData(const int num_groups, const int num_delayed_groups,
           const bool fissionable, const int scatter_format, const int n_pol,
           const int n_azi);
    void from_hdf5(const hid_t xsdata_grp, const bool fissionable,
                   const int scatter_format, const int final_scatter_format,
                   const int order_data, const int max_order,
                   const int legendre_to_tabular_points,
                   const bool is_isotropic, const int n_pol, const int n_azi);
    void combine(const std::vector<XsData*>& those_xs,
                 const double_1dvec& scalars);
    bool equiv(const XsData& that);
};


} //namespace openmc
#endif // XSDATA_H