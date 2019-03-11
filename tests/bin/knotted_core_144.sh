#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//knotted_core --projections-list="${sourcepath}"/input/input_projections.txt --closure-method=open --output=knotted_core_144.txt --output-search=knotted_core_144_search.txt "${sourcepath}"/input/input5.xyz || exit $? 
diff knotted_core_144.txt "${sourcepath}"/output/knotted_core_144.txt || exit $?
diff knotted_core_144_search.txt "${sourcepath}"/output/knotted_core_144_search.txt || exit $?
