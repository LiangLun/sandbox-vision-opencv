#!/bin/sh

OUTPUT=7376_02.zip

# Remove any previous archive of the project.
rm -f "$OUTPUT"

# Blank out the Primary Account Key of the Microsoft Account.
sed -i '.bak' -e "s/bingKey = '.*'/bingKey = 'xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx'/g" ImageSearchSession.py

# Archive the project, excluding hidden and generated files.
zip -r --exclude='*/.*' "$OUTPUT" *.py *.sh build.bat cacert.pem Luxocator.spec images mac win

# Restore the Primary Account Key of the Microsoft Account.
mv -f ImageSearchSession.py.bak ImageSearchSession.py