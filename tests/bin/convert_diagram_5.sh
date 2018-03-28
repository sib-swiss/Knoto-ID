#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=0.77613,-0.524826,0.349542 --closure-method=direct --output=convert_diagram_5.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff convert_diagram_5.txt "${sourcepath}"/output/convert_diagram_5.txt || exit $?
