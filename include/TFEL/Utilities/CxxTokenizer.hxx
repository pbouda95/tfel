/*!
 * \file   include/TFEL/Utilities/CxxTokenizer.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   12 nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_TFEL_CXXTOKENIZER_HXX_
#define LIB_TFEL_CXXTOKENIZER_HXX_ 

#include<map>
#include<list>
#include<string>
#include<vector>
#include<iosfwd>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Utilities/Token.hxx"

namespace tfel{

  namespace utilities{

    /*!
     * class in charge of reading a C++-style file and converting it
     * in tokens.
     */
    struct TFELUTILITIES_VISIBILITY_EXPORT CxxTokenizer
    {
    
      //! a simple alias
      typedef std::list<Token> TokensContainer;
      //! a simple alias
      typedef TokensContainer::const_iterator const_iterator;
      //! a simple alias
      typedef TokensContainer::size_type size_type;

      /*!
       * \return true if the given string is a valid variable name.
       * \param[in] s : variable name
       * \param[in] b : boolean telling if C++ keywords are allowed.
       */
      static bool
      isValidIdentifier(const std::string&,const bool=true);

      /*!
       * \brief default constructor
       */
      CxxTokenizer();

      /*!
       * \brief constructor
       * Open the given file.
       * \param[in] f : file name
       */
      CxxTokenizer(const std::string&);

      /*!
       * \brief import the specified tokens
       * \param[in] p : iterator to the first token
       * \param[in] pe : iterator past the last token
       */
      template<typename InputIterator>
      void import(const InputIterator,
		  const InputIterator);
    
      /*!
       * \brief analyse the string given
       * all previous tokens are erased
       * \param[in] s : string to be parsed
       */ 
      void parseString(const std::string&);

      /*!
       * \brief import the specified file
       * \param[in] f : file name
       */ 
      void openFile(const std::string&);
      /*!
       * \brief print the stored tokens to the specified stream
       * \param[out] out : output stream
       */ 
      void printFileTokens(std::ostream&) const;
      /*!
       * \brief assume that a C-style comment was opened at the
       * beginning of the processing.
       * This is usefull when parsing a file line by line
       * \param[in] b: boolean
       */
      void setCStyleCommentOpened(const bool);
      /*!
       * \brief remove all comment tokens
       */ 
      void stripComments(void);
      /*!
       * \brief clear the object
       */ 
      void clear(void);
      /*!
       * \brief set if char tokens shall be treated as string tokens
       * \param[in] b : boolean telling if char tokens shall be
       * treated as string tokens
       */ 
      void treatCharAsString(const bool);
      /*!
       * \brief set if tokens shall be split to extract numbers
       * \param[in] b : boolean telling numbers shall be extracted
       * from tokens
       */ 
      void extractNumbers(const bool);
      /*!
       * \brief an helper method throwing an exception if the given
       * iterator is egal to the third argument (which shall point
       * after the last token of the line)
       * \param[in] method : calling method name
       * \param[in] p  : iterator to the current token
       * \param[in] pe : iterator pointing after the last token of the
       * line
       * \throw std::runtime_error if the given iterator is egal to
       * the third argument
       */ 
      static void
      checkNotEndOfLine(const std::string&,
			const_iterator&, 
			const const_iterator);

      /*!
       * \brief an helper method throwing an exception if the given
       * iterator is egal to the fourth argument (which shall point
       * after the last token of the line)
       * \param[in] method : calling method name
       * \param[in] error  : error message
       * \param[in] p  : iterator to the current token
       * \param[in] pe : iterator pointing after the last token of the
       * line
       * \throw std::runtime_error if the given iterator is egal to
       * the fourth argument
       */ 
      static void
      checkNotEndOfLine(const std::string&,
			const std::string&,
			const_iterator&, 
			const const_iterator);

      /*!
       * \brief an helper method throwing an exception if the given
       * iterator is egal to the fourth argument (which shall point
       * after the last token of the line) or if the given token is
       * not equal to the expected value
       * \param[in] method : calling method name
       * \param[in] value  : expected value
       * \param[in] p  : iterator to the current token
       * \param[in] pe : iterator pointing after the last token of the
       * line
       * \throw std::runtime_error if the given iterator is egal to
       * the fourth argument
       */ 
      static void
      readSpecifiedToken(const std::string&,
			 const std::string&,
			 const_iterator&, 
			 const const_iterator);

      /*!
       * \brief an helper method to extract an unsigned int from the
       * given token
       * \param[in] p  : iterator to the current token
       * \param[in] pe : iterator pointing after the last token of the
       * line
       * \return the extracted value
       * \throw std::runtime_error if the given iterator is egal to
       * the second argument
       */ 
      static unsigned int
      readUnsignedInt(const_iterator&, 
		      const const_iterator);

      /*!
       * \brief an helper method to extract a string from the
       * given token
       * \param[in] p  : iterator to the current token
       * \param[in] pe : iterator pointing after the last token of the
       * line
       * \return the extracted value
       * \throw std::runtime_error if the given iterator is egal to
       * the second argument or if the given token is not a string
       */ 
      static std::string
      readString(const_iterator&, 
		 const const_iterator);
      /*!
       * \brief an helper method to extract a set of values from the
       * given as an array
       * \param[in] method : calling method name
       * \param[out] v  : the extracted array
       * \param[in]  p  : iterator to the current token
       * \param[in]  pe : iterator pointing after the last token of the
       * line
       * \throw std::runtime_error if the given iterator is egal to
       * the second argument or if the given token is not a string
       */ 
      static void
      readArray(const std::string&,
		std::vector<std::string>&,
		const_iterator&, 
		const const_iterator);
      /*!
       * \brief an helper method to extract a set of values from the
       * given as an array
       * \param[out] v  : the extracted array
       * \param[in]  p  : iterator to the current token
       * \param[in]  pe : iterator pointing after the last token of the
       * line
       * \throw std::runtime_error if the given iterator is egal to
       * the second argument or if the given token is not a string
       */ 
      static std::vector<std::string>
      readStringArray(const_iterator&, 
		      const const_iterator);
      /*!
       * \brief an helper method to extract a double from the given
       * token
       * \param[in] p  : iterator to the current token
       * \param[in] pe : iterator pointing after the last token of the
       * line
       * \return the extracted value
       * \throw std::runtime_error if the given iterator is egal to
       * the second argument
       */ 
      static double
      readDouble(const_iterator&, 
		 const const_iterator);	
      /*!
       * \brief an helper method to extract a int from the given
       * token
       * \param[in] p  : iterator to the current token
       * \param[in] pe : iterator pointing after the last token of the
       * line
       * \return the extracted value
       * \throw std::runtime_error if the given iterator is egal to
       * the second argument
       */ 
      static int
      readInt(const_iterator&, 
	      const const_iterator);	
      //! \return an iterator to the first token
      const_iterator begin(void) const;
      //! \return an iterator past the last token
      const_iterator end(void) const;
      /*!
       * \return the ith token
       * \param[in] i : index
       * \throw an exception if i>=this->size()
       */
      const Token& operator[](const size_type) const;
      //! \return the number of token
      size_type size() const;
      //! destructor
      virtual ~CxxTokenizer();

    private:

      static std::string TFEL_VISIBILITY_LOCAL
      readNumber(std::string::const_iterator&,
		 const std::string::const_iterator);

      static void TFEL_VISIBILITY_LOCAL
      extractNumbers(std::vector<std::string>&,
		     const std::string&);

      static void TFEL_VISIBILITY_LOCAL
      joinIncludes(std::vector<std::string>&);

      static void TFEL_VISIBILITY_LOCAL
      join(std::vector<std::string>&,const std::string&,const std::string&);

      TFEL_VISIBILITY_LOCAL void
      splitAtCxxTokenSperator(std::vector<std::string>&);
    
      static void TFEL_VISIBILITY_LOCAL
      splitString(std::vector<std::string>&,std::string,const std::string);

      static void TFEL_VISIBILITY_LOCAL
      splitString2(std::vector<std::string>&,std::string);

      static void TFEL_VISIBILITY_LOCAL
      splitString3(std::vector<std::string>&,std::string);

      TFEL_VISIBILITY_LOCAL 
      static TokensContainer::iterator TFEL_VISIBILITY_LOCAL
      joinPreviousCurrentNext(TokensContainer&,TokensContainer&,
			      TokensContainer::iterator);

    protected:

      static bool
      isValidFunctionIdentifier(const std::string&,const bool=true);

      void
      splitLine(std::string,const unsigned int);

      void
      treatPreprocessorDirectives(void);

      void
      splitTokens(void);

      /*!
       * \param[in/out] l: line treated
       * \param[in]     n: line number
       * \param[p]      p: position of the C comment
       */
      void treatString(std::string&,
		       const Token::size_type,
		       const std::string::size_type);
      /*!
       * \param[in/out] l: line treated
       * \param[in]     n: line number
       * \param[p]      p: position of the C comment
       */
      void treatCxxComment(std::string&,
			   const Token::size_type,
			   const std::string::size_type);
      /*!
       * \param[in/out] l: line treated
       * \param[in]     n: line number
       * \param[p]      p: position of the C comment
       */
      void treatCComment(std::string&,
			 const Token::size_type,
			 const std::string::size_type);
      /*!
       * \param[in/out] l: line treated
       * \param[in]     n: line number
       * \param[p]      p: position of the C comment
       */
      void treatCChar(std::string&,
		      const Token::size_type,
		      const std::string::size_type);
      
      TokensContainer tokens;

      //! store all the comments of a line
      std::map<Token::size_type,std::string> comments;

      bool cStyleCommentOpened = false;

      bool bExtractNumbers = true;

      bool charAsString = false;

    }; // end of struct CxxTokenizer

  } // end of namespace utilities

} // end of namespace tfel

#include"TFEL/Utilities/CxxTokenizer.ixx"

#endif /* LIB_TFEL_CXXTOKENIZER_HXX_ */
