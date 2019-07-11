/*!
 * \file   CMakeGenerator.cxx
 * \brief
 * \author Thomas Helfer
 * \date   16/08/2015
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <set>
#include <cstring>
#include <ostream>
#include <sstream>
#include <fstream>
#include <iterator>
#include <algorithm>

#include <sys/types.h>
#include <sys/stat.h>
#if defined _WIN32 || defined _WIN64
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <io.h>
#include <conio.h>
#include <windows.h>
#include <process.h>
#ifdef small
#undef small
#endif /* small */
#else
#include <dlfcn.h>
#include <sys/wait.h>
#include <dirent.h>
#include <unistd.h>
#endif

#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/System/System.hxx"
#include "MFront/MFrontHeader.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/SearchPathsHandler.hxx"
#include "MFront/MFrontLock.hxx"
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/GeneratorOptions.hxx"
#include "MFront/CMakeGenerator.hxx"

namespace mfront {

  static const char* getCMakeCommand() {
    const auto* cmake = ::getenv("CMAKE");
    if (cmake != nullptr) {
      return cmake;
    }
#if defined _WIN32 || defined _WIN64
    return "cmake.exe";
#else
    return "cmake";
#endif
  }

  static std::string getCMakeDefaultGenerator() {
#ifdef TFEL_CMAKE_GENERATOR
    return TFEL_CMAKE_GENERATOR;
#else  /* TFEL_CMAKE_GENERATOR */
    return "\"Unix Makefiles\"";
#endif /* TFEL_CMAKE_GENERATOR */
  }

  void generateCMakeListsFile(const TargetsDescription& t,
                              const GeneratorOptions& o,
                              const std::string&) {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "generateCMakeListFile: " + m);
    };
    if (getVerboseMode() >= VERBOSE_LEVEL2) {
      getLogStream() << "generating 'src/CMakeList.txt'\n";
    }
    MFrontLockGuard lock;
    std::ofstream m("src/CMakeLists.txt");
    m.exceptions(std::ios::badbit | std::ios::failbit);
    throw_if(!m, "can't open file 'src/CMakeList.txt'");
    auto append = [&m](const std::string& n, const std::string& v) {
      if ((tfel::utilities::starts_with(v, "$(shell ")) ||
          (tfel::utilities::ends_with(v, ")"))) {
        m << "append_spawn(" << n << " " << v.substr(8, v.size() - 9) << ")\n";
      } else {
        m << "list(APPEND " << n << " " << v << ")\n";
      }
    };
    // spawn without calling seperate_arguments
    auto append2 = [&m](const std::string& n, const std::string& v) {
      if ((tfel::utilities::starts_with(v, "$(shell ")) ||
          (tfel::utilities::ends_with(v, ")"))) {
        m << "append_spawn2(" << n << " " << v.substr(8, v.size() - 9) << ")\n";
      } else {
        m << "list(APPEND " << n << " " << v << ")\n";
      }
    };
    throw_if(!t.specific_targets.empty(), "specific targets are not supported");
    m << "# CMakeList.txt generated by mfront.\n"
      << MFrontHeader::getHeader("# ") << "\n"
      << "\n"
      << "cmake_minimum_required(VERSION 2.4)\n"
      << "project(\"mfront-sources\")\n";
    if (o.sys != "apple") {
      m << "set(MACOSX_RPATH ON)\n";
      m << "set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)\n";
    }
    m << "function(spawn res cmd)\n"
      << "  execute_process(COMMAND ${cmd} ${ARGN}\n"
      << "    OUTPUT_VARIABLE SPAWN_RESULT\n"
      << "    OUTPUT_STRIP_TRAILING_WHITESPACE)\n"
      << "  separate_arguments(SPAWN_RESULT)  \n"
      << "  set(${res} ${SPAWN_RESULT} PARENT_SCOPE)\n"
      << "endfunction(spawn)\n"
      << "function(append_spawn res cmd)\n"
      << "  spawn(SPAWN_RESULTS ${cmd} ${ARGN})\n"
      << "  list(APPEND SPAWN_RESULTS ${${res}})\n"
      << "  set(${res} ${SPAWN_RESULTS} PARENT_SCOPE)\n"
      << "endfunction(append_spawn)\n"
      << "function(spawn2 res cmd)\n"
      << "  execute_process(COMMAND ${cmd} ${ARGN}\n"
      << "    OUTPUT_VARIABLE SPAWN_RESULT\n"
      << "    OUTPUT_STRIP_TRAILING_WHITESPACE)\n"
      << "  set(${res} ${SPAWN_RESULT} PARENT_SCOPE)\n"
      << "endfunction(spawn2)\n"
      << "function(append_spawn2 res cmd)\n"
      << "  spawn2(SPAWN_RESULTS ${cmd} ${ARGN})\n"
      << "  list(APPEND SPAWN_RESULTS ${${res}})\n"
      << "  set(${res} ${SPAWN_RESULTS} PARENT_SCOPE)\n"
      << "endfunction(append_spawn2)\n"
      << "\n"
      << "if(TFEL_INSTALL_PATH)\n"
      << "  set(TFELHOME \"${TFEL_INSTALL_PATH}\")\n"
      << "else(TFEL_INSTALL_PATH)\n"
      << "  set(TFELHOME $ENV{TFELHOME})\n"
      << "endif(TFEL_INSTALL_PATH)\n"
      << "\n";
