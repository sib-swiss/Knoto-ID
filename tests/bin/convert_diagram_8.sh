#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=0.0880198,0.115469,0.989404 --closure-method=open --output=convert_diagram_8.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff convert_diagram_8.txt "${sourcepath}"/output/convert_diagram_8.txt || exit $?
