#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/knotted_core --closure-method=direct --output=knotted_core_20.txt --output-search=knotted_core_20_search.txt "${sourcepath}"/input/input7.xyz || exit $? 
diff knotted_core_20.txt "${sourcepath}"/output/knotted_core_20.txt || exit $?
diff knotted_core_20_search.txt "${sourcepath}"/output/knotted_core_20_search.txt || exit $?
