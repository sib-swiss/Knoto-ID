#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/knotted_core --closure-method=direct --output=knotted_core_16.txt --output-search=knotted_core_16_search.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff knotted_core_16.txt "${sourcepath}"/output/knotted_core_16.txt || exit $?
diff knotted_core_16_search.txt "${sourcepath}"/output/knotted_core_16_search.txt || exit $?
