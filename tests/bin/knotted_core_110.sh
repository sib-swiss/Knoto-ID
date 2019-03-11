#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --arrow-polynomial --names-db=internal --projections-list="${sourcepath}"/input/input_projections.txt --closure-method=open --output=knotted_core_110.txt --output-search=knotted_core_110_search.txt "${sourcepath}"/input/input1.xyz || exit $? 
diff knotted_core_110.txt "${sourcepath}"/output/knotted_core_110.txt || exit $?
diff knotted_core_110_search.txt "${sourcepath}"/output/knotted_core_110_search.txt || exit $?
