#!/usr/bin/bash

echo "Running cppcheck quietly and capturing ALL output streams..."

ENABLED_CHECKS="warning,style,performance,portability"

# This time, we capture stdout (1>) and stderr (2>) to separate files
cppcheck --enable="${ENABLED_CHECKS}" \
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
    1> cppcheck_stdout.log \
    2> cppcheck_stderr.log

EXIT_CODE=$?

# Print the contents of the stdout log
echo "--- Cppcheck STDOUT Log (stdout.log) ---"
cat cppcheck_stdout.log
echo "----------------------------------------"

# Print the contents of the stderr log
echo "--- Cppcheck STDERR Log (stderr.log) ---"
cat cppcheck_stderr.log
echo "----------------------------------------"

# Fail the build if cppcheck failed
exit $EXIT_CODE