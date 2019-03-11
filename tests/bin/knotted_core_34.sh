#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --names-db="${sourcepath}"/input/input_knot_names.txt --cyclic-input --closure-method=direct --output=knotted_core_34.txt --output-search=knotted_core_34_search.txt "${sourcepath}"/input/input7.xyz || exit $? 
diff knotted_core_34.txt "${sourcepath}"/output/knotted_core_34.txt || exit $?
diff knotted_core_34_search.txt "${sourcepath}"/output/knotted_core_34_search.txt || exit $?
