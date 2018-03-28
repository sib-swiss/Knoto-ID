#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/knotted_core --cyclic-input --closure-method=direct --output=knotted_core_22.txt --output-search=knotted_core_22_search.txt "${sourcepath}"/input/input7.xyz || exit $? 
diff knotted_core_22.txt "${sourcepath}"/output/knotted_core_22.txt || exit $?
diff knotted_core_22_search.txt "${sourcepath}"/output/knotted_core_22_search.txt || exit $?
