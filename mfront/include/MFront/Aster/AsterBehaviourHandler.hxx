/*! 
 * \file  mfront/include/MFront/Aster/AsterBehaviourHandler.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 30 janv. 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_ASTER_ASTERBEHAVIOURHANDLER_H_
#define LIB_MFRONT_ASTER_ASTERBEHAVIOURHANDLER_H_ 

#ifndef LIB_MFRONT_ASTER_CALL_H_
#error "This header shall not be called directly"
#endif /* LIB_MFRONT_ASTER_CALL_H_ */

#include"TFEL/Math/Matrix/TMatrixView.hxx"
#include"TFEL/Math/T2toST2/T2toST2View.hxx"
#include"TFEL/Math/ST2toST2/ST2toST2View.hxx"
#include"TFEL/Material/MechanicalBehaviour.hxx"

#include"MFront/Aster/AsterTangentOperator.hxx"
#include"MFront/Aster/AsterComputeStiffnessTensor.hxx"
#include"MFront/Aster/AsterComputeThermalExpansionCoefficientTensor.hxx"

namespace aster
{

  template<AsterBehaviourType btype>
  struct AsterTangentOperatorFlag;

  template<>
  struct AsterTangentOperatorFlag<aster::SMALLSTRAINSTANDARDBEHAVIOUR>
  {
    typedef tfel::material::MechanicalBehaviourBase MechanicalBehaviourBase; 
    typedef tfel::material::TangentOperatorTraits<MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR>
    TangentOperatorTraits;
    static constexpr TangentOperatorTraits::SMFlag value = TangentOperatorTraits::STANDARDTANGENTOPERATOR;
  };

  template<>
  struct AsterTangentOperatorFlag<aster::FINITESTRAINSTANDARDBEHAVIOUR>
  {
    typedef tfel::material::MechanicalBehaviourBase MechanicalBehaviourBase; 
    typedef tfel::material::TangentOperatorTraits<MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR>
    TangentOperatorTraits;
    static constexpr TangentOperatorTraits::SMFlag value = TangentOperatorTraits::DTAU_DDF;
  };

  template<>
  struct AsterTangentOperatorFlag<aster::COHESIVEZONEMODEL>
  {
    typedef tfel::material::MechanicalBehaviourBase MechanicalBehaviourBase; 
    typedef tfel::material::TangentOperatorTraits<MechanicalBehaviourBase::COHESIVEZONEMODEL>
    TangentOperatorTraits;
    static constexpr TangentOperatorTraits::SMFlag value = TangentOperatorTraits::STANDARDTANGENTOPERATOR;
  };

  template<AsterBehaviourType btype,
	   unsigned short N>
  struct AsterTangentOperatorType;

  template<unsigned short N>
  struct AsterTangentOperatorType<aster::SMALLSTRAINSTANDARDBEHAVIOUR,N>
  {
    using type      = tfel::math::st2tost2<N,AsterReal>;
    using view_type = tfel::math::ST2toST2View<N,AsterReal>;
  };

  template<unsigned short N>
  struct AsterTangentOperatorType<aster::FINITESTRAINSTANDARDBEHAVIOUR,N>
  {
    using type      = tfel::math::t2tost2<N,AsterReal>;
    using view_type = tfel::math::T2toST2View<N,AsterReal>;
  };

  template<unsigned short N>
  struct AsterTangentOperatorType<aster::COHESIVEZONEMODEL,N>
  {
    using type      = tfel::math::tmatrix<N,N,AsterReal>;
    using view_type = tfel::math::TMatrixView<N,N,AsterReal>;
  };

