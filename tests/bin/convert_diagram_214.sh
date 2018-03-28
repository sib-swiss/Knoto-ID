#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=gauss --planar --output-format=gauss --output=convert_diagram_214.txt "${sourcepath}"/input/input_diagram_gauss_open_plane_1.txt || exit $? 
diff convert_diagram_214.txt "${sourcepath}"/output/convert_diagram_214.txt || exit $?
