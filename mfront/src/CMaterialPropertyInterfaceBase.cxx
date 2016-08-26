/*!
 * \file   mfront/src/CMaterialPropertyInterfaceBase.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<sstream>
#include<stdexcept>

#include"MFront/DSLUtilities.hxx"
#include"MFront/MFrontHeader.hxx"
#include"MFront/FileDescription.hxx"
#include"MFront/MaterialPropertyDescription.hxx"
#include"MFront/CMaterialPropertyInterfaceBase.hxx"

#ifndef _MSC_VER
static const char * const constexpr_c = "constexpr";
#else
static const char * const constexpr_c = "const";
#endif

namespace mfront
{

  static std::string
  transformHeaderName(const std::string& h)
  {
    auto replace = [](std::string& s,
		      const std::string::value_type a,
		      const std::string::value_type b){
      std::string::size_type p = 0;
      while((p=s.find(a))!=std::string::npos){
	s[p]=b;
      }
    };
    auto header = makeUpperCase(h);
    replace(header,'.','_');
    replace(header,'-','_');
    replace(header,'/','_');
    return header;
  } // end of transformHeaderName
  
  CMaterialPropertyInterfaceBase::CMaterialPropertyInterfaceBase()
  {}
  
  void
  CMaterialPropertyInterfaceBase::writeHeaderPreprocessorDirectives(const MaterialPropertyDescription&)
  {
    writeExportDirectives(this->headerFile);
  } // end of CMaterialPropertyInterfaceBase::writeSrcPreprocessorDirectives

  void
  CMaterialPropertyInterfaceBase::writeSrcPreprocessorDirectives(const MaterialPropertyDescription&)
  {} // end of CMaterialPropertyInterfaceBase::writeSrcPreprocessorDirectives
  
  CMaterialPropertyInterfaceBase::~CMaterialPropertyInterfaceBase()
  {}

  void
  CMaterialPropertyInterfaceBase::writeOutputFiles(const MaterialPropertyDescription& mpd,
						   const FileDescription& fd)
  {
    const auto header = this->getHeaderFileName(mpd.material,mpd.className);
    const auto src    = this->getSrcFileName(mpd.material,mpd.className);
    // the fortran interface does not need any header...
    if(!header.empty()){
      this->headerFileName  = "include/" + header +".hxx";
      this->headerFile.open(this->headerFileName);
      if(!this->headerFile){
	throw(std::runtime_error("CMaterialPropertyInterfaceBase::writeOutputFiles : "
				 "unable to open '"+this->headerFileName+
				 "' for writing output file."));
      }
      this->headerFile.exceptions(std::ios::badbit|std::ios::failbit);
      this->writeHeaderFile(mpd,fd);
      this->headerFile.close();
    }
    // opening the source file
    this->srcFileName  = "src/" + src +".cxx";
    this->srcFile.open(this->srcFileName);
    if(!this->srcFile){
      throw(std::runtime_error("CMaterialPropertyInterfaceBase::writeOutputFiles : "
			       "unable to open '"+this->srcFileName+
			       "' for writing output file."));
    }
    this->srcFile.exceptions(std::ios::badbit|std::ios::failbit);
    this->writeSrcFile(mpd,fd);
    this->srcFile.close();
  } // end of CMaterialPropertyInterfaceBase::writeOutputFiles

  std::string
  CMaterialPropertyInterfaceBase::getCallingConvention(void) const{
    return "";
  }
  
  void
  CMaterialPropertyInterfaceBase::writeHeaderFile(const MaterialPropertyDescription& mpd,
						  const FileDescription& fd)
  {
    const auto header = this->getHeaderFileName(mpd.material,mpd.className);
    const auto headerGard = transformHeaderName(header)+"_HH";
    this->headerFile << "/*!\n"
		     << "* \\file   " << this->headerFileName  << "\n"
		     << "* \\brief  " << "this file declares the " 
		     << mpd.className << " MaterialLaw.\n"
		     << "*         File generated by "
		     << MFrontHeader::getVersionName() << " "
		     << "version " << MFrontHeader::getVersionNumber()
		     << "\n";
    if(!fd.authorName.empty()){
      this->headerFile << "* \\author " << fd.authorName << "\n";
    }
    if(!fd.date.empty()){
      this->headerFile << "* \\date   " << fd.date       << "\n";
    }
    if(!fd.description.empty()){
      this->headerFile << fd.description << "\n";
    }
    this->headerFile << " */\n\n"
		     << "#ifndef " << headerGard << "\n"
		     << "#define " << headerGard << "\n\n";
    writeExportDirectives(this->headerFile);
    this->writeHeaderPreprocessorDirectives(mpd);
    this->writeBeginHeaderNamespace();
    this->headerFile << "MFRONT_SHAREDOBJ double "
		     << this->getCallingConvention() << '\n'
		     << this->getFunctionName(mpd.material,mpd.className);
    this->headerFile << "(";
    this->writeParameterList(this->headerFile,mpd.inputs);
    this->headerFile << ");\n\n";
    if(((!mpd.bounds.empty())||(!mpd.physicalBounds.empty()))||
       (this->requiresCheckBoundsFunction())){
      this->headerFile << "MFRONT_SHAREDOBJ int "
		       << this->getCallingConvention() << '\n'
		       << this->getCheckBoundsFunctionName(mpd.material,mpd.className);
      this->headerFile << "(";
      this->writeParameterList(this->headerFile,mpd.inputs);
      this->headerFile << ");\n\n";
    }
    this->writeEndHeaderNamespace();
    this->headerFile << "#endif /* " << headerGard << " */\n";
  } // end of CMaterialPropertyInterfaceBase::writeHeaderFile(void)

  void
  CMaterialPropertyInterfaceBase::writeInterfaceSpecificVariables(const VariableDescriptionContainer&)
  {} // end of CMaterialPropertyInterfaceBase::writeInterfaceSpecificVariables

  void
  CMaterialPropertyInterfaceBase::writeParameterList(std::ostream& file,
						     const VariableDescriptionContainer& inputs){
    if(!inputs.empty()){
      for(auto p=inputs.begin();p!=inputs.end();){
	file << "const double " << p->name;
	if((++p)!=inputs.end()){
	  file << ",";
	}
      }
    } else {
      file << "void";
    }
  } // end of CMaterialPropertyInterfaceBase::writeParameterList

  static void writeBounds(std::ostream& out,
			  const std::vector<VariableBoundsDescription>& bounds,
			  const char * const s){
    for(const auto& b : bounds){
      if(b.boundsType==VariableBoundsDescription::Lower){
	out << "if(" << b.varName<< " < "<< b.lowerBound << "){\n"
	    << "return " << s << b.varNbr << ";\n"
	    << "}\n";
      } else if(b.boundsType==VariableBoundsDescription::Upper){
	out << "if(" << b.varName<< " > "<< b.upperBound << "){\n"
	    << "return " << s << b.varNbr << ";\n"
	    << "}\n";
      } else {
	out << "if((" << b.varName<< " < "<< b.lowerBound << ")||"
	    << "(" << b.varName<< " > "<< b.upperBound << ")){\n"
	    << "return " << s << b.varNbr << ";\n"
	    << "}\n";
      }
    }
  }
  
  void
  CMaterialPropertyInterfaceBase::writeSrcFile(const MaterialPropertyDescription& mpd,
					       const FileDescription& fd)
  {
    this->srcFile << "/*!\n"
		  << " * \\file   " << this->srcFileName  << '\n'
		  << " * \\brief  " << "this file implements the " 
		  << mpd.className << " MaterialLaw.\n"
		  << " *         File generated by "
		  << MFrontHeader::getVersionName() << " "
		  << "version " << MFrontHeader::getVersionNumber()
		  << '\n';
    if(!fd.authorName.empty()){
      this->srcFile << "* \\fd.author " << fd.authorName << '\n';
    }
    if(!fd.date.empty()){
      this->srcFile << "* \\date   " << fd.date       << '\n';
    }
    this->srcFile << " */\n\n"
		  << "#include<cmath>\n\n"
      		  << "#include<cerrno>\n\n"
		  << "#include<algorithm>\n\n";
    if(!mpd.includes.empty()){
      this->srcFile << mpd.includes << "\n\n";
    }
    const auto header = this->getHeaderFileName(mpd.material,mpd.className);
    if(!header.empty()){
      this->srcFile << "#include\""+header+".hxx\"\n\n";
    }
    this->writeSrcPreprocessorDirectives(mpd);
    this->writeBeginSrcNamespace();
    this->srcFile << "double " << this->getFunctionName(mpd.material,mpd.className)
		  << "(";
    this->writeParameterList(this->srcFile,mpd.inputs);
    this->srcFile << ")\n{\n"
		  << "using namespace std;\n"
		  << "using real = double;\n";
    // material laws
    writeMaterialLaws("CMaterialPropertyInterfaceBase::writeSrcFile",
		      this->srcFile,mpd.materialLaws);
    // static variables
    writeStaticVariables("CMaterialPropertyInterfaceBase::writeSrcFile",
			 this->srcFile,mpd.staticVars,fd.fileName);
    // parameters
    if(!mpd.parameters.empty()){
      for(const auto& p : mpd.parameters){
	const auto p6 = mpd.parametersValues.find(p);
	if(p6==mpd.parametersValues.end()){
	  throw(std::runtime_error("CMaterialPropertyInterfaceBase::writeSrcFile : "
				   "internal error (can't find value of parameter '"+p+"')"));
	}
	this->srcFile << "static " << constexpr_c << " real " << p << " = " << p6->second << ";\n";
      }
    }
    this->writeInterfaceSpecificVariables(mpd.inputs);
    if(!mpd.inputs.empty()){
      this->srcFile << "#ifndef MFRONT_NOERRNO_HANDLING\n"
		    << "const auto mfront_errno_old = errno;\n"
		    << "errno=0;\n"
		    << "#endif /* MFRONT_NOERRNO_HANDLING */\n";
    }
    this->srcFile << "real " << mpd.output << ";\n"
		  << mpd.f.body << "\n";
    if(!mpd.inputs.empty()){
      this->srcFile << "#ifndef MFRONT_NOERRNO_HANDLING\n"
	// can't use std::swap here as errno might be a macro
		    << "const auto mfront_errno = errno;\n"
		    << "errno = mfront_errno_old;\n"
		    << "if(mfront_errno!=0){\n";
      this->writeCErrorTreatment(this->srcFile,mpd);
      this->srcFile << "}\n"
		    << "#endif /* MFRONT_NOERRNO_HANDLING */\n";
    }
    this->srcFile << "return " << mpd.output << ";\n"
		  << "} /* end of " << mpd.className << " */\n\n";
    if(((!mpd.bounds.empty())||(!mpd.physicalBounds.empty()))||
       (this->requiresCheckBoundsFunction())){
      this->srcFile << "int "
		    << this->getCheckBoundsFunctionName(mpd.material,mpd.className);
      this->srcFile << "(";
      this->writeParameterList(this->srcFile,mpd.inputs);
      this->srcFile << ")\n{\n";
      this->writeInterfaceSpecificVariables(mpd.inputs);
      for(const auto& i : mpd.inputs){
	this->srcFile << "static_cast<void>(" << i.name << ");\n";
      }
      if(!mpd.physicalBounds.empty()){
	this->srcFile << "/* treating mpd.physical bounds */\n";
	writeBounds(this->srcFile,mpd.physicalBounds,"-");
      }
      if(!mpd.bounds.empty()){
	this->srcFile << "/* treating standard bounds */\n";
	writeBounds(this->srcFile,mpd.physicalBounds,"");
      }
      this->srcFile << "return 0;\n} /* end of " << mpd.className << "_checkBounds */\n\n";
    }
    this->writeEndSrcNamespace();
  } // end of CMaterialPropertyInterfaceBase::writeSrcFile(void)

  void
  CMaterialPropertyInterfaceBase::writeCErrorTreatment(std::ostream& os,
						       const MaterialPropertyDescription& mpd) const{
    os << "return std::nan(\"" << this->getFunctionName(mpd.material,mpd.className)
       << ": invalid call to a C function (errno is not null)\");\n";
  } // CMaterialPropertyInterfaceBase::writeCErrorTreatment
  
} // end of namespace mfront
