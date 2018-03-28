#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=0.474646,-0.0177513,0.879998 --closure-method=direct --output=convert_diagram_53.txt "${sourcepath}"/input/input3.xyz || exit $? 
diff convert_diagram_53.txt "${sourcepath}"/output/convert_diagram_53.txt || exit $?
