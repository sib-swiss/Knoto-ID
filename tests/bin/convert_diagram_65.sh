#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=0.828306,0.52219,-0.203043 --closure-method=direct --output=convert_diagram_65.txt "${sourcepath}"/input/input4.xyz || exit $? 
diff convert_diagram_65.txt "${sourcepath}"/output/convert_diagram_65.txt || exit $?
