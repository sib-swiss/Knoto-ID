#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/knotted_core --cyclic-input --closure-method=direct --output=knotted_core_6.txt --output-search=knotted_core_6_search.txt "${sourcepath}"/input/input1.xyz || exit $? 
diff knotted_core_6.txt "${sourcepath}"/output/knotted_core_6.txt || exit $?
diff knotted_core_6_search.txt "${sourcepath}"/output/knotted_core_6_search.txt || exit $?
