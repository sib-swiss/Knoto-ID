#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --cyclic-input --closure-method=direct --output=knotted_core_33.txt --output-search=knotted_core_33_search.txt "${sourcepath}"/input/input7.xyz || exit $? 
diff knotted_core_33.txt "${sourcepath}"/output/knotted_core_33.txt || exit $?
diff knotted_core_33_search.txt "${sourcepath}"/output/knotted_core_33_search.txt || exit $?
