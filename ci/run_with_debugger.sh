#!/bin/bash -e
SCRIPT_PATH="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

if [ $# -eq 0 ]; then
    echo "Usage: $0 program [args...]"
    exit 1
fi

PROGRAM=$1
shift

# Create a temporary LLDB script file
TMP_LLDB_SCRIPT=$(mktemp)

cat > "$TMP_LLDB_SCRIPT" << EOF
settings set auto-confirm true

# Pass any arguments to the process
process launch -- $@

# When it stops (likely due to a crash), print backtrace
bt all
frame variable
quit
EOF

# Run LLDB with the program
lldb -s "$TMP_LLDB_SCRIPT" "$PROGRAM"

# Clean up
rm "$TMP_LLDB_SCRIPT"
