#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=-0.0914729,0.194614,-0.976605 --closure-method=open --output=convert_diagram_152.txt "${sourcepath}"/input/input9.xyz || exit $? 
diff convert_diagram_152.txt "${sourcepath}"/output/convert_diagram_152.txt || exit $?
