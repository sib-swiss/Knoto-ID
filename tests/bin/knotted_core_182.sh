#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --arrow-polynomial --names-db=internal --projections-list="${sourcepath}"/input/input_projections.txt --closure-method=open --output=knotted_core_182.txt --output-search=knotted_core_182_search.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff knotted_core_182.txt "${sourcepath}"/output/knotted_core_182.txt || exit $?
diff knotted_core_182_search.txt "${sourcepath}"/output/knotted_core_182_search.txt || exit $?
