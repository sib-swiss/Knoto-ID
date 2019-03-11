#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=-0.596545,0.613145,-0.517867 --closure-method=direct --output=convert_diagram_55.txt "${sourcepath}"/input/input3.xyz || exit $? 
diff convert_diagram_55.txt "${sourcepath}"/output/convert_diagram_55.txt || exit $?
