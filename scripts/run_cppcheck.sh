#!/usr/bin/bash

# This is the same command as before, but it redirects all
# error output (stderr, which is '2') to a file named 'cppcheck_errors.log'
# The progress (stdout, which is '1') goes to the normal log.
echo "Running cppcheck and capturing error output..."

ENABLED_CHECKS="warning,style,performance,portability"

cppcheck --enable="${ENABLED_CHECKS}" \
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

# Get the exit code from cppcheck
# This is crucial!
EXIT_CODE=$?

# Now, print the contents of the error log.
# This will be *clean* and won't be mixed with progress.
echo "--- Cppcheck Error Log ---"
cat cppcheck_errors.log
echo "--------------------------"

# Finally, exit with the original exit code from cppcheck.
# This ensures the step still fails if errors were found.
exit $EXIT_CODE