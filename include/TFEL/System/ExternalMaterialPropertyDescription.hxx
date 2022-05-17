/*!
 * \file   include/TFEL/System/ExternalMaterialPropertyDescription.hxx
 * \brief
 * \author Thomas Helfer
 * \date   15 June 2015
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_SYSTEM_EXTERNALMATERIALPROPERTYDESCRIPTION_HXX
#define LIB_TFEL_SYSTEM_EXTERNALMATERIALPROPERTYDESCRIPTION_HXX

#include <string>
#include <vector>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/System/ExternalMaterialKnowledgeDescription.hxx"

namespace tfel::system {

  /*!
   * \brief a structure containing the information about a given material
   * property.
   */
  struct TFELSYSTEM_VISIBILITY_EXPORT ExternalMaterialPropertyDescription
      : public ExternalMaterialKnowledgeDescription {
    /*!
     * \param[in] l: library
     * \param[in] m: material property
     */
    ExternalMaterialPropertyDescription(const std::string&, const std::string&);
    //! \brief default constructor
    ExternalMaterialPropertyDescription();
    //! \brief move constructor
    ExternalMaterialPropertyDescription(ExternalMaterialPropertyDescription&&);
    //! \brief move constructor
    ExternalMaterialPropertyDescription(const ExternalMaterialPropertyDescription&);
    //! \brief move assignement
    ExternalMaterialPropertyDescription& operator=(ExternalMaterialPropertyDescription&&);
    //! \brief standard assignement
    ExternalMaterialPropertyDescription& operator=(
        const ExternalMaterialPropertyDescription&);
    //! \brief destructor
    ~ExternalMaterialPropertyDescription();
    //! \brief name of the law defining the material property
    std::string law;
    //! \brief name of the enty point associated with the material property
    std::string material_property;
    //! \brief list of arguments
    std::vector<std::string> arguments;
    //! \brief list of parameters
    std::vector<std::string> parameters;
  };  // end of struct ExternalMaterialPropertyDescription

  //! \brief a simple alias for backward compatibility
  using ExternalMaterialPropertyData = ExternalMaterialPropertyDescription;

}  // end of namespace tfel::system

extern "C" {

/*!
 * \author DeltaCAD
 * \brief build a new behaviour data
 * \note the behaviour data must be freed using
 * `freeExternalMaterialPropertyData`
 */
TFELSYSTEM_VISIBILITY_EXPORT tfel::system::ExternalMaterialPropertyData*
newExternalMaterialPropertyData();
/*!
 * \author DeltaCAD
 * \brief build a new behaviour data
 * \note  the behaviour data
 */
TFELSYSTEM_VISIBILITY_EXPORT void freeExternalMaterialPropertyData(
    tfel::system::ExternalMaterialPropertyData*);
/*!
 * \author DeltaCAD
 * \brief retrieving the information from a behaviour
 * \param[in] d: data to be fetched
 * \param[in] l: library
 * \param[in] m: material property
 *
 * \return nullptr on success, an error message otherwise.\n
 * If an error message is set, it must be freed  using std::free
 * If the allocation of the error message fails, std::exit is called
 */
TFELSYSTEM_VISIBILITY_EXPORT char* getExternalMaterialPropertyData(
    tfel::system::ExternalMaterialPropertyData* const,
    const char* const,
    const char* const);

}  // end of extern "C"

#endif /* LIB_TFEL_SYSTEM_EXTERNALMATERIALPROPERTYDESCRIPTION_HXX */
