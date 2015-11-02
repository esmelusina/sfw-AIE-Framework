# sfw-AIE-Framework

Contact me, Esmeralda Salamone, at esmes@aie.edu.au for any complaints/feature requests.

Simple wrapper for GLFW/GLM/STB and OpenGL Loader (I will totes cut this for GLEW because it has no acronym).

Oh, currently only links to release build of GLFW. Similarly, the debug build of this is worthless (laziness). Just use x86-release always when building.


Preliminary readme. Download release for example project. Instructions for use in nsfwdraw.h.

For users, sfw::initContext may only be called once per application execution and must be called before anything else (otherwise YMMV).
