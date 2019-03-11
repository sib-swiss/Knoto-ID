#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --arrow-polynomial --names-db="${sourcepath}"/input/input_knotoid_names.txt --projections-list="${sourcepath}"/input/input_projections.txt --closure-method=open --output=knotted_core_181.txt --output-search=knotted_core_181_search.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff knotted_core_181.txt "${sourcepath}"/output/knotted_core_181.txt || exit $?
diff knotted_core_181_search.txt "${sourcepath}"/output/knotted_core_181_search.txt || exit $?
