#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/knotted_core --planar --projections-list="${sourcepath}"/input/input_projections.txt --closure-method=open --output=knotted_core_84.txt --output-search=knotted_core_84_search.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff knotted_core_84.txt "${sourcepath}"/output/knotted_core_84.txt || exit $?
diff knotted_core_84_search.txt "${sourcepath}"/output/knotted_core_84_search.txt || exit $?
