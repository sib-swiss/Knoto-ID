#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/knotted_core --cyclic-input --projections-list="${sourcepath}"/input/input_projections.txt --closure-method=open --output=knotted_core_90.txt --output-search=knotted_core_90_search.txt "${sourcepath}"/input/input7.xyz || exit $? 
diff knotted_core_90.txt "${sourcepath}"/output/knotted_core_90.txt || exit $?
diff knotted_core_90_search.txt "${sourcepath}"/output/knotted_core_90_search.txt || exit $?
