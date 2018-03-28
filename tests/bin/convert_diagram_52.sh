#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=0.407156,0.676913,0.613199 --closure-method=direct --output=convert_diagram_52.txt "${sourcepath}"/input/input3.xyz || exit $? 
diff convert_diagram_52.txt "${sourcepath}"/output/convert_diagram_52.txt || exit $?
