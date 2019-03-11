#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=-0.570331,0.129549,-0.811135 --closure-method=open --output=convert_diagram_60.txt "${sourcepath}"/input/input3.xyz || exit $? 
diff convert_diagram_60.txt "${sourcepath}"/output/convert_diagram_60.txt || exit $?
