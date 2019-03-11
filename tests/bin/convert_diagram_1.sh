#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=0.132454,-0.93665,-0.324257 --closure-method=direct --output=convert_diagram_1.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff convert_diagram_1.txt "${sourcepath}"/output/convert_diagram_1.txt || exit $?
