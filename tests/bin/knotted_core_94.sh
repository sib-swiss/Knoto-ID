#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --planar --arrow-polynomial --names-db="${sourcepath}"/input/input_knotoid_names.txt --cyclic-input --projections-list="${sourcepath}"/input/input_projections.txt --closure-method=open --output=knotted_core_94.txt --output-search=knotted_core_94_search.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff knotted_core_94.txt "${sourcepath}"/output/knotted_core_94.txt || exit $?
diff knotted_core_94_search.txt "${sourcepath}"/output/knotted_core_94_search.txt || exit $?
