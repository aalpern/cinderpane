Overview
########

cinderpane is a UI library for libcinder, a cross-platform "creative
coding" library in C++. The name comes from the idea of a window pane;
an element of a 2D layer which overlays the 3D world that you see
through it. An interface is composed of multiple panes, each of which
is a positionable element on the 2D layer of a Cinder
application. Each pane is generally rendered by a GL Texture.

Status
------

This is very much the preliminary stages of a work in progress.

Installation
------------

cinderpane is currently organized as a header-only library. The
primary reason for that is for simplicity's sake. While developing the
core library, learning GL, and learning XCode, I didn't want to also
need to learn the ins and outs of building libraries on OS X as well.
