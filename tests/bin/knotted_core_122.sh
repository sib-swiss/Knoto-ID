#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --names-db=internal --projections-list="${sourcepath}"/input/input_projections.txt --closure-method=open --output=knotted_core_122.txt --output-search=knotted_core_122_search.txt "${sourcepath}"/input/input4.xyz || exit $? 
diff knotted_core_122.txt "${sourcepath}"/output/knotted_core_122.txt || exit $?
diff knotted_core_122_search.txt "${sourcepath}"/output/knotted_core_122_search.txt || exit $?
