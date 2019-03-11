#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --names-db=internal --closure-method=direct --output=knotted_core_20.txt --output-search=knotted_core_20_search.txt "${sourcepath}"/input/input5.xyz || exit $? 
diff knotted_core_20.txt "${sourcepath}"/output/knotted_core_20.txt || exit $?
diff knotted_core_20_search.txt "${sourcepath}"/output/knotted_core_20_search.txt || exit $?
