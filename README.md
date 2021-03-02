# Cscroll
A simple text scroller, written in C.

## Installation
  ```bash
  git clone https://github.com/jan146/Cscroll.git
  cd Cscroll
  sudo make
  ```

## Functionality

  ### Set scrolling speed
  Set with `-d [time]` or `--delay [time]` parameter, text will be rotate every [time] seconds. Parameter can be a decimal number, but obviously has to be greater than zero.

  ### Set minimum length
  Set with `-l [len]` or `--length [len]` parameter. If text is not longer than [len], it will be just printed, but not rotate.
  
  ### Can force scrolling
  Set with `-f` or `--force` parameter. If this option is set, text will rotate even if it is shorter than length parameter.
  
  ### Text can include commands
  Set with `-c [command]` or `--command [command]`. In double quotes (""), put your desired command, which you want to be added to the text.
  
  ### Set command update interval
  Set with `-u [interval]` or `--update [interval]`. The commands, passed via `-c` and `--command` will be updated every [interval]\*[time] seconds.
  
  ### Set custom separator
  Set with `-s [separator]` or `--separator [separator]`. If the text should rotate, [separator] will be appended to the text.
  
## Example
  `$ cscroll -d 0.3 -l 10 -u 100 "Today is " -c "date +%A" ". "`
  
## Why
  I needed something similar to <a href="https://github.com/noctuid/zscroll">zscroll</a>, but python is not very efficient and I wanted to customize it a bit for my liking. I might also use this for other projects.
  
## Dependencies
* <a href="https://github.com/git-guides/install-git">git</a> (to clone the repo)
* <a href="https://gcc.gnu.org/releases.html">gcc</a> (C compiler)
