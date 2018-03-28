#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=-0.99235,-0.0257673,0.120739 --closure-method=direct --output=convert_diagram_148.txt "${sourcepath}"/input/input9.xyz || exit $? 
diff convert_diagram_148.txt "${sourcepath}"/output/convert_diagram_148.txt || exit $?
