/*!
 * \file   include/TFEL/Utilities/ToString.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   04 f�v 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_TOSTRING_IXX_
#define _LIB_TFEL_TOSTRING_IXX_ 

namespace tfel{
  
  namespace utilities{

    template<typename T>
    std::string
    ToString(const T& src)
    {
      std::ostringstream os;
      os << src;
      return os.str();
    }

  } // end of namespace utilities

} // end of namespace tfel

#endif /* _LIB_TFEL_TOSTRING_IXX */
