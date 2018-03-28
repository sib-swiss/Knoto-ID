#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/knotted_core --cyclic-input --closure-method=direct --output=knotted_core_14.txt --output-search=knotted_core_14_search.txt "${sourcepath}"/input/input5.xyz || exit $? 
diff knotted_core_14.txt "${sourcepath}"/output/knotted_core_14.txt || exit $?
diff knotted_core_14_search.txt "${sourcepath}"/output/knotted_core_14_search.txt || exit $?
