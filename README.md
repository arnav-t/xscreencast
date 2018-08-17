# xscreencast
## Screencasting tool for X Window System    
### Installation     
```sh     
make    
sudo make install     
```     
### Usage    
#### Stream on [port]   
```sh
xscreencast [port]     
xscreencast 8080     
```                  
#### Verbose output    
```sh      
xscreencast -v 8080     
```     
#### Maximum framerate (frames might be skipped on slower connections)            
```sh
xscreencast -m 8080       
```