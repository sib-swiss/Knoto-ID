#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=0.424785,-0.21144,0.880256 --closure-method=direct --output=convert_diagram_99.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff convert_diagram_99.txt "${sourcepath}"/output/convert_diagram_99.txt || exit $?
