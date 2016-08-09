/*! 
 * \file  mfront/include/MFront/SecondBroydenSolver.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 22 août 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_MFRONTSECONDBROYDENSOLVER_H_
#define LIB_MFRONT_MFRONTSECONDBROYDENSOLVER_H_ 

#include "MFront/NonLinearSystemSolverBase.hxx"

namespace mfront{

  /*!
   * Non linear system solver based on the
   * second Broyden algorithm.
   */
  struct SecondBroydenSolver
    : public NonLinearSystemSolverBase
  {
    /*!
     * \return the reserved names
     */
    virtual std::vector<std::string>
    getReservedNames(void) const override;
    /*!
     * \return true if the solver uses the jacobian of the system
     * (Newton-Raphson solvers) or an approximation of it (Broyden
     * method).
     */
    virtual bool
    usesJacobian(void) const override;
    /*!
     * \return true if the solver uses the invert of the jacobian of
     * the system or an approximation of it (second Broyden method).
     */
    virtual bool
    usesJacobianInvert(void) const override;
    /*!
     * \return true if the solver requires a numerical jacobian
     */
    virtual bool
    requiresNumericalJacobian(void) const override;
    /*!
     * \return true if the solver allows the user to initialize the
     * jacobian
     */
    virtual bool
    allowsJacobianInitialisation(void) const override;
    /*!
     * \return true if the solver allows the user to initialize the
     * invert of the jacobian.
     * \note If the solver does not provide such initialisation, the
     * invert of the jacobian could for example be initialised to
     * identity in the behaviour constructor.
     */
    virtual bool
    allowsJacobianInvertInitialisation(void) const override;
    /*!
     * \return true if the solver requires the jacobian to be
     * initialized to the identity matrix at the beginning of the
     * computeFdF method.
     */
    virtual bool
    requiresJacobianToBeReinitialisedToIdentityAtEachIterations(void) const override;
    /*!
     * \brief write the algorithm specific members
     * \param[in,out] md  : mechanical description
     * \param[in] key     : keyword
     * \param[in] p       : current position in file (after keyword)
     * \param[in] pe      : end of file
     */
    virtual std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
    treatSpecificKeywords(BehaviourDescription&,
			  const std::string&,
			  const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator,
			  const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator) override;
    /*!
     * \brief method called at the end of the input file processing.
     * Solver can use it to declared additional parameters
     * \param[in,out] md  : mechanical description
     */
    virtual void
    endsInputFileProcessing(BehaviourDescription&) const override;
    /*!
     * \brief write algorithm specific initialisations
     * \param[in] out : output file
     * \param[in] md  : mechanical description
     * \param[in] h   : modelling hypotheis
     */
    virtual void
    writeSpecificInitializeMethodPart(std::ostream&,
				      const BehaviourDescription&,
				      const tfel::material::ModellingHypothesis::Hypothesis) const override;
    /*!
     * \brief write the algorithm specific members
     * \param[in] out : output file
     * \param[in] md  : mechanical description
     * \param[in] h   : modelling hypotheis
     */
    virtual void
    writeSpecificMembers(std::ostream&,
			 const BehaviourDescription&,
			 const tfel::material::ModellingHypothesis::Hypothesis) const override;
    /*!
     * \brief write the resolution algorithm
     * \param[in] out : output file
     * \param[in] md  : mechanical description
     * \param[in] h   : modelling hypotheis
     */
    virtual void
    writeResolutionAlgorithm(std::ostream&,
			     const BehaviourDescription&,
			     const tfel::material::ModellingHypothesis::Hypothesis) const override;
    /*!
     * destructor
     */
    virtual ~SecondBroydenSolver();
  }; // end of struct SecondBroydenSolver

} // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTSECONDBROYDENSOLVER_H_ */
