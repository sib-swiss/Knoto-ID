#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=pd --output-format=gauss --output=convert_diagram_194.txt "${sourcepath}"/input/input_diagram_closed_2.txt || exit $? 
diff convert_diagram_194.txt "${sourcepath}"/output/convert_diagram_194.txt || exit $?
