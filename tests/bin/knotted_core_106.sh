#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --planar --names-db="${sourcepath}"/input/input_knotoid_names.txt --cyclic-input --projections-list="${sourcepath}"/input/input_projections.txt --closure-method=open --output=knotted_core_106.txt --output-search=knotted_core_106_search.txt "${sourcepath}"/input/input1.xyz || exit $? 
diff knotted_core_106.txt "${sourcepath}"/output/knotted_core_106.txt || exit $?
diff knotted_core_106_search.txt "${sourcepath}"/output/knotted_core_106_search.txt || exit $?
