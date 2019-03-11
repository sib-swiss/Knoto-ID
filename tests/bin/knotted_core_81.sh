#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --planar --cyclic-input --projections-list="${sourcepath}"/input/input_projections.txt --closure-method=open --output=knotted_core_81.txt --output-search=knotted_core_81_search.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff knotted_core_81.txt "${sourcepath}"/output/knotted_core_81.txt || exit $?
diff knotted_core_81_search.txt "${sourcepath}"/output/knotted_core_81_search.txt || exit $?
