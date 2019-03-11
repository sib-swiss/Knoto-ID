#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=0.796929,-0.599637,-0.0730747 --closure-method=direct --output=convert_diagram_36.txt "${sourcepath}"/input/input2.xyz || exit $? 
diff convert_diagram_36.txt "${sourcepath}"/output/convert_diagram_36.txt || exit $?
