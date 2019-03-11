#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --arrow-polynomial --names-db=internal --cyclic-input --projections-list="${sourcepath}"/input/input_projections.txt --closure-method=open --output=knotted_core_185.txt --output-search=knotted_core_185_search.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff knotted_core_185.txt "${sourcepath}"/output/knotted_core_185.txt || exit $?
diff knotted_core_185_search.txt "${sourcepath}"/output/knotted_core_185_search.txt || exit $?
