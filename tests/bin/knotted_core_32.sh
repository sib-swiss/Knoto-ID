#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --names-db=internal --closure-method=direct --output=knotted_core_32.txt --output-search=knotted_core_32_search.txt "${sourcepath}"/input/input7.xyz || exit $? 
diff knotted_core_32.txt "${sourcepath}"/output/knotted_core_32.txt || exit $?
diff knotted_core_32_search.txt "${sourcepath}"/output/knotted_core_32_search.txt || exit $?
