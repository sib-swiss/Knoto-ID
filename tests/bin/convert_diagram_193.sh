#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=pd --output-format=pd --output=convert_diagram_193.txt "${sourcepath}"/input/input_diagram_closed_2.txt || exit $? 
diff convert_diagram_193.txt "${sourcepath}"/output/convert_diagram_193.txt || exit $?
