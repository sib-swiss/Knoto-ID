#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --closure-method=direct --output=knotted_core_30.txt --output-search=knotted_core_30_search.txt "${sourcepath}"/input/input7.xyz || exit $? 
diff knotted_core_30.txt "${sourcepath}"/output/knotted_core_30.txt || exit $?
diff knotted_core_30_search.txt "${sourcepath}"/output/knotted_core_30_search.txt || exit $?
