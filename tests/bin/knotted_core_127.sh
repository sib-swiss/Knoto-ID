#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --planar --names-db="${sourcepath}"/input/input_knotoid_names.txt --projections-list="${sourcepath}"/input/input_projections.txt --closure-method=open --output=knotted_core_127.txt --output-search=knotted_core_127_search.txt "${sourcepath}"/input/input4.xyz || exit $? 
diff knotted_core_127.txt "${sourcepath}"/output/knotted_core_127.txt || exit $?
diff knotted_core_127_search.txt "${sourcepath}"/output/knotted_core_127_search.txt || exit $?
