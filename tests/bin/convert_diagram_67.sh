#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=0.0773987,-0.524768,-0.84772 --closure-method=direct --output=convert_diagram_67.txt "${sourcepath}"/input/input4.xyz || exit $? 
diff convert_diagram_67.txt "${sourcepath}"/output/convert_diagram_67.txt || exit $?
