# GemParticles

Behold, my ability to screw up code at its finest. It also happens to be an flexible core-profile compliant OpenGL particle engine I used to create various visual effects. Big thanks to [Mr. Lariviere](https://github.com/larivierec) for the help!

NOTE(s):
* [2018/11/05] None of the following stats are using multi-threading. I might implement threaded stages in the system's pipeline. WIP
* [2018/11/05] I'm currently working on GPU particles with Curl noise...

 Here are some highlights that I managed to create with system.

__Lit particles *or* particles with 200 dynamic cpu lights__
![lit particles or welding sparkles](https://github.com/frtru/GemParticles/blob/dev-master/gifs_and_screenshots/lit_particles.gif)

__500000 cpu particles attracted toward a location with hot and cold colors__
<img src="https://github.com/frtru/GemParticles/blob/dev-master/gifs_and_screenshots/attractor_1_cropped.gif" width="434" height="250">        <img src="https://github.com/frtru/GemParticles/blob/dev-master/gifs_and_screenshots/attractor_2_cropped.gif" width="434" height="250">

__and some rain... (1 million cpu particles with a steady 50 FPS)__
<img src="https://github.com/frtru/GemParticles/blob/dev-master/gifs_and_screenshots/heavy_rain_cropped_big.gif">

## External libraries
I'm using various external libraries in this project.
* ImGui
* GLM
* Glew
* GLFW
* FreeImage
* OpenGL

## Utilities
I pretty much built this from scratch looking up efficient design patterns, tutorials. So you'll find a handful of utilities that I built to help me create stuff more easily. Most of them are named as *modules*.
* Camera
* Light module
* Shader module
* Texture module
* Timer

And last but not least, some wrapper utilities I built around ImGui examples. This includes
* a Property editor
* a logger

## Extending the project
If one desires to create its own project using this *engine*, he or she can refer to the other projects' blueprints or \*\*\_project.cc files.
