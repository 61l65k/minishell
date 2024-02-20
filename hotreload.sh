#!/bin/bash

MINISHELL="./minishell"
REBUILD_SIGNAL_FILE=".minishell_rebuild_signal"

# Function to run minishell
run_minishell() {
    echo "Starting minishell in a new terminal..."
    gnome-terminal -- $MINISHELL &
    MINISHELL_PID=$!
}


# Function to rebuild and restart the program
rebuild_and_restart() {
    echo "Rebuilding the program..."
    make
    if [ $? -ne 0 ]; then
        echo "Build failed, not restarting the program."
        return
    fi
    echo "Rebuild successful. Notifying to restart minishell."
    touch "$REBUILD_SIGNAL_FILE"
}

# Function to monitor file changes and trigger rebuild
monitor_and_rebuild() {
    OS="$(uname)"
    if [ "$OS" = "Darwin" ]; then
        echo "Running on macOS, using fswatch."a
        fswatch -o ./srcs ./includes | while read change; do
            rebuild_and_restart
        done
    elif [ "$OS" = "Linux" ]; then
        echo "Running on Linux, using inotifywait."
        while inotifywait -e modify ./srcs ./includes; do
            rebuild_and_restart
        done
    else
        echo "Unsupported operating system."
        exit 1
    fi
}

## MAIN ##
run_minishell

monitor_and_rebuild &

while true; do
    if [ -f "$REBUILD_SIGNAL_FILE" ]; then
        echo "Restarting minishell due to rebuild..."

        rm "$REBUILD_SIGNAL_FILE"

        if kill -0 $MINISHELL_PID 2>/dev/null; then
            kill $MINISHELL_PID
            wait $MINISHELL_PID 2>/dev/null
        fi

        run_minishell
    fi
    sleep 1
done
## END ##

