#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=0.0779337,-0.991982,0.099486 --closure-method=direct --output=convert_diagram_149.txt "${sourcepath}"/input/input9.xyz || exit $? 
diff convert_diagram_149.txt "${sourcepath}"/output/convert_diagram_149.txt || exit $?
