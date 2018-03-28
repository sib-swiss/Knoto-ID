#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=0.406845,-0.661868,0.62961 --closure-method=direct --output=convert_diagram_32.txt "${sourcepath}"/input/input2.xyz || exit $? 
diff convert_diagram_32.txt "${sourcepath}"/output/convert_diagram_32.txt || exit $?
