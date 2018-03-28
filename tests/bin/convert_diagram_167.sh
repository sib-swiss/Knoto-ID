#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=pd --output-format=gauss --output=convert_diagram_167.txt "${sourcepath}"/input/input_diagram_open_sphere_1.txt || exit $? 
diff convert_diagram_167.txt "${sourcepath}"/output/convert_diagram_167.txt || exit $?
