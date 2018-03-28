#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=0.924618,0.129208,0.358311 --closure-method=open --output=convert_diagram_40.txt "${sourcepath}"/input/input2.xyz || exit $? 
diff convert_diagram_40.txt "${sourcepath}"/output/convert_diagram_40.txt || exit $?
