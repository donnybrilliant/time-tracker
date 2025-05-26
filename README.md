# CLI Time Tracker

## Features

- Track time spent on tasks grouped by projects
- Set hourly wage per project
- Generate cost/time reports
- Export tasks to CSV
- View all running tasks

## Installation

```sh
make
sudo make install
```

## Shell Auto-completion

This project includes a shell auto-completion script for Bash and Zsh. It enables `<TAB>` completion for subcommands, project names, and task names (with partial name matching).

**Quick setup (run this as a single command):**

```sh
make install-completion
```

- This will copy the completion script to your home directory and add the source line to your `~/.bashrc` or `~/.zshrc` (depending on your shell).
- After running, restart your shell or run `source ~/.bashrc` or `source ~/.zshrc` to enable completion immediately.

**Manual setup (alternative):**

```sh
cp tracker_completion.sh ~/.tracker_completion.sh && echo 'source ~/.tracker_completion.sh' >> ~/.bashrc && source ~/.bashrc
```

- For Zsh, replace `~/.bashrc` with `~/.zshrc` in the command above.

Now, you can type `tracker project delete "My` and press `<TAB>` to auto-complete `"My Project"`.

## Usage

**You can reference projects and tasks by either their numeric ID or their name in quotes.**

Add a project with wage:

```sh
tracker project add "My Project" 75.0
```

Set or edit wage:

```sh
tracker project wage 1 100.0
tracker project wage "My Project" 100.0
```

Add/start/stop tasks:

```sh
tracker task add 1 "Write docs"
tracker task add "My Project" "Write docs"
tracker task start 1 "Develop feature"
tracker task start "My Project" "Develop feature"
tracker task stop 2
tracker task stop "Develop feature"
```

Show running tasks:

```sh
tracker running
```

Project report:

```sh
tracker report 1
tracker report "My Project"
```

Export CSV:

```sh
tracker export 1 mytasks.csv
tracker export "My Project" mytasks.csv
```
