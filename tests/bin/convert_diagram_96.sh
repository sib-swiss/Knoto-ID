#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=-0.816423,0.099924,-0.568743 --closure-method=direct --output=convert_diagram_96.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff convert_diagram_96.txt "${sourcepath}"/output/convert_diagram_96.txt || exit $?
