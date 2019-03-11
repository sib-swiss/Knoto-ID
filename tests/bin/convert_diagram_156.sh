#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=-0.319744,-0.165946,-0.932859 --closure-method=open --output=convert_diagram_156.txt "${sourcepath}"/input/input9.xyz || exit $? 
diff convert_diagram_156.txt "${sourcepath}"/output/convert_diagram_156.txt || exit $?
