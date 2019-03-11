#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --cyclic-input --closure-method=direct --output=knotted_core_15.txt --output-search=knotted_core_15_search.txt "${sourcepath}"/input/input4.xyz || exit $? 
diff knotted_core_15.txt "${sourcepath}"/output/knotted_core_15.txt || exit $?
diff knotted_core_15_search.txt "${sourcepath}"/output/knotted_core_15_search.txt || exit $?
