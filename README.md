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

## Usage

Add a project with wage:

```sh
tracker add-project "My Project" 75.0
```

Set wage:

```sh
tracker set-wage 1 100.0
```

Add/start/stop tasks:

```sh
tracker add-task 1 "Write docs"
tracker start-task 1 "Develop feature"
tracker stop-task 1 2
```

Show running tasks:

```sh
tracker running-tasks
```

Project report:

```sh
tracker report 1
```

Export CSV:

```sh
tracker export-csv 1 mytasks.csv
```
