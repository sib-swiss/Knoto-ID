#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=0.0939056,-0.918628,0.383803 --closure-method=direct --output=convert_diagram_131.txt "${sourcepath}"/input/input8.xyz || exit $? 
diff convert_diagram_131.txt "${sourcepath}"/output/convert_diagram_131.txt || exit $?
