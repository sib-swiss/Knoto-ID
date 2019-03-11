#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --planar --names-db="${sourcepath}"/input/input_knotoid_names.txt --projections-list="${sourcepath}"/input/input_projections.txt --closure-method=open --output=knotted_core_175.txt --output-search=knotted_core_175_search.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff knotted_core_175.txt "${sourcepath}"/output/knotted_core_175.txt || exit $?
diff knotted_core_175_search.txt "${sourcepath}"/output/knotted_core_175_search.txt || exit $?
