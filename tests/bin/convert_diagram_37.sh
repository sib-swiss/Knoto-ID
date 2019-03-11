#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=0.455188,-0.445404,0.770986 --closure-method=direct --output=convert_diagram_37.txt "${sourcepath}"/input/input2.xyz || exit $? 
diff convert_diagram_37.txt "${sourcepath}"/output/convert_diagram_37.txt || exit $?
