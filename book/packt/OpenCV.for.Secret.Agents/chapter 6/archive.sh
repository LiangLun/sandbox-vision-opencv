#!/bin/sh

OUTPUT=7376_06.zip

# Remove any previous archive of the project.
rm -f "$OUTPUT"

# Archive the project, excluding hidden and generated files.
zip -r --exclude='*/.*' "$OUTPUT" *.py *.sh