# Rendering

In this last sequence of steps, you will generate the actual visualization of the MIDI data.
You are not given any tests: it is easy for you to actually run your program and check
whether the results agree with your expectations.

The goal is to end up with an actual animation:

* [A horizontal animation with rectangles](https://youtu.be/p5favl2Qtx0)
* [A vertical animation](https://youtu.be/BV7RkEL6oRc)
* [Rhombi!](https://youtu.be/Vh-7NZbg65M)
* [Slightly more ambitious](https://youtu.be/hyCIpKAIFyo)

You are free to choose your own visualization, but it has to be in a similar vein as the examples given above:
notes have to be represented by some shape and there is a sliding view window.
This explanation assumes you choose to produce a horizontal animation with rectangles, as this is
probably the simplest to implement.

We suggest you take the following steps:

* Start with experimenting with the `Bitmap` class.
  * Find out how to draw pixels.
  * Once you can draw pixels, you can also draw rectangles.
  * Find a way to write a bitmap to file.
* Create a single large bitmap on which you draw all notes.
  * A few details will need to be parameters: for example, how high each rectangle is should be specified by the user. However, we strongly suggest you start simple and hard code values. Generalize the rendering algorithm in a separate step.
  * The drawing function should check which are the lowest and highest notes in use and
  choose the correct bitmap height correspondingly. In other words, cut off
  superfluous black rows of pixels at the top and bottom of the bitmap.
* Once you can produce a large bitmap of the music, you can use it to generate an animation by slicing overlapping frames out of it. For example, say the bitmap is 10,000 pixels wide and you want the actual movie to be 500 pixels wide. The first frame will span from 0-500, the next ones from 5-505, 10-510, 15-515, etc. The horizontal step between frames (here 5) can be chosen arbitrarily: the smaller it is, the smoother the animation will be.
* Both the rendering of the large bitmap and the frame generation functions have multiple parameters, which should be specified by the user. Since the midi visualizer is a command line tool, you can receive these parameters as command line arguments. Rely on `CommandLineParser` to parse `main`'s arguments.

You must at least support the following command line parameters:

* `-w N`: the frame width. If left unspecified, it defaults to the total width, i.e., only one frame will be produced.
* `-d N`: the horizontal step between frames. In the example above, a value of 5 was used. If left unspecified, this value should be 1.
* `-s N`: the horizontal scale. Lets the user scale the bitmap horizontally. E.g., `-s 100` produces a bitmap twice as wide as `-s 50`. You can interpret the value as the width of a note that lasts one second, but you are not required to implement this exactly. Pick a default value that makes sense.
* `-h N`: the pixel height of each note rectangle. Default value: 16.
* `filename`: MIDI file.
* `pattern`: determines the file names of each frame. Uses `%d` to indicate where to insert the frame index. This index should always count 5 digits. For instance, `frame-%d.bmp` should produce files named `frame-00000.bmp`, `frame-00001.bmp`, `frame-00002.bmp`, etc.

For example,

```bash
$ midi -h 10 -w 500 music.mid f%d.bmp
```

reads in `music.mid` and produces frames `f00000.bmp`, `f000001.bmp`, etc. Each note rectangle is 10 pixels high. Frames are 500 pixels wide and move one pixel at a time.