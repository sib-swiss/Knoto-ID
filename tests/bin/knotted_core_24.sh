#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --closure-method=direct --output=knotted_core_24.txt --output-search=knotted_core_24_search.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff knotted_core_24.txt "${sourcepath}"/output/knotted_core_24.txt || exit $?
diff knotted_core_24_search.txt "${sourcepath}"/output/knotted_core_24_search.txt || exit $?
