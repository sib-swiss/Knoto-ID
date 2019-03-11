#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=-0.45129,-0.642531,0.619267 --closure-method=direct --output=convert_diagram_112.txt "${sourcepath}"/input/input7.xyz || exit $? 
diff convert_diagram_112.txt "${sourcepath}"/output/convert_diagram_112.txt || exit $?
