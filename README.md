# stopwatch ![icon](stopwatch.png)

![Screenshot](screenshot.png)

Controls:
* Drag to move
* Click to pause/resume
* Double-click to restart
* Right-click to close

Install a flatpak bundle (EASIEST method):
    
    curl -L -o ~/Downloads/stopwatch.flatpak https://github.com/Mark-Weston/stopwatch/releases/download/v1.0/stopwatch.flatpak
    flatpak install --bundle ~/Downloads/stopwatch.flatpak


Install from source:
* Install dependencies (libc, glfw >=3.2, GLESv2) and build dependencies
  
  On Ubuntu 17.04 and up this can be done using
  
  `sudo apt-get install libglfw-dev libgles2-mesa-dev build-essential`
  
  On Ubuntu 16.04 you will have to install GLFW 3.2.1 from source

* Run `sudo make && make install`

Install a flatpak bundle from source:
* Install dependencies
* Run `sudo make && make flatpak && make flatpak-install`
