#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=0.812788,0.459141,-0.358561 --closure-method=open --output=convert_diagram_8.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff convert_diagram_8.txt "${sourcepath}"/output/convert_diagram_8.txt || exit $?