  template<tfel::material::ModellingHypothesis::Hypothesis H,
	   template<tfel::material::ModellingHypothesis::Hypothesis,typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL AsterBehaviourHandler
    : public AsterInterfaceExceptions
  {

    /*!
     * An helper structure used to initialise the driving variables
     */
    struct TFEL_VISIBILITY_LOCAL DrivingVariableInitialiserWithStressFreeExpansion
      : public AsterInterfaceExceptions
      {
      //! a simple alias
      typedef Behaviour<H,AsterReal,false> BV;
      /*!
       * \param[out] b      : behaviour
       * \param[in]  STRAN  : driving variable at the beginning of the
       *                      time step
       * \param[in]  DSTRAN : driving variable at the end of the
       *                      time step or driving variable increment
       * \param[in]  sfeh   : function handling the stress-free expansion
       *                      at the beginning of the time step
       */
      TFEL_ASTER_INLINE static 
      void exe(BV& b,
	       const AsterReal *const STRAN,
	       const AsterReal *const DSTRAN,
	       const StressFreeExpansionHandler& sfeh)
      {
	using std::pair;
	using tfel::fsalgo::copy;
	using namespace tfel::material;
	typedef typename BV::StressFreeExpansionType StressFreeExpansionType;
	typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
	const AsterInt N = ModellingHypothesisToSpaceDimension<H>::value;
	AsterReal dv0[AsterTraits<BV>::DrivingVariableSize];
	AsterReal dv1[AsterTraits<BV>::DrivingVariableSize];
	copy<AsterTraits<BV>::DrivingVariableSize>::exe(STRAN,dv0);
	copy<AsterTraits<BV>::DrivingVariableSize>::exe(DSTRAN,dv1);
	// check that the function pointer are not null
	if(sfeh==nullptr){
	  throwUnsupportedStressFreeExpansionException(Traits::getName());
	}
	pair<StressFreeExpansionType,StressFreeExpansionType> s;
	b.computeStressFreeExpansion(s);
	const auto& s0 = s.first;
	const auto& s1 = s.second;
	sfeh(dv0,dv1,&s0[0],&s1[0],N);
	b.setASTERBehaviourDataDrivingVariables(dv0);
	b.setASTERIntegrationDataDrivingVariables(dv1);
      } // end of exe

    }; // end of struct DrivingVariableInitialiserWithStressFreeExpansion

    /*!
     * An helper structure used to initialise the driving variables
     */
    struct TFEL_VISIBILITY_LOCAL DrivingVariableInitialiserWithoutStressFreeExpansion
    {
      //! a simple alias
      typedef Behaviour<H,AsterReal,false> BV;
      /*!
       * \param[out] b      : b
       * \param[in]  STRAN  : driving variable at the beginning of the
       *                     time step
       * \param[in]  DSTRAN : driving variable at the end of the
       *                      time step or driving variable increment
       * \param[in]  sfeh   : function handling the stress-free expansion
       *                      at the beginning of the time step
       */
      TFEL_ASTER_INLINE static 
      void exe(BV& b,
	       const AsterReal *const STRAN,
	       const AsterReal *const DSTRAN,
	       const StressFreeExpansionHandler&)
      {
	b.setASTERBehaviourDataDrivingVariables(STRAN);
	b.setASTERIntegrationDataDrivingVariables(DSTRAN);
      } // end of exe
    }; // end of struct DrivingVariableInitialiserWithoutStressFreeExpansion

    struct TFEL_VISIBILITY_LOCAL StiffnessOperatorInitializer
    {
      typedef Behaviour<H,AsterReal,false> BV;
      typedef typename BV::BehaviourData  BData;
      TFEL_ASTER_INLINE static void
      exe(BData& data,const AsterReal * const props){
	typedef AsterTraits<BV> Traits;
	const bool buas = Traits::requiresUnAlteredStiffnessTensor;
	AsterComputeStiffnessTensor<AsterTraits<BV>::btype,H,
				    AsterTraits<BV>::etype,buas>::exe(data.getStiffnessTensor(),
								      props);
      } // end of exe
    }; // end of struct StiffnessOperatorInitializer

    struct TFEL_VISIBILITY_LOCAL ThermalExpansionCoefficientTensorInitializer
    {
      typedef Behaviour<H,AsterReal,false> BV;
      typedef typename BV::BehaviourData  BData;
      TFEL_ASTER_INLINE static void
	exe(BData& data,const AsterReal * const props){
	const unsigned short o =
	  AsterTraits<BV>::elasticPropertiesOffset;
	AsterComputeThermalExpansionCoefficientTensor<AsterTraits<BV>::btype,H,
						      AsterTraits<BV>::stype>::exe(props+o,
										   data.getThermalExpansionCoefficientTensor());
      } // end of exe
    }; // end of struct ThermalExpansionCoefficientTensorInitializer

    struct TFEL_VISIBILITY_LOCAL DoNothingInitializer
    {
      typedef Behaviour<H,AsterReal,false> BV;
      typedef typename BV::BehaviourData  BData;
      TFEL_ASTER_INLINE static void
	exe(BData&,const AsterReal * const)
      {}
    }; // end of struct DoNothingInitializer

    struct TFEL_VISIBILITY_LOCAL Error
    {
	
      TFEL_ASTER_INLINE
      Error(const AsterReal *const,
	    const AsterReal *const,
	    const AsterReal *const,
	    const AsterReal *const,
	    const AsterReal *const,
	    const AsterReal *const,
	    const AsterReal *const,
	    const AsterReal *const,
	    const AsterReal *const,
	    const AsterReal *const,
	    const StressFreeExpansionHandler&)
      {} // end of Error

      TFEL_ASTER_INLINE void exe(AsterReal *const,
				 AsterReal *const,
				 AsterReal *const)
	throw(AsterException)
      {
	typedef tfel::material::MechanicalBehaviourTraits<Behaviour<H,AsterReal,false>> Traits;
	throw(AsterInvalidModellingHypothesis(Traits::getName(),H));
	return;
      } // end of Error::exe
	
    }; // end of struct Error

    template<const bool bs,     // requires StiffnessOperator
	     const bool ba>     // requires ThermalExpansionCoefficientTensor
      struct TFEL_VISIBILITY_LOCAL Integrator
    {
      typedef typename std::conditional<bs,
				      StiffnessOperatorInitializer,
				      DoNothingInitializer>::type SInitializer;

      typedef typename std::conditional<ba,
				      ThermalExpansionCoefficientTensorInitializer,
				      DoNothingInitializer>::type AInitializer;

      TFEL_ASTER_INLINE Integrator(const AsterReal *const DTIME ,
				   const AsterReal *const STRAN ,
				   const AsterReal *const DSTRAN,
				   const AsterReal *const TEMP,
				   const AsterReal *const DTEMP,
				   const AsterReal *const PROPS ,
				   const AsterReal *const PREDEF,
				   const AsterReal *const DPRED,
				   const AsterReal *const STATEV,
				   const AsterReal *const STRESS,
				   const StressFreeExpansionHandler& sfeh)
	: behaviour(DTIME,TEMP,DTEMP,
		    PROPS+AsterTraits<BV>::elasticPropertiesOffset+
		    AsterTraits<BV>::thermalExpansionPropertiesOffset,
		    STATEV,PREDEF,DPRED),
	dt(*DTIME)
	  {
	    using namespace tfel::material;
	    typedef MechanicalBehaviourTraits<BV> Traits;
	    typedef typename std::conditional<
	      Traits::hasStressFreeExpansion,
	      DrivingVariableInitialiserWithStressFreeExpansion,
	      DrivingVariableInitialiserWithoutStressFreeExpansion>::type DVInitializer;
	    SInitializer::exe(this->behaviour,PROPS);
	    AInitializer::exe(this->behaviour,PROPS);
	    DVInitializer::exe(this->behaviour,STRAN,DSTRAN,sfeh);
	    this->behaviour.setASTERBehaviourDataThermodynamicForces(STRESS);
	    this->behaviour.initialize();
	    // 22/03/2012 : la gestion des bornes sera implantée plus tard
	    // const auto& up = AsterOutOfBoundsPolicy::getAsterOutOfBoundsPolicy();
	    // this->behaviour.setOutOfBoundsPolicy(up.getOutOfBoundsPolicy());
	  } // end of Integrator::Integrator
	
      TFEL_ASTER_INLINE2
	void exe(AsterReal *const DDSOE,
		 AsterReal *const STRESS,
		 AsterReal *const STATEV)
      {
	using namespace tfel::material;
	typedef MechanicalBehaviourTraits<BV> Traits;
	typedef typename std::conditional<
	  Traits::hasConsistentTangentOperator,
	  typename std::conditional<
	    Traits::isConsistentTangentOperatorSymmetric,
	    SymmetricConsistentTangentOperatorComputer,
	    GeneralConsistentTangentOperatorComputer>::type,
	  ConsistentTangentOperatorIsNotAvalaible
	  >::type ConsistentTangentOperatorHandler;
	typedef typename std::conditional<
	  Traits::hasPredictionOperator,
	  StandardPredictionOperatorComputer,
	  PredictionOperatorIsNotAvalaible
	  >::type PredictionOperatorComputer;
	if(this->dt<0.){
	  throwNegativeTimeStepException(Traits::getName());
	}
	this->behaviour.checkBounds();
	typename BV::IntegrationResult r = BV::SUCCESS;
	const typename BV::SMFlag smflag = AsterTangentOperatorFlag<AsterTraits<BV>::btype>::value;
	if((-3.25<*DDSOE)&&(*DDSOE<-2.75)){
	  r = PredictionOperatorComputer::exe(this->behaviour,smflag,
					      BV::TANGENTOPERATOR);
	} else if((-2.25<*DDSOE)&&(*DDSOE<-1.75)){
	  r = PredictionOperatorComputer::exe(this->behaviour,smflag,
					      BV::SECANTOPERATOR);
	} else if((-1.25<*DDSOE)&&(*DDSOE<-0.75)){
	  r = PredictionOperatorComputer::exe(this->behaviour,smflag,
					      BV::ELASTIC);
	} else if((-0.25<*DDSOE)&&(*DDSOE<0.25)){
	  r = this->behaviour.integrate(smflag,BV::NOSTIFFNESSREQUESTED);
	} else if((0.75<*DDSOE)&&(*DDSOE<1.25)){
	  r = this->behaviour.integrate(smflag,BV::ELASTIC);
	} else if((1.75<*DDSOE)&&(*DDSOE<2.25)){
	  r = this->behaviour.integrate(smflag,BV::SECANTOPERATOR);
	} else if((2.75<*DDSOE)&&(*DDSOE<3.25)){
	  r = this->behaviour.integrate(smflag,BV::TANGENTOPERATOR);
	} else if((3.75<*DDSOE)&&(*DDSOE<4.25)){
	  r = this->behaviour.integrate(smflag,BV::CONSISTENTTANGENTOPERATOR);
	} else {
	  throwInvalidDDSOEException(Traits::getName(),*DDSOE);
	}
	if(r==BV::FAILURE){
	  // Il manque un vraie gestion locale de résultats imprécis
	  if(*DDSOE<-0.5){
	    throwPredictionComputationFailedException(Traits::getName());
	  } else {
	    throwBehaviourIntegrationFailedException(Traits::getName());
	  }
	}
	this->behaviour.checkBounds();
	this->behaviour.ASTERexportStateData(STRESS,STATEV);
	if((*DDSOE>0.5)||(*DDSOE<-0.5)){
	  ConsistentTangentOperatorHandler::exe(this->behaviour,DDSOE);
	}
      } // end of Integrator::exe
	
    private:

      struct StandardPredictionOperatorComputer
      {
	typedef Behaviour<H,AsterReal,false> BV;
	static typename BV::IntegrationResult
	exe(BV& b,const typename BV::SMFlag smf,const typename BV::SMType smt)
	{
	  return b.computePredictionOperator(smf,smt);
	} // end of exe	  
      };

      struct PredictionOperatorIsNotAvalaible
      {
	typedef Behaviour<H,AsterReal,false> BV;
	static typename BV::IntegrationResult
	exe(BV&,const typename BV::SMFlag,
	    const typename BV::SMType)
	{
	  typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
	  throwPredictionOperatorIsNotAvalaible(Traits::getName());
	  return BV::FAILURE;
	} // end of exe	  
      };
      
      struct ConsistentTangentOperatorIsNotAvalaible
      {
	typedef Behaviour<H,AsterReal,false> BV;
	static void exe(BV&,AsterReal *const)
	{
	  typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
	  throwConsistentTangentOperatorIsNotAvalaible(Traits::getName());
	} // end of exe	  
      };

      struct ConsistentTangentOperatorComputer
      {
	typedef Behaviour<H,AsterReal,false> BV;
	static void exe(const BV& bv,AsterReal *const DDSOE)
	{
	  using tfel::material::ModellingHypothesisToSpaceDimension;
	  const unsigned short N = ModellingHypothesisToSpaceDimension<H>::value;
	  using  TangentOperatorType     = typename AsterTangentOperatorType<AsterTraits<BV>::btype,N>::type;
	  using  TangentOperatorViewType = typename AsterTangentOperatorType<AsterTraits<BV>::btype,N>::view_type;
	  TangentOperatorViewType Dt{DDSOE};
	  Dt = static_cast<const TangentOperatorType&>(bv.getTangentOperator());
	  // l'opérateur tangent contient des sqrt(2) en petites déformations...
	  AsterTangentOperator::normalize(Dt);
	} // end of exe	  
      };

      struct SymmetricConsistentTangentOperatorComputer
      {
	typedef Behaviour<H,AsterReal,false> BV;
	static void exe(const BV& bv,AsterReal *const DDSOE)
	{
	  ConsistentTangentOperatorComputer::exe(bv,DDSOE);
	} // end of exe	  
      };

      struct GeneralConsistentTangentOperatorComputer
      {
	typedef Behaviour<H,AsterReal,false> BV;
	static void exe(const BV& bv,AsterReal *const DDSOE)
	{
	  using tfel::material::ModellingHypothesisToSpaceDimension;
	  const unsigned short N = ModellingHypothesisToSpaceDimension<H>::value;
	  using  TangentOperatorType     = typename AsterTangentOperatorType<AsterTraits<BV>::btype,N>::type;
	  using  TangentOperatorViewType = typename AsterTangentOperatorType<AsterTraits<BV>::btype,N>::view_type;
	  ConsistentTangentOperatorComputer::exe(bv,DDSOE);
	  // les conventions fortran.... (petites déformations et modèles de zones cohésives)
	  TangentOperatorViewType Dt{DDSOE};
	  Dt = static_cast<const TangentOperatorType&>(bv.getTangentOperator());
	  AsterTangentOperator::transpose(Dt);
	} // end of exe	  
      };

      typedef Behaviour<H,AsterReal,false> BV;
      BV behaviour;
      AsterReal dt;
    }; // end of struct Integrator

    TFEL_ASTER_INLINE2 static void
      checkNPROPS(const AsterInt NPROPS)
    {
      using namespace std;
      using namespace tfel::material;
      typedef Behaviour<H,AsterReal,false> BV;
      typedef MechanicalBehaviourTraits<BV> Traits;
      const unsigned short offset  = (AsterTraits<BV>::elasticPropertiesOffset+
				      AsterTraits<BV>::thermalExpansionPropertiesOffset);
      const unsigned short nprops  = AsterTraits<BV>::material_properties_nb;
      const unsigned short NPROPS_ = offset+nprops == 0 ? 1u : offset+nprops; 
      const bool is_defined_       = Traits::is_defined;
      //Test if the nb of properties matches Behaviour requirements
      if((NPROPS!=NPROPS_)&&is_defined_){
	throwUnMatchedNumberOfMaterialProperties(Traits::getName(),
						 NPROPS_,NPROPS);
      }
    } // end of checkNPROPS
      
    TFEL_ASTER_INLINE2 static void
      checkNSTATV(const AsterInt NSTATV)
    {
      typedef Behaviour<H,AsterReal,false> BV;
      typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
      const unsigned short nstatv  = Traits::internal_variables_nb;
      const unsigned short NSTATV_ = nstatv == 0 ? 1u : nstatv;
      const bool is_defined_       = Traits::is_defined;
      //Test if the nb of state variables matches Behaviour requirements
      if((NSTATV_!=NSTATV)&&is_defined_){
	throwUnMatchedNumberOfStateVariables(Traits::getName(),
					     NSTATV_,NSTATV);
      }
    } // end of checkNSTATV
      
  }; // end of struct AsterBehaviourHandler
  
} // end of namespace aster

#include"MFront/Aster/AsterIsotropicBehaviourHandler.hxx"
#include"MFront/Aster/AsterOrthotropicBehaviourHandler.hxx"

#endif /* LIB_MFRONT_ASTER_ASTERBEHAVIOURHANDLER_H_ */
