#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=0.431597,-0.833798,-0.344246 --closure-method=open --output=convert_diagram_12.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff convert_diagram_12.txt "${sourcepath}"/output/convert_diagram_12.txt || exit $?
