#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --names-db="${sourcepath}"/input/input_knot_names.txt --cyclic-input --closure-method=direct --output=knotted_core_28.txt --output-search=knotted_core_28_search.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff knotted_core_28.txt "${sourcepath}"/output/knotted_core_28.txt || exit $?
diff knotted_core_28_search.txt "${sourcepath}"/output/knotted_core_28_search.txt || exit $?
