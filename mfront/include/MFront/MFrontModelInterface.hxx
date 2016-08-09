/*!
 * \file   MFrontModelInterface.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   03 juin 2016
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_MFRONTMODELINTERFACE_HXX
#define LIB_MFRONT_MFRONTMODELINTERFACE_HXX

#include"MFront/AbstractModelInterface.hxx"

namespace mfront {

  /*!
   * \brief 
   */
  struct MFrontModelInterface final
    : public AbstractModelInterface
  {
    //! \return the name of the interface
    static std::string getName(void);
    //! constructor
    MFrontModelInterface();
    /*!
     * \param[in]     k:  key being treated
     * \param[in/out] p:  iterator to the token after the key being treated
     * \param[out]    pe: iterator past the end of the file
     * \return a pair. The first member of the pair is true if the
     * keyword was handled by interface. The second member of the pair
     * is an iterator after the last token used to treat the keyword.
     * pair.
     */ 
    virtual std::pair<bool,TokensContainer::const_iterator>
    treatKeyword(const std::string&,
		 TokensContainer::const_iterator,
		 const TokensContainer::const_iterator) override;
    /*!
     * \param[out] names: reserved names
     */
    virtual void
    declareReservedNames(std::set<std::string>&) override;
    /*!
     * \brief write the output files for the given model description
     * \param[in] fd: file description
     * \param[in] md: model description
     */
    virtual void
    writeOutputFiles(const FileDescription&,
		     const ModelDescription&) override;
    /*!
     * \brief : fill the target descripton
     * \param[out] td: target description
     * \param[in]  md: model description
     */
    virtual void getTargetsDescription(TargetsDescription&,
				       const ModelDescription&) override;
    //! destructor
    virtual ~MFrontModelInterface();
  }; // end of struct MFrontModelInterface

} // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTMODELINTERFACE_HXX_ */