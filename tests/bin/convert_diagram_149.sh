#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=-0.330148,0.527837,0.782554 --closure-method=direct --output=convert_diagram_149.txt "${sourcepath}"/input/input9.xyz || exit $? 
diff convert_diagram_149.txt "${sourcepath}"/output/convert_diagram_149.txt || exit $?
