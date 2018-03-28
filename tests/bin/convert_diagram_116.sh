#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=0.317953,0.321239,0.892027 --closure-method=direct --output=convert_diagram_116.txt "${sourcepath}"/input/input7.xyz || exit $? 
diff convert_diagram_116.txt "${sourcepath}"/output/convert_diagram_116.txt || exit $?
