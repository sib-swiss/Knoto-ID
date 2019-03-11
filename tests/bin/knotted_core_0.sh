#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --closure-method=direct --output=knotted_core_0.txt --output-search=knotted_core_0_search.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff knotted_core_0.txt "${sourcepath}"/output/knotted_core_0.txt || exit $?
diff knotted_core_0_search.txt "${sourcepath}"/output/knotted_core_0_search.txt || exit $?
