#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=0.899361,-0.0729055,-0.431085 --closure-method=direct --output=convert_diagram_33.txt "${sourcepath}"/input/input2.xyz || exit $? 
diff convert_diagram_33.txt "${sourcepath}"/output/convert_diagram_33.txt || exit $?
