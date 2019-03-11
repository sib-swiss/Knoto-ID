#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=-0.620525,-0.222393,-0.751991 --closure-method=direct --output=convert_diagram_35.txt "${sourcepath}"/input/input2.xyz || exit $? 
diff convert_diagram_35.txt "${sourcepath}"/output/convert_diagram_35.txt || exit $?
