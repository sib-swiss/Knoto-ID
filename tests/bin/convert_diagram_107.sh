#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=-0.156527,-0.947282,0.279565 --closure-method=open --output=convert_diagram_107.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff convert_diagram_107.txt "${sourcepath}"/output/convert_diagram_107.txt || exit $?
