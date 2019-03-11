#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=-0.0364937,0.716305,0.696832 --closure-method=open --output=convert_diagram_28.txt "${sourcepath}"/input/input1.xyz || exit $? 
diff convert_diagram_28.txt "${sourcepath}"/output/convert_diagram_28.txt || exit $?
