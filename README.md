# Discord

The goal of this project is to implement a chat system.

## Travis

[![Build Status](https://travis-ci.com/PamplemousseMR/Discord.svg?branch=master)](https://travis-ci.com/PamplemousseMR/Discord)

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

- [ncurses](https://en.wikipedia.org/wiki/Ncurses) : A library providing an API that allows to write Text-based user interface in a terminal.

### Compilation

Compile using the makefile : `make`.

```
- all : Compile everything.
- customer : Compile customer.
- coordinator : Compile coordinator.
- clean : Clean previous compilation.
```

### Use

Start the coordinator to create the IPC tools : `./coordinator_exe` .
```
-h [options] : Displays help for execution arguments. 
-c [options] <key> : To specify the key of the message queue.
-s [options] <key> : To specify the key of the semaphore array. 
-m [options] <key> : Specify the key for the shared memory segment. 
-d [options] : Cleans the IPC if there is a problem during a previous creation that would not have deleted the IPC tools itself.
```

Start the customers : `./customer_exe`.

```
-h [options] : Displays help for execution arguments. 
-c [options] <key> : To specify the key of the message queue. 
-i [options] <id> : Allows to specify the string identifier of the client.
```

## Authors

* **MANCIAUX Romain** - *Initial work* - [PamplemousseMR](https://github.com/PamplemousseMR).

## License

This project is licensed under the GNU Lesser General Public License v3.0 - see the [LICENSE.md](LICENSE.md) file for details.