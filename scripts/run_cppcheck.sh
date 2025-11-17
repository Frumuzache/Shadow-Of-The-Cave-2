#!/usr/bin/bash

echo "Running cppcheck quietly and capturing error output..."

ENABLED_CHECKS="warning,style,performance,portability"

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
    2> cppcheck_errors.log

EXIT_CODE=$?

echo "--- Cppcheck Error Log ---"
cat cppcheck_errors.log
echo "--------------------------"

exit $EXIT_CODE