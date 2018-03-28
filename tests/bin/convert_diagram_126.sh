#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=-0.00533884,-0.754729,-0.656015 --closure-method=open --output=convert_diagram_126.txt "${sourcepath}"/input/input7.xyz || exit $? 
diff convert_diagram_126.txt "${sourcepath}"/output/convert_diagram_126.txt || exit $?
