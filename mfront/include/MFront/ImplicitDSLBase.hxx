/*!
 * \file   mfront/include/MFront/ImplicitDSLBase.hxx
 * \brief  This file declares the ImplicitDSLBase class
 * \author Thomas Helfer
 * \date   10 Nov 2006
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONTIMPLICITPARSERBASE_HXX
#define LIB_MFRONTIMPLICITPARSERBASE_HXX 

#include<string>
#include<memory>
#include"MFront/MFrontConfig.hxx"
#include"MFront/BehaviourDSLBase.hxx"

namespace mfront{

  // forward declaratin
  struct NonLinearSystemSolver;

  //! \brief Base class for all parser based on an implicit scheme
  struct MFRONT_VISIBILITY_EXPORT ImplicitDSLBase
      : public BehaviourDSLBase<ImplicitDSLBase> {
    //! \brief constructor
    ImplicitDSLBase();
    /*!
     * \return the solver used to integrate the mechanical behaviour
     * \throw this methods throws is the solver is not defined yet.
     * \note the solver is only defined after the
     * endsInputFileProcessing is called. This defined at the
     * beginning of this method, so it is safe to use this method in
     * the rest of the endsInputFileProcessing method, in the
     * endTreatment method of the behaviour bricks in particular.
     */
    const NonLinearSystemSolver& getSolver() const;

    std::string getCodeBlockTemplate(const std::string&,
                                     const bool) const override;

    void getSymbols(std::map<std::string, std::string>&,
                    const Hypothesis,
                    const std::string&) override;
    //!\brief destructor
    ~ImplicitDSLBase() override;

  protected:
    
    //! a simple alias
    using Solver = std::shared_ptr<NonLinearSystemSolver>;

    virtual void predictorAnalyser(const Hypothesis, const std::string&);

    virtual void integratorAnalyser(const Hypothesis, const std::string&);

    std::string tangentOperatorVariableModifier(const Hypothesis,
                                                const std::string&,
                                                const bool) override;

    virtual std::string integratorVariableModifier(const Hypothesis,
                                                   const std::string&,
                                                   const bool);

    virtual std::string computeStressVariableModifier1(const Hypothesis,
                                                       const std::string&,
                                                       const bool);

    virtual std::string computeStressVariableModifier2(const Hypothesis,
                                                       const std::string&,
                                                       const bool);
    /*!
     * treat an unknown keyword. This method is overriden so the
     * solver may have specific keywords
     */
    void treatUnknownKeyword() override;
    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] n : variable name
     */
    void treatUnknownVariableMethod(const Hypothesis,
				    const std::string&) override;
    //! \brief treat the `@StateVariable` keyword
    void treatStateVariable() override;
    //! \brief treat the `@IntegrationVariable` keyword
    void treatIntegrationVariable();
    //! \brief treat the `@Integrator` keyword
    void  treatIntegrator() override;
    //! \brief treat the `@ComputeFinalStress` keyword
    virtual void treatComputeFinalStress();

    void completeVariableDeclaration() override;

    void endsInputFileProcessing() override;

    void writeBehaviourLocalVariablesInitialisation(std::ostream&,
						    const Hypothesis) const override;
    
    void writeBehaviourIntegrator(std::ostream&,
				  const Hypothesis) const override;

    virtual void writeComputeFdF(std::ostream&,
				 const Hypothesis) const;

    void writeBehaviourParserSpecificIncludes(std::ostream&) const override;

    void writeBehaviourParserSpecificTypedefs(std::ostream&) const override;

    void writeBehaviourParserSpecificMembers(std::ostream&,
					     const Hypothesis) const override;

    void writeBehaviourIntegrationVariablesIncrements(std::ostream&,
						      const Hypothesis) const override;

    std::string getLocalVariablesInitializers(const Hypothesis) const override;

    std::string getBehaviourConstructorsInitializers(const Hypothesis) const override;

    std::string getIntegrationVariablesIncrementsInitializers(const Hypothesis) const override;

    void writeBehaviourParserSpecificInitializeMethodPart(std::ostream&,
							  const Hypothesis) const override;

    virtual void writeComputeNumericalJacobian(std::ostream&,
					       const Hypothesis) const;

    virtual void writeGetPartialJacobianInvert(std::ostream&,
					       const Hypothesis) const;
    //! \brief treat the `@Theta` keyword
    virtual void treatTheta();
    //! \brief treat the `@IterMax` keyword
    virtual void treatIterMax();
    //! \brief treat the `@Epsilon` keyword
    virtual void treatEpsilon();
    //! \brief treat the `@AdditionalConvergenceChecks` keyword
    virtual void treatAdditionalConvergenceChecks();
    //! \brief treat the `@PerturbationValueForNumericalJacobianComputation` keyword
    virtual void treatPerturbationValueForNumericalJacobianComputation();
    //! \brief treat the `@Algorithm` keyword
    virtual void treatAlgorithm();
    //! \brief treat the `@Predictor` keyword
    virtual void treatPredictor();
    //! \brief treat the `@ComputeStress` keyword
    virtual void treatComputeStress();
    //! \brief treat the `@CompareToNumericalJacobian` keyword
    virtual void treatCompareToNumericalJacobian();
    //! \brief treat the `@JacobianComparisonCriterion` keyword
    virtual void treatJacobianComparisonCriterion();
    //! \brief treat the `@InitJacobian` keyword
    virtual void treatInitJacobian();
    //! \brief treat the `@InitJacobianInvert` keyword
    virtual void treatInitJacobianInvert();
    //! \brief treat the `@MaximumIncrementValuePerIteration` keyword
    virtual void treatMaximumIncrementValuePerIteration();
    //! \brief treat the `@NumericallyComputedJacobianBlocks` keyword
    virtual void treatNumericallyComputedJacobianBlocks();
    /*!
     * \return true if the the given variable may have methods
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    bool isCallableVariable(const Hypothesis,
                            const std::string&) const override;
    /*!
     * \return a type able to do the mapping of a variable with the
     * vector used by the root-finding algorithm.
     * \param[in] v : variable
     */
    virtual std::string getVectorMappingClass(const VariableDescription&) const;

    // let the BehaviourDSLBase access specific keywords
    friend struct BehaviourDSLBase<ImplicitDSLBase>;

    std::set<std::string> jacobianPartsUsedInIntegrator;

    std::set<std::string> integrationVariablesIncrementsUsedInPredictor;

    Solver solver;

  }; // end of struct ImplicitDSLBase

} // end of namespace mfront  

#endif /* LIB_MFRONTIMPLICITPARSERBASE_HXX */
