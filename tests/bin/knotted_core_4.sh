#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/knotted_core --closure-method=direct --output=knotted_core_4.txt --output-search=knotted_core_4_search.txt "${sourcepath}"/input/input1.xyz || exit $? 
diff knotted_core_4.txt "${sourcepath}"/output/knotted_core_4.txt || exit $?
diff knotted_core_4_search.txt "${sourcepath}"/output/knotted_core_4_search.txt || exit $?
