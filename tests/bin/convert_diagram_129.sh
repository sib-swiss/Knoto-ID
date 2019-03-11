#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=0.228653,0.139439,-0.96347 --closure-method=direct --output=convert_diagram_129.txt "${sourcepath}"/input/input8.xyz || exit $? 
diff convert_diagram_129.txt "${sourcepath}"/output/convert_diagram_129.txt || exit $?
