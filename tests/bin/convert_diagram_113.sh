#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=-0.0658948,-0.995209,0.0722291 --closure-method=direct --output=convert_diagram_113.txt "${sourcepath}"/input/input7.xyz || exit $? 
diff convert_diagram_113.txt "${sourcepath}"/output/convert_diagram_113.txt || exit $?
