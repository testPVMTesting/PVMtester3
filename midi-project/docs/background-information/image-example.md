# Binary File Example

The purpose of this text is to give you an simple example of how binary files are structured
and how to deal with them. MIDI files are relatively complex, so it might be a good idea
to get the basics of reading binary data right before tackling on MIDI.

## Specification

Let's say we need a format to encode images. We want to support arbitrary sizes up to 4 billion &times; 4 billion.
We also want to allow three different pixel formats:

* Grayscale (8 bits per pixel)
* RGB (24 bits per pixel)
* ARGB (32 bits per pixel)

Binary file contain bits. Bits can represent anything: maybe you need to group them by 8 to form
an ASCII char, or maybe by 32 to form an `int`, or by 64 to form a `double`.
It is therefore important that the structure be agreed upon beforehand.
This is of course exactly what a file format specification is for.

However, not all details are known beforehand. In our case, the width, height and pixel format
can vary from file to file. We need a way to inform the reader of the file what these are.

Binary files generally start with a header which describes the further contents of the file.
In our case, the header would contain information about the width, height and pixel format.
Based on that information, the rest of the file can be interpreted correctly.

The width and height can vary from 0 to 4 billion. A 32-bit unsigned integer is ideal to encode
this information. The pixel format is one of three types. Technically, we only need 2 bits,
but even binary files rather work on the level of bytes for efficiency reasons, i.e.,
storage is sacrificed for speed. In our experience, only compression formats (e.g. zip)
actually operate on the level of bits. So, even though we only need 2 bits, we'll use
a full byte for the pixel format. This results in the following header:

<center>

|Field|Size|
|-----|----|
|`width`|4|
|`height`|4|
|`pixel_format`|1|

</center>

We need to choose a pixel format representation: there are 256 possible values,
only 3 will mean something. Let's use the following mapping:

<center>

|Value|Pixel Format|
|:-----:|:----:|
| 1 | Grayscale |
| 3 | RGB |
| 4 | ARGB |

</center>

This representation has the advantage that the numeric value of `pixel_format`
corresponds to the number of bytes per pixel.

After the header comes the actual image data. We start with the first row of pixels,
followed by the second row, etc. There are `height` rows, and each row is `width * pixel_format` bytes long.
Therefore, the image itself consumes `width * height * pixel_format` bytes.

## Reading

Given a file following the earlier specification, we will read it in two phases.
First, we read in the header and based on the information encoded within it,
we read the rest of the file.

### Defining the Header

We start by defining the layout of the header as a struct:

```c++
struct HEADER
{
    uint32_t width;
    uint32_t height;
    uint8_t  pixel_format;
};
```

We use `uint32_t` and `uint8_t` because they are guaranteed to be 32 and 8 bit long, respectively.
The size of `unsigned` is platform and compiler dependent, so if it works, it does so by accident.

Since it's important we get the header exactly right, let's add some checks. We can use `static_assert`
to ask the compiler to performs checks at compile time. It has the following form:

```c++
static_assert(condition, error_message)
```

The error message will appear as compiler error if `condition` evaluates to false.
We can write our header assertions as follows:

```c++
static_assert(sizeof(HEADER) == 9, "HEADER must be exactly 9 bytes long");

// Check the order and placement of each field
static_assert(offsetof(HEADER, width) == 0, "HEADER::width has incorrect offset");
static_assert(offsetof(HEADER, height) == 4, "HEADER::height has incorrect offset");
static_assert(offsetof(HEADER, pixel_format) == 8, "HEADER::pixel_format has incorrect offset");
```

The compiler now complains about the header size: instead of the expected 9 bytes, it is 12 bytes long.
This is due to memory alignment: for efficiency reasons, the compiler pads the struct
to make its size a multiple of 4. How can we tell the compiler not to do this?

To prevent padding, you can use the `#pragma pack`:

```c++
#pragma pack(push, 1)
struct HEADER
{
    uint32_t width;
    uint32_t height;
    uint8_t  pixel_format;
};
#pragma pack(pop)
```

This tells the compiler to temporarily (between the `push` and `pop`) align
data on 1-byte boundaries, which in essence is equivalent to not
performing any kind of alignment.

Technically, `#pragma` directives are not guaranteed to be supported.
It is a directive that serves to "talk directly to the compiler".
If the compiler does not understand the directive, it is allowed to simply ignore it.
Normally, this is a dangerous thing to rely on, but fortunately,
we have our `static_assert`s to fall back on: either the `#pragma pack`
works as intended, or we get a compiler error.

## Reading the Header

We can open a file as follows:

```c++
std::ifstream in(filename);
```

This won't work, however, since by default files are opened in text mode.
To open it in binary mode:

```c++
std::ifstream in(filename, std::ios::binary);
```

Reading from the file happens with

```c++
in.read(buffer, nbytes)
```

where `buffer` must be a `char*` pointing to a memory buffer large enough to accomodate
`nbytes` number of bytes. `nbytes` should of course be the size of the header:

```c++
in.read(buffer, sizeof(HEADER));
```

We can allocate a buffer efficiently on the stack:

```c++
char buffer[sizeof(HEADER)];
in.read(buffer, sizeof(HEADER));
```

Note that in order to allocate an array on the stack, the size of the array must
be known at compile time. This is the case here: the definition of `HEADER`
is available to the compiler, therefore it can compute its size.

However, instead of using a `char[sizeof(HEADER)]`, it's easier to simply allocate
a `HEADER` directly:

```c++
HEADER header;
in.read(&header, sizeof(HEADER));
```

The compiler will complain now: `read` expects a `char*`, we're giving it a `HEADER*`.
No problem: we can force it to the correct type as follows:

```c++
HEADER header;
in.read(reinterpret_cast<char*>(&header), sizeof(HEADER));
```

Realize that `reinterpret_cast` is a dangerous cast: it essentially tells the compiler
to ditch the type system entirely and to just trust us.

Now we can read the header data by simply accessing `header`'s field.

## Defining the Pixel Types

We support three kinds of pixel formats, so we define
three corresponding pixel types:

```c++
struct GRAYSCALE
{
    uint8_t shade;
};

struct RGB
{
    uint8_t r, g, b;
};

struct ARGB
{
    uint8_t a, r, g, b;
};
```

There's no need for `#pragma pack` directives as there is no reason for the compiler
to add padding (see memory alignment slides to understand why.)

## Reading the Pixels

We prepare our buffer. Its size can be determined using `header`:

```c++
unsigned buffer_size = buffer.pixel_format * buffer.width * buffer.height;
```

Since the buffer size is unknown at compile time, we cannot allocate the buffer
on the stack. This would be a bad idea anyway, since a) the stack is limited in size
and b) we probably want to return a pointer to the pixel data, which we can't do
if it resides on the stack.

```c++
char* buffer = new char[buffer_size];
```

Next, we read in the data from the file:

```c++
char* buffer = new char[buffer_size];
in.read(buffer, buffer_size);
```

Lastly, we can cast our pointer to the right pixel format:

```c++
if ( header.pixel_format == 1 )
{
    auto pixels = reinterpret_cast<GRAYSCALE*>(buffer);
    // ...
}
else if ( header.pixel_format == 3 )
{
    auto pixels = reinterpret_cast<RGB*>(buffer);
    // ...
}
else if ( header.pixel_format == 4 )
{
    auto pixels = reinterpret_cast<ARGB*>(buffer);
    // ...
}
else
{
    abort();
}
```