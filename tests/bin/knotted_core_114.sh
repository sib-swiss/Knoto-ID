#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --planar --arrow-polynomial --projections-list="${sourcepath}"/input/input_projections.txt --closure-method=open --output=knotted_core_114.txt --output-search=knotted_core_114_search.txt "${sourcepath}"/input/input1.xyz || exit $? 
diff knotted_core_114.txt "${sourcepath}"/output/knotted_core_114.txt || exit $?
diff knotted_core_114_search.txt "${sourcepath}"/output/knotted_core_114_search.txt || exit $?
