#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --names-db="${sourcepath}"/input/input_knot_names.txt --cyclic-input --projections-list="${sourcepath}"/input/input_projections.txt --closure-method=rays --output=knotted_core_52.txt --output-search=knotted_core_52_search.txt "${sourcepath}"/input/input4.xyz || exit $? 
diff knotted_core_52.txt "${sourcepath}"/output/knotted_core_52.txt || exit $?
diff knotted_core_52_search.txt "${sourcepath}"/output/knotted_core_52_search.txt || exit $?
