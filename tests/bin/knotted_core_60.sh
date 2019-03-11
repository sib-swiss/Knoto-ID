#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --projections-list="${sourcepath}"/input/input_projections.txt --closure-method=rays --output=knotted_core_60.txt --output-search=knotted_core_60_search.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff knotted_core_60.txt "${sourcepath}"/output/knotted_core_60.txt || exit $?
diff knotted_core_60_search.txt "${sourcepath}"/output/knotted_core_60_search.txt || exit $?
