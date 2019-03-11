#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --cyclic-input --projections-list="${sourcepath}"/input/input_projections.txt --closure-method=open --output=knotted_core_75.txt --output-search=knotted_core_75_search.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff knotted_core_75.txt "${sourcepath}"/output/knotted_core_75.txt || exit $?
diff knotted_core_75_search.txt "${sourcepath}"/output/knotted_core_75_search.txt || exit $?
