# Empty MTrk

Write a function `read_mtrk` that takes a `std::istream` and
an `EventReceiver`. The `istream` is positioned at the beginning
of an MTrk chunk. `read_mtrk` is expected to
go through all events in the MTrk and call the `EventReceiver`'s
corresponding methods.

Since `read_mtrk` is a rather long function (more than 100 lines in
my own implementation, but it contains many uninteresting lines, such as a sole `{` or `}`),
we'll build it up gradually.

First, write `read_mtrk` so that it can process an "empty" MTrk chunk,
that is, an MTrk with just an end-of-track event.
In byte form, an empty MTrk looks like this:

```text
M T r k               MTrk chunk identifier
0 0 0 4               Length of the chunk
0 0xFF 0x2F 00        End-of-track event
```

The end-of-track event, as described in the MIDI background information,
consists of the following bytes:

* `0x00`: Delta time since last event. It makes little sense to delay this event in time.
* `0xFF`: Identifier for meta event
* `0x2F`: End of stream identifier
* `0x00`: Meta event data length. An end-of-track event carries no extra data, so 0.

This decomposition of the byte stream can also be found within the tests.

Given this input stream, `read_mtrk` is expected to make a single call
to `EventReceiver::meta` with

* `dt = 0`
* `type = 0x2F`
* `data =` zero-length-byte[]
* `data_size = 0`

Note that `meta` expects a `std::unique_ptr<byte[]>`. Unique pointers
can't simply be passed to functions, since that would invoke
the copy constructor, and copying unique pointers defeats
the purpose of them being *unique*. Instead,
you'll need `std::move(ptr)` to explicitly carry over ownership.

```cpp
auto p = std::make_unique<byte[]>(5);

// p != nullptr
f(std::move(p));
// p == nullptr
```