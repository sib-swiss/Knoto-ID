#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --planar --names-db=internal --cyclic-input --projections-list="${sourcepath}"/input/input_projections.txt --closure-method=open --output=knotted_core_203.txt --output-search=knotted_core_203_search.txt "${sourcepath}"/input/input7.xyz || exit $? 
diff knotted_core_203.txt "${sourcepath}"/output/knotted_core_203.txt || exit $?
diff knotted_core_203_search.txt "${sourcepath}"/output/knotted_core_203_search.txt || exit $?
