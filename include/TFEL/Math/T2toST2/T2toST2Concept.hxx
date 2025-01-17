/*!
 * \file   include/TFEL/Math/T2toST2/T2toST2Concept.hxx
 * \brief
 * \author Thomas Helfer
 * \date   19 November 2013
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_T2TOST2CONCEPT_HXX
#define LIB_TFEL_MATH_T2TOST2CONCEPT_HXX 1

#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Metaprogramming/Implements.hxx"
#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/TypeTraits/IsFundamentalNumericType.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/General/ConceptRebind.hxx"
#include "TFEL/Math/Tensor/TensorConcept.hxx"
#include "TFEL/Math/Stensor/StensorConcept.hxx"

namespace tfel::math {

  /*!
   * \class T2toST2Tag
   * \brief Helper class to characterise t2tost2.
   */
  struct T2toST2Tag {};

  template <class T>
  struct T2toST2Concept {
    typedef T2toST2Tag ConceptTag;

   protected:
    T2toST2Concept() = default;
    T2toST2Concept(T2toST2Concept&&) = default;
    T2toST2Concept(const T2toST2Concept&) = default;
    T2toST2Concept& operator=(const T2toST2Concept&) = default;
    ~T2toST2Concept() = default;
  };

  /*!
   * \brief an helper function which returns if the given type implements the
   * `T2toST2Concept`.
   * \tparam T2toST2Type: type tested
   */
  template <typename T2toST2Type>
  TFEL_HOST_DEVICE constexpr bool implementsT2toST2Concept() {
    return tfel::meta::implements<T2toST2Type, T2toST2Concept>();
  }  // end of implementsT2toST2Concept

  //! paratial specialisation for T2toST2
  template <typename Type>
  struct ConceptRebind<T2toST2Tag, Type> {
    using type = T2toST2Concept<Type>;
  };

  template <typename T2toST2Type>
  typename std::enable_if<
      implementsT2toST2Concept<T2toST2Type>(),
      typename tfel::typetraits::AbsType<numeric_type<T2toST2Type>>::type>::type
  abs(const T2toST2Type&);

  /*!
   * \brief compute de derivative of the push-forward of a symmetric
   * second order tensor with respect to the deformation gradient, i.e.
   * it computes the derivative of F . S . transpose(F) with respect to F
   * assuming that S is constant
   *
   * \param[out] : derivative of the push-forward symmetric tensor
   * \param[in]  : orginal tensor
   * \param[in]  : deformation gradient
   */
  template <typename T2toST2ResultType,
            typename StensorType,
            typename TensorType>
  typename std::enable_if<implementsT2toST2Concept<T2toST2ResultType>() &&
                              implementsStensorConcept<StensorType>() &&
                              implementsTensorConcept<TensorType>() &&
                              tfel::typetraits::IsFundamentalNumericType<
                                  numeric_type<TensorType>>::cond &&
                              isAssignableTo<numeric_type<StensorType>,
                                             numeric_type<T2toST2ResultType>>(),
                          void>::type
  computePushForwardDerivativeWithRespectToDeformationGradient(
      T2toST2ResultType&, const StensorType&, const TensorType&);
  /*!
   * \brief compute the Cauchy stress derivative from the Kirchhoff
   * stress derivative with respect to the deformation gradient
   * \param[out] ds   : Cauchy stress derivative
   * \param[in]  dt_K : Kirchhoff stress derivative
   * \param[in]  s    : Cauchy stress
   * \param[in]  F    : deformation gradient
   */
  template <typename T2toST2ResultType,
            typename T2toST2Type,
            typename StensorType,
            typename TensorType>
  typename std::enable_if<
      implementsT2toST2Concept<T2toST2ResultType>() &&
          implementsT2toST2Concept<T2toST2Type>() &&
          implementsStensorConcept<StensorType>() &&
          implementsTensorConcept<TensorType>() &&
          getSpaceDimension<T2toST2ResultType>() ==
              getSpaceDimension<T2toST2Type>() &&
          getSpaceDimension<T2toST2ResultType>() ==
              getSpaceDimension<StensorType>() &&
          getSpaceDimension<T2toST2ResultType>() ==
              getSpaceDimension<TensorType>() &&
          tfel::typetraits::IsFundamentalNumericType<
              numeric_type<TensorType>>::cond &&
          isAssignableTo<typename ComputeBinaryResult<numeric_type<T2toST2Type>,
                                                      numeric_type<StensorType>,
                                                      OpPlus>::Result,
                         numeric_type<T2toST2ResultType>>(),
      void>::type
  computeCauchyStressDerivativeFromKirchhoffStressDerivative(T2toST2ResultType&,
                                                             const T2toST2Type&,
                                                             const StensorType&,
                                                             const TensorType&);
  /*!
   * \brief compute the Kirchhoff stress derivative from the Cauchy
   * stress derivative with respect to the deformation gradient
   * \param[out] dt_K : Kirchhoff stress derivative
   * \param[in]  ds   : Cauchy stress derivative
   * \param[in]  s    : Cauchy stress
   * \param[in]  F    : deformation gradient
   */
  template <typename T2toST2ResultType,
            typename T2toST2Type,
            typename StensorType,
            typename TensorType>
  typename std::enable_if<
      implementsT2toST2Concept<T2toST2ResultType>() &&
          implementsT2toST2Concept<T2toST2Type>() &&
          implementsStensorConcept<StensorType>() &&
          implementsTensorConcept<TensorType>() &&
          getSpaceDimension<T2toST2ResultType>() ==
              getSpaceDimension<T2toST2Type>() &&
          getSpaceDimension<T2toST2ResultType>() ==
              getSpaceDimension<StensorType>() &&
          getSpaceDimension<T2toST2ResultType>() ==
              getSpaceDimension<TensorType>() &&
          tfel::typetraits::IsFundamentalNumericType<
              numeric_type<TensorType>>::cond &&
          isAssignableTo<typename ComputeBinaryResult<numeric_type<T2toST2Type>,
                                                      numeric_type<StensorType>,
                                                      OpPlus>::Result,
                         numeric_type<T2toST2ResultType>>(),
      void>::type
  computeKirchhoffStressDerivativeFromCauchyStressDerivative(T2toST2ResultType&,
                                                             const T2toST2Type&,
                                                             const StensorType&,
                                                             const TensorType&);

}  // end of namespace tfel::math

#include "TFEL/Math/T2toST2/T2toST2Concept.ixx"

#endif /* LIB_TFEL_MATH_T2TOST2CONCEPT_HXX */
