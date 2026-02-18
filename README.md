## [PureMVC](http://puremvc.github.com/) [C](https://github.com/PureMVC/puremvc-c-multicore-framework) Demo - Todo Command Line Interface Utility

This demo illustrates techniques for performing routine maintenance operations using the PureMVC framework in a C command-line environment. It demonstrates the separation of concerns between business logic, data persistence, and user notification.

## Screenshot
![PureMVC C Demo: Todo Command Line Interface Utility](https://ibb.co/7JkstMZy)

## Features
* **PureMVC Multicore Architecture**: Clean separation of Data (Proxy), UI (Mediator), and Business Logic (Command) to facilitate modularity.
* **MISRA C Compliant**: Adheres to safety-critical coding standards for reliable C development.
* **Interface-Based Storage**: Pluggable storage system (JSON/Flat-file) using Inversion of Control (IoC) and Strategy patterns.
* **Stateless CRUD Logic**: Each operation follows a transactional "Read-Modify-Write" cycle to ensure data integrity.
* **Unified Status Codes**: System-wide `Status` enum for consistent, predictable error propagation across all application layers.
* **Zero Heap Dependency**: Optimized for performance and predictability by minimizing dynamic memory allocation in favor of stack-based data handling.

## Usage

`todo <command> [options]`

```shell
todo list
todo add "Buy Milk"
todo edit 2 -title "Water the plants" --completed
todo edit 2 -t "Water the plants" -c
todo delete 4
todo --version
todo -v
todo --help
todo -h
```


## Platforms / Technologies
* [C11](https://en.wikipedia.org/wiki/C11_(C_standard_revision))
* [CMake](https://cmake.org/) 
* [Make](https://www.gnu.org/software/make/)

## License
* PureMVC Javascript Demo - Employee Admin - Copyright © 2026 [Saad Shams](https://www.linkedin.com/in/muizz)
* PureMVC - Copyright © 2026 Futurescale, Inc.
* All rights reserved.

* Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the name of Futurescale, Inc., PureMVC.org, nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
