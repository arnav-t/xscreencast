# xscreencast
## Screencasting tool for X Window System    
### Usage    
#### Run without any downscaling (slower)    
`xscreencast [port]`     
`xscreencast 8080`          
#### Run with downscaling (faster)    
`xscreencast -d [scale] [port]`     
`xscreencast -d 4 8080`           
#### Verbose output
`xscreencast -v 8080`    
`xscreencast -v -d 2 8080`     
`xscreencast -vd 6 8080`     
Note that the scale must be an integer.    
