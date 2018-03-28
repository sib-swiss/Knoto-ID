#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/knotted_core --cyclic-input --closure-method=direct --output=knotted_core_10.txt --output-search=knotted_core_10_search.txt "${sourcepath}"/input/input4.xyz || exit $? 
diff knotted_core_10.txt "${sourcepath}"/output/knotted_core_10.txt || exit $?
diff knotted_core_10_search.txt "${sourcepath}"/output/knotted_core_10_search.txt || exit $?
