#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --cyclic-input --closure-method=direct --output=knotted_core_27.txt --output-search=knotted_core_27_search.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff knotted_core_27.txt "${sourcepath}"/output/knotted_core_27.txt || exit $?
diff knotted_core_27_search.txt "${sourcepath}"/output/knotted_core_27_search.txt || exit $?
