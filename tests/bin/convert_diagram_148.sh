#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=0.80807,-0.39994,0.432518 --closure-method=direct --output=convert_diagram_148.txt "${sourcepath}"/input/input9.xyz || exit $? 
diff convert_diagram_148.txt "${sourcepath}"/output/convert_diagram_148.txt || exit $?
