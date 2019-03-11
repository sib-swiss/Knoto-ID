#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --names-db=internal --projections-list="${sourcepath}"/input/input_projections.txt --closure-method=open --output=knotted_core_146.txt --output-search=knotted_core_146_search.txt "${sourcepath}"/input/input5.xyz || exit $? 
diff knotted_core_146.txt "${sourcepath}"/output/knotted_core_146.txt || exit $?
diff knotted_core_146_search.txt "${sourcepath}"/output/knotted_core_146_search.txt || exit $?
