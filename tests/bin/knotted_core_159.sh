#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --arrow-polynomial --cyclic-input --projections-list="${sourcepath}"/input/input_projections.txt --closure-method=open --output=knotted_core_159.txt --output-search=knotted_core_159_search.txt "${sourcepath}"/input/input5.xyz || exit $? 
diff knotted_core_159.txt "${sourcepath}"/output/knotted_core_159.txt || exit $?
diff knotted_core_159_search.txt "${sourcepath}"/output/knotted_core_159_search.txt || exit $?
