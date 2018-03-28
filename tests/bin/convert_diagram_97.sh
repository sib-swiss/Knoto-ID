#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=0.396532,0.404556,-0.824073 --closure-method=direct --output=convert_diagram_97.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff convert_diagram_97.txt "${sourcepath}"/output/convert_diagram_97.txt || exit $?
