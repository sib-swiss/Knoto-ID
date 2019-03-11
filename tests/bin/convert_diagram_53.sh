#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=-0.407853,-0.663498,0.627237 --closure-method=direct --output=convert_diagram_53.txt "${sourcepath}"/input/input3.xyz || exit $? 
diff convert_diagram_53.txt "${sourcepath}"/output/convert_diagram_53.txt || exit $?
