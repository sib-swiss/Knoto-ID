#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=-0.229371,0.531488,0.81542 --closure-method=direct --output=convert_diagram_115.txt "${sourcepath}"/input/input7.xyz || exit $? 
diff convert_diagram_115.txt "${sourcepath}"/output/convert_diagram_115.txt || exit $?
