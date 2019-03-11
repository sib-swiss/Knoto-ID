#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --arrow-polynomial --projections-list="${sourcepath}"/input/input_projections.txt --closure-method=open --output=knotted_core_180.txt --output-search=knotted_core_180_search.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff knotted_core_180.txt "${sourcepath}"/output/knotted_core_180.txt || exit $?
diff knotted_core_180_search.txt "${sourcepath}"/output/knotted_core_180_search.txt || exit $?
