#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/knotted_core --projections-list="${sourcepath}"/input/input_projections.txt --closure-method=open --output=knotted_core_48.txt --output-search=knotted_core_48_search.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff knotted_core_48.txt "${sourcepath}"/output/knotted_core_48.txt || exit $?
diff knotted_core_48_search.txt "${sourcepath}"/output/knotted_core_48_search.txt || exit $?
