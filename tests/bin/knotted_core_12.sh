#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/knotted_core --closure-method=direct --output=knotted_core_12.txt --output-search=knotted_core_12_search.txt "${sourcepath}"/input/input5.xyz || exit $? 
diff knotted_core_12.txt "${sourcepath}"/output/knotted_core_12.txt || exit $?
diff knotted_core_12_search.txt "${sourcepath}"/output/knotted_core_12_search.txt || exit $?
