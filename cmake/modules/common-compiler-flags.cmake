# This files gather compiler flags that are common to g++, clang and
# pathCC

tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wall" "W" "Wextra" "pedantic" "Wshadow")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wpointer-arith" "Wcast-qual" "Wcast-align")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wwrite-strings" "Wctor-dtor-privacy" "Wnon-virtual-dtor")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Woverloaded-virtual" "Wreturn-type" "Wfloat-equal")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wno-endif-labels" "Wsign-compare" "Wmissing-format-attribute")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wno-multichar" "Wno-deprecated-declarations" "Wpacked")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wredundant-decls")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wunknown-pragmas" "Wundef" "Wreorder")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wconversion")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wdouble-promotion")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Winit-self")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wlogical-op")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wmissing-declarations")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wmissing-include-dirs")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wnoexcept")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wold-style-cast")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wsign-conversion")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wsign-promo")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wstrict-null-sentinel")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wtrampolines")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wvector-operation-performance")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wzero-as-null-pointer-constant")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wignored-qualifiers")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wsequence-point")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wswitch")
# tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wsuggest-attribute=pure")
# tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wsuggest-attribute=const")
# tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wsuggest-attribute=noreturn")
# tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wsuggest-attribute=format")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wsuggest-final-types")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wsuggest-final-methods")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wsuggest-override")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wswitch-bool")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wlogical-not-parentheses")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wsizeof-array-argument")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wbool-compare")
# tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Winline")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wshift-negative-value warns")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wshift-overflow warns")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wtautological-compare")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wnull-dereference")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wduplicated-cond warns")
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wmisleading-indentation")

if(NOT (WIN32 AND ( CMAKE_SIZEOF_VOID_P EQUAL 8 )))
tfel_enable_cxx_compiler_flag(COMPILER_WARNINGS "Wlong-long")
endif(NOT (WIN32 AND ( CMAKE_SIZEOF_VOID_P EQUAL 8 )))
