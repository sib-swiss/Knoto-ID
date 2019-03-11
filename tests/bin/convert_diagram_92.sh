#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=0.821034,-0.155823,0.549202 --closure-method=open --output=convert_diagram_92.txt "${sourcepath}"/input/input5.xyz || exit $? 
diff convert_diagram_92.txt "${sourcepath}"/output/convert_diagram_92.txt || exit $?
