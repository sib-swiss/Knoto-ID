#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --planar --arrow-polynomial --projections-list="${sourcepath}"/input/input_projections.txt --closure-method=open --output=knotted_core_186.txt --output-search=knotted_core_186_search.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff knotted_core_186.txt "${sourcepath}"/output/knotted_core_186.txt || exit $?
diff knotted_core_186_search.txt "${sourcepath}"/output/knotted_core_186_search.txt || exit $?
