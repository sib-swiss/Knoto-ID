#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=0.496803,-0.122739,0.85914 --closure-method=direct --output=convert_diagram_101.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff convert_diagram_101.txt "${sourcepath}"/output/convert_diagram_101.txt || exit $?
