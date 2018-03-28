#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/knotted_core --planar --projections-list="${sourcepath}"/input/input_projections.txt --closure-method=open --output=knotted_core_92.txt --output-search=knotted_core_92_search.txt "${sourcepath}"/input/input7.xyz || exit $? 
diff knotted_core_92.txt "${sourcepath}"/output/knotted_core_92.txt || exit $?
diff knotted_core_92_search.txt "${sourcepath}"/output/knotted_core_92_search.txt || exit $?
