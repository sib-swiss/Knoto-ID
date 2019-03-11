#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=0.906256,0.268335,-0.326646 --closure-method=direct --output=convert_diagram_0.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff convert_diagram_0.txt "${sourcepath}"/output/convert_diagram_0.txt || exit $?
