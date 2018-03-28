#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/knotted_core --names-db="${sourcepath}"/input/input_knot_names.txt --closure-method=direct --output=knotted_core_13.txt --output-search=knotted_core_13_search.txt "${sourcepath}"/input/input5.xyz || exit $? 
diff knotted_core_13.txt "${sourcepath}"/output/knotted_core_13.txt || exit $?
diff knotted_core_13_search.txt "${sourcepath}"/output/knotted_core_13_search.txt || exit $?
