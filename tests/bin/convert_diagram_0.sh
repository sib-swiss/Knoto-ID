#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=-0.469305,0.658514,0.588313 --closure-method=direct --output=convert_diagram_0.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff convert_diagram_0.txt "${sourcepath}"/output/convert_diagram_0.txt || exit $?
