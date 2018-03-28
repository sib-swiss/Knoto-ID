#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=pd --output-format=gauss --output=convert_diagram_171.txt "${sourcepath}"/input/input_diagram_open_sphere_2.txt || exit $? 
diff convert_diagram_171.txt "${sourcepath}"/output/convert_diagram_171.txt || exit $?
