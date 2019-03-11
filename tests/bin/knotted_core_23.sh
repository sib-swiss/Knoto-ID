#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --names-db=internal --cyclic-input --closure-method=direct --output=knotted_core_23.txt --output-search=knotted_core_23_search.txt "${sourcepath}"/input/input5.xyz || exit $? 
diff knotted_core_23.txt "${sourcepath}"/output/knotted_core_23.txt || exit $?
diff knotted_core_23_search.txt "${sourcepath}"/output/knotted_core_23_search.txt || exit $?
