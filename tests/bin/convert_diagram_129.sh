#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=0.919558,0.000765341,0.392954 --closure-method=direct --output=convert_diagram_129.txt "${sourcepath}"/input/input8.xyz || exit $? 
diff convert_diagram_129.txt "${sourcepath}"/output/convert_diagram_129.txt || exit $?
