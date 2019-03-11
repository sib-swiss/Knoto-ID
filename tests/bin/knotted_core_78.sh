#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --planar --projections-list="${sourcepath}"/input/input_projections.txt --closure-method=open --output=knotted_core_78.txt --output-search=knotted_core_78_search.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff knotted_core_78.txt "${sourcepath}"/output/knotted_core_78.txt || exit $?
diff knotted_core_78_search.txt "${sourcepath}"/output/knotted_core_78_search.txt || exit $?
