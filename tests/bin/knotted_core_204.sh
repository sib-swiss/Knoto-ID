#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --arrow-polynomial --projections-list="${sourcepath}"/input/input_projections.txt --closure-method=open --output=knotted_core_204.txt --output-search=knotted_core_204_search.txt "${sourcepath}"/input/input7.xyz || exit $? 
diff knotted_core_204.txt "${sourcepath}"/output/knotted_core_204.txt || exit $?
diff knotted_core_204_search.txt "${sourcepath}"/output/knotted_core_204_search.txt || exit $?
