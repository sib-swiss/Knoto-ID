#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=0.576523,-0.357725,0.734612 --closure-method=open --output=convert_diagram_157.txt "${sourcepath}"/input/input9.xyz || exit $? 
diff convert_diagram_157.txt "${sourcepath}"/output/convert_diagram_157.txt || exit $?
