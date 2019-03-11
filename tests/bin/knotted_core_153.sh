#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --planar --cyclic-input --projections-list="${sourcepath}"/input/input_projections.txt --closure-method=open --output=knotted_core_153.txt --output-search=knotted_core_153_search.txt "${sourcepath}"/input/input5.xyz || exit $? 
diff knotted_core_153.txt "${sourcepath}"/output/knotted_core_153.txt || exit $?
diff knotted_core_153_search.txt "${sourcepath}"/output/knotted_core_153_search.txt || exit $?
