#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --cyclic-input --projections-list="${sourcepath}"/input/input_projections.txt --closure-method=rays --output=knotted_core_63.txt --output-search=knotted_core_63_search.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff knotted_core_63.txt "${sourcepath}"/output/knotted_core_63.txt || exit $?
diff knotted_core_63_search.txt "${sourcepath}"/output/knotted_core_63_search.txt || exit $?
