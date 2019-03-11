#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --names-db=internal --cyclic-input --closure-method=direct --output=knotted_core_11.txt --output-search=knotted_core_11_search.txt "${sourcepath}"/input/input1.xyz || exit $? 
diff knotted_core_11.txt "${sourcepath}"/output/knotted_core_11.txt || exit $?
diff knotted_core_11_search.txt "${sourcepath}"/output/knotted_core_11_search.txt || exit $?
