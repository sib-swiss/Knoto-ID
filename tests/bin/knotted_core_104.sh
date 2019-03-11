#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --planar --names-db=internal --projections-list="${sourcepath}"/input/input_projections.txt --closure-method=open --output=knotted_core_104.txt --output-search=knotted_core_104_search.txt "${sourcepath}"/input/input1.xyz || exit $? 
diff knotted_core_104.txt "${sourcepath}"/output/knotted_core_104.txt || exit $?
diff knotted_core_104_search.txt "${sourcepath}"/output/knotted_core_104_search.txt || exit $?
