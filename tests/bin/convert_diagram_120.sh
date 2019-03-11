#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=-0.979769,0.197404,-0.0329195 --closure-method=open --output=convert_diagram_120.txt "${sourcepath}"/input/input7.xyz || exit $? 
diff convert_diagram_120.txt "${sourcepath}"/output/convert_diagram_120.txt || exit $?
