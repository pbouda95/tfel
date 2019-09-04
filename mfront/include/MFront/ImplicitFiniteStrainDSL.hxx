/*!
 * \file  mfront/include/MFront/ImplicitFiniteStrainDSL.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 18 févr. 2013
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_IMPLICITFINITESTRAINPARSER_HXX
#define LIB_MFRONT_IMPLICITFINITESTRAINPARSER_HXX

#include "MFront/ImplicitDSLBase.hxx"

namespace mfront {

  /*!
   * \brief a DSL handling finite strain behaviours integrated using an implicit
   * scheme.
   */
  struct ImplicitFiniteStrainDSL : public ImplicitDSLBase {
    //! \brief return the name of the DSL
    static std::string getName();
    //! \brief return a short description of the DSL
    static std::string getDescription();
    //! \return a description of the DSL
    BehaviourDSLDescription getBehaviourDSLDescription() const override;
    //! \brief  constructor
    ImplicitFiniteStrainDSL();
    //! \brief  destructor
    ~ImplicitFiniteStrainDSL();
  };  // end of struct ImplicitFiniteStrainDSL

}  // end of namespace mfront

#endif /* LIB_MFRONT_IMPLICITFINITESTRAINPARSER_HXX */
