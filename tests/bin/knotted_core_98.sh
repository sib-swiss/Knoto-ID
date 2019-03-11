#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --names-db=internal --projections-list="${sourcepath}"/input/input_projections.txt --closure-method=open --output=knotted_core_98.txt --output-search=knotted_core_98_search.txt "${sourcepath}"/input/input1.xyz || exit $? 
diff knotted_core_98.txt "${sourcepath}"/output/knotted_core_98.txt || exit $?
diff knotted_core_98_search.txt "${sourcepath}"/output/knotted_core_98_search.txt || exit $?
