#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=-0.598351,-0.583931,0.548636 --closure-method=open --output=convert_diagram_28.txt "${sourcepath}"/input/input1.xyz || exit $? 
diff convert_diagram_28.txt "${sourcepath}"/output/convert_diagram_28.txt || exit $?
