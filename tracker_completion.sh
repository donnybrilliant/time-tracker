# tracker_completion.sh
# Bash completion for the tracker CLI

_tracker_completions() {
    local cur prev words cword
    _init_completion || return

    local subcommands="project task report running export"
    local project_cmds="add list edit wage delete"
    local task_cmds="add start stop edit delete list"

    # Complete the first argument (subcommand)
    if [[ $cword -eq 1 ]]; then
        COMPREPLY=($(compgen -W "$subcommands" -- "$cur"))
        return 0
    fi

    # Complete project subcommands
    if [[ ${words[1]} == "project" ]]; then
        if [[ $cword -eq 2 ]]; then
            COMPREPLY=($(compgen -W "$project_cmds" -- "$cur"))
            return 0
        fi
        # Complete project names/IDs for relevant commands
        if [[ $cword -ge 3 && (${words[2]} == "edit" || ${words[2]} == "wage" || ${words[2]} == "delete") ]]; then
            # Try to get project names from the database
            local projects=$(sqlite3 ~/.tracker/tracker.db "SELECT name FROM projects;")
            COMPREPLY=($(compgen -W "$projects" -- "$cur"))
            return 0
        fi
    fi

    # Complete task subcommands
    if [[ ${words[1]} == "task" ]]; then
        if [[ $cword -eq 2 ]]; then
            COMPREPLY=($(compgen -W "$task_cmds" -- "$cur"))
            return 0
        fi
        # Complete project names/IDs for add/start/list
        if [[ $cword -eq 3 && (${words[2]} == "add" || ${words[2]} == "start" || ${words[2]} == "list") ]]; then
            local projects=$(sqlite3 ~/.tracker/tracker.db "SELECT name FROM projects;")
            COMPREPLY=($(compgen -W "$projects" -- "$cur"))
            return 0
        fi
        # Complete task names/IDs for stop/edit/delete
        if [[ $cword -eq 3 && (${words[2]} == "stop" || ${words[2]} == "edit" || ${words[2]} == "delete") ]]; then
            local tasks=$(sqlite3 ~/.tracker/tracker.db "SELECT name FROM tasks WHERE is_running=1;")
            COMPREPLY=($(compgen -W "$tasks" -- "$cur"))
            return 0
        fi
    fi

    # Complete project for report/export
    if [[ ${words[1]} == "report" || ${words[1]} == "export" ]]; then
        if [[ $cword -eq 2 ]]; then
            local projects=$(sqlite3 ~/.tracker/tracker.db "SELECT name FROM projects;")
            COMPREPLY=($(compgen -W "$projects" -- "$cur"))
            return 0
        fi
    fi
}

complete -F _tracker_completions tracker
