#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/knotted_core --projections-list="${sourcepath}"/input/input_projections.txt --closure-method=rays --output=knotted_core_40.txt --output-search=knotted_core_40_search.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff knotted_core_40.txt "${sourcepath}"/output/knotted_core_40.txt || exit $?
diff knotted_core_40_search.txt "${sourcepath}"/output/knotted_core_40_search.txt || exit $?
