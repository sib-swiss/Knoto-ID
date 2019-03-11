#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --names-db=internal --cyclic-input --projections-list="${sourcepath}"/input/input_projections.txt --closure-method=rays --output=knotted_core_41.txt --output-search=knotted_core_41_search.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff knotted_core_41.txt "${sourcepath}"/output/knotted_core_41.txt || exit $?
diff knotted_core_41_search.txt "${sourcepath}"/output/knotted_core_41_search.txt || exit $?
