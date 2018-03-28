#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/knotted_core --projections-list="${sourcepath}"/input/input_projections.txt --closure-method=rays --output=knotted_core_32.txt --output-search=knotted_core_32_search.txt "${sourcepath}"/input/input4.xyz || exit $? 
diff knotted_core_32.txt "${sourcepath}"/output/knotted_core_32.txt || exit $?
diff knotted_core_32_search.txt "${sourcepath}"/output/knotted_core_32_search.txt || exit $?
