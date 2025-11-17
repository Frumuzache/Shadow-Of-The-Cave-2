#!/usr/bin/bash

echo "Running cppcheck with GCC-style error formatting..."

ENABLED_CHECKS="warning,style,performance,portability"

#
# 1. Added --template=gcc to force a standard error format
# 2. Kept --quiet to stop progress messages
# 3. Kept redirection of stderr (2>) to the log file
#
cppcheck --enable="${ENABLED_CHECKS}" \
    --template=gcc \
    --quiet \
    --inline-suppr \
    --project="${BUILD_DIR:-build}"/compile_commands.json \
    -i"${BUILD_DIR:-build}" --suppress="*:${BUILD_DIR:-build}/*" \
    -i"${EXT_DIR:-ext}" --suppress="*:${EXT_DIR:-ext}/*" \
    -i"${GEN_DIR:-generated}" --suppress="*:${GEN_DIR:-generated}/*" \
    --suppress=missingIncludeSystem \
    --suppress=unmatchedSuppression \
    --suppress=useStlAlgorithm \
    --error-exitcode=1 \
    2> cppcheck_errors.log

EXIT_CODE=$?

# Print the error log, which should NOW contain the findings
echo "--- Cppcheck Error Log ---"
cat cppcheck_errors.log
echo "--------------------------"

exit $EXIT_CODE