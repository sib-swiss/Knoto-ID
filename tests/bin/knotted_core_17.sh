#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --names-db=internal --cyclic-input --closure-method=direct --output=knotted_core_17.txt --output-search=knotted_core_17_search.txt "${sourcepath}"/input/input4.xyz || exit $? 
diff knotted_core_17.txt "${sourcepath}"/output/knotted_core_17.txt || exit $?
diff knotted_core_17_search.txt "${sourcepath}"/output/knotted_core_17_search.txt || exit $?
