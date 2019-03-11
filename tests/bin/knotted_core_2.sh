#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --names-db=internal --closure-method=direct --output=knotted_core_2.txt --output-search=knotted_core_2_search.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff knotted_core_2.txt "${sourcepath}"/output/knotted_core_2.txt || exit $?
diff knotted_core_2_search.txt "${sourcepath}"/output/knotted_core_2_search.txt || exit $?
