#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=-0.0330865,-0.676602,0.735605 --closure-method=open --output=convert_diagram_25.txt "${sourcepath}"/input/input1.xyz || exit $? 
diff convert_diagram_25.txt "${sourcepath}"/output/convert_diagram_25.txt || exit $?
