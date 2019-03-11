#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --names-db=internal --cyclic-input --closure-method=direct --output=knotted_core_29.txt --output-search=knotted_core_29_search.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff knotted_core_29.txt "${sourcepath}"/output/knotted_core_29.txt || exit $?
diff knotted_core_29_search.txt "${sourcepath}"/output/knotted_core_29_search.txt || exit $?
