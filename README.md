# xscreencast
![C](https://img.shields.io/badge/Made%20with-C-orange.svg)        
Screencasting tool for X Window System    
## Installation     
```sh     
make    
sudo make install     
```     
## Usage      
```sh
Usage: xscreencast [OPTION]... [PORT]

Options:
	-v		Verbose output
	-m		Maximum framerate
```
### Stream on [port] 
```sh
xscreencast [port]     
```
```sh
xscreencast 8080     
```                  
### Verbose output    
```sh      
xscreencast -v 8080     
```     
### Maximum framerate (frames might be skipped on slower connections)            
```sh
xscreencast -m 8080       
```
## Customization
The stream page can also be customized. The template is stored in `~/.xscreencast/home.html` and can be edited. Note that there must be at least one `img` container pointing to `/scr.jpg` for streaming.       
![screenshot](https://i.imgur.com/tb8nfQQ.jpg "Screenshot")     
## Uninstallation
```sh    
sudo make uninstall
```
