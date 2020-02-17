# `NoteCollector`

Write the `EventReceiver` subclass `NoteCollector`. It should
work just like a `ChannelNoteCollector`, except that it
works on all channels instead of just one.
Internally, it should create an `EventMulticaster` and 16 `ChannelNoteCollector`s.