#ifdef TFEL_APPEND_SUFFIX
    m << "find_program(TFEL_CONFIG  tfel-config-" TFEL_SUFFIX
         " \"${TFELHOME}/bin\")\n";
#else  /* TFEL_APPEND_SUFFIX */
    m << "find_program(TFEL_CONFIG  tfel-config \"${TFELHOME}/bin\")\n";
#endif /* TFEL_APPEND_SUFFIX */
    m << "message(STATUS \"tfel-config         : ${TFEL_CONFIG}\")\n"
      << "\n";
    // << "spawn(TFEL_INCLUDE_PATH ${TFEL_CONFIG} \"--include-path\")\n"
    // << "spawn(TFEL_LIBRARY_PATH ${TFEL_CONFIG} \"--library-path\")\n"
    // << "spawn(TFEL_COMPILER_FLAGS ${TFEL_CONFIG}\n"
    // << "  \"--cppflags\" \"--compiler-flags\")\n"
    switch (o.olevel) {
      case GeneratorOptions::LEVEL2:
        m << "spawn(TFEL_OFLAGS ${TFEL_CONFIG} \"--oflags\" \"--oflags2\")\n";
        break;
      case GeneratorOptions::LEVEL1:
        m << "spawn(TFEL_OFLAGS ${TFEL_CONFIG} \"--oflags\")\n";
        break;
      case GeneratorOptions::LEVEL0:
        m << "spawn(TFEL_OFLAGS ${TFEL_CONFIG} \"--oflags0\")\n";
        break;
    }
    m << "\n"
      << "message(STATUS \"tfel oflags         : ${TFEL_OFLAGS}\")\n"
      << "\n";
    // include_directories
    const auto include_directories = [&t] {
      auto r = std::vector<std::string>{};
      r.push_back("../include");
      for (const auto& l : t) {
        for (const auto& ld : l.include_directories) {
          if (std::find(r.begin(), r.end(), ld) == r.end()) {
            r.push_back(ld);
          }
        }
      }
      // adding the mfront search path to the include files
      if (!SearchPathsHandler::getSearchPaths().empty()) {
        const auto& paths = SearchPathsHandler::getSearchPaths();
        for (const auto& p : paths) {
          if (std::find(r.begin(), r.end(), p) == r.end()) {
            r.push_back(p);
          }
        }
      }
      return r;
    }();
    if (include_directories.size() == 1) {
      m << "include_directories(" << include_directories[0] << ")\n";
    } else {
      m << "set(INCLUDE_DIRECTORIES)\n";
      for (const auto& d : include_directories) {
        append2("INCLUDE_DIRECTORIES", d);
      }
      m << "include_directories(${INCLUDE_DIRECTORIES})\n";
    }
    // link_directories
    const auto link_directories = [&t] {
      auto r = std::vector<std::string>{};
      for (const auto& l : t) {
        for (const auto& ld : l.link_directories) {
          if (std::find(r.begin(), r.end(), ld) == r.end()) {
            r.push_back(ld);
          }
        }
      }
      return r;
    }();
    if (!link_directories.empty()) {
      m << "set(LINK_DIRECTORIES)\n";
      for (const auto& ld : link_directories) {
        append2("LINK_DIRECTORIES", ld);
      }
      m << "link_directories(${LINK_DIRECTORIES})\n";
    }
    m << "\n";
    for (const auto& l : t) {
      if (l.name == "MFrontMaterialLaw") {
        continue;
      }
      // cppflags
      m << "# Setting compile flags for " << l.name << '\n'
        << "set(" << l.name << "_COMPILE_FLAGS)\n"
        << "list(APPEND " << l.name << "_COMPILE_FLAGS ${TFEL_OFLAGS})\n";
      if ((o.sys == "win32") || (o.sys == "cygwin")) {
        m << "list(APPEND " << l.name
          << "_COMPILE_FLAGS \"-DMFRONT_COMPILING\")\n";
      }
      for (const auto& f : l.cppflags) {
        append(l.name + "_COMPILE_FLAGS", f);
      }
      m << "string (REPLACE \";\" \" \" " << l.name << "_COMPILE_FLAGS\n"
        << "  \"${" << l.name << "_COMPILE_FLAGS}\")\n";
      // link_libraries
      auto has_link_libraries = [&o, &l] {
        if (!l.link_libraries.empty()) {
          return true;
        }
        return (!o.melt) ? !l.deps.empty() : false;
      }();
      if (has_link_libraries) {
        m << "# Setting link libraries for " << l.name << '\n'
          << "set(" << l.name << "_LINK_LIBRARIES)\n";
        for (const auto& lf : l.link_libraries) {
          append(l.name + "_LINK_LIBRARIES", lf);
        }
        if (!o.melt) {
          for (const auto& ld : l.deps) {
            append(l.name + "_LINK_LIBRARIES", ld);
          }
        }
      }
      // ldflags
      m << "# Setting linker flags for " << l.name << '\n';
      if (!l.ldflags.empty()) {
        m << "set(" << l.name << "_LINK_FLAGS )\n";
        for (const auto& lf : l.ldflags) {
          append(l.name + "_LINK_FLAGS", lf);
        }
        m << "string(REPLACE \";\" \" \" " << l.name << "_LINK_FLAGS\n"
          << "  \"${" << l.name << "_LINK_FLAGS}\")\n";
      }
      m << "add_library(" << l.name << " SHARED\n";
      for (const auto& s : l.sources) {
        m << s << " ";
      }
      if (o.melt) {
        for (const auto& ld : l.deps) {
          for (const auto& s : t[ld].sources) {
            m << s << " ";
          }
        }
      }
      m << ")\n";
      if (!l.link_libraries.empty()) {
        m << "target_link_libraries(" << l.name << '\n'
          << "${" << l.name << "_LINK_LIBRARIES})\n";
      }
      m << "set_target_properties(" << l.name << '\n'
        << "PROPERTIES COMPILE_FLAGS \"${" << l.name << "_COMPILE_FLAGS}\")\n";
      if (l.suffix != LibraryDescription::getDefaultLibrarySuffix(
                          t.system, t.libraryType)) {
        m << "set_target_properties(" << l.name << '\n'
          << "PROPERTIES SUFFIX " << l.suffix << ")\n";
      }
      if (!l.ldflags.empty()) {
        m << "set_target_properties(" << l.name << '\n'
          << "PROPERTIES LINK_FLAGS \"${" << l.name << "_LINK_FLAGS}\")\n";
      }
      m << '\n';
    }
  }

  void callCMake(const std::string& t, const std::string& d) {
    using namespace tfel::system;
    using tfel::utilities::starts_with;
    const char* cmake = getCMakeCommand();
    //    const char * silent = getDebugMode() ? nullptr : "-s";
    const char* silent = nullptr;
    const auto g = [] {
      const auto cg = []() -> std::string {
        const auto* e = ::getenv("CMAKE_GENERATOR");
        if (e != nullptr) {
          return e;
        }
        return getCMakeDefaultGenerator();
      }();
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
      // spawn function do not like spaces in arguments
      if (cg.find(' ') != std::string::npos) {
        return '"' + cg + '"';
      }
#endif
      return cg;
    }();
    // check for multi-configuration generator
    const char* cfg1 = nullptr;
    const char* cfg2 = nullptr;
    if ((starts_with(g, "\"Visual Studio")) || (starts_with(g, "XCode"))) {
      cfg1 = "--config";
      cfg2 = "Release";
    }
    const char* tg1 = nullptr;
    const char* tg2 = nullptr;
    if (t != "all") {
      tg1 = "--target";
      tg2 = t.c_str();
    }
    const char* argv[] = {cmake, "-G", g.c_str(), ".", silent, nullptr};
    const char* argv2[] = {cmake, "--build", ".",    tg1,    tg2,
                           cfg1,  cfg2,      silent, nullptr};
    std::remove_if(argv2, argv2 + 9,
                   [](const char* ptr) { return ptr == nullptr; });
    auto error = [&t](const std::string& e, const char* const* args) {
      auto msg = "callCmake: can't build target '" + t + "'\n";
      if (!e.empty()) {
        msg += e + '\n';
      }
      msg += "Command was: ";
      for (const char* const* a = args; *a != nullptr; ++a) {
        msg += *a;
        msg += ' ';
      }
      tfel::raise(msg);
    };
    tfel::raise_if(::strlen(cmake) == 0u, "callCmake: empty cmake command");
    const auto pwd = systemCall::getCurrentWorkingDirectory();
    systemCall::changeCurrentWorkingDirectory(d);
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
    if (_spawnvp(_P_WAIT, cmake, argv) != 0) {
      error("", argv);
    }
    if (_spawnvp(_P_WAIT, cmake, argv2) != 0) {
      error("", argv2);
    }
#else
    auto call_cmake = [&cmake, &error](const char* const* args) {
      const auto child_pid = fork();
      int status = 0;
      if (child_pid != 0) {
        if (wait(&status) == -1) {
          error(
              "something went wrong while "
              "waiting end of cmake process",
              args);
        }
      } else {
        execvp(cmake, const_cast<char* const*>(args));
        ::exit(EXIT_FAILURE);
      }
      return status;
    };
    if (call_cmake(argv) != 0) {
      error("cmake configuration went wrong", argv);
    }
    if (call_cmake(argv2) != 0) {
      error("libraries building went wrong", argv2);
    }
#endif
    systemCall::changeCurrentWorkingDirectory(pwd);
  }

}  // end of namespace mfront
