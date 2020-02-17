# `EventMulticaster`

Write the `EventMulticaster` class. Its constructor accepts a vector of `std::shared_ptr<EventReceiver>`s. At each event X, it iterates
over the vector and calls each `EventReceiver`'s corresponding X member function.

Note: Visual Studio might indicate errors regarding a missing `check_finished` member function in this test file. You can ignore them, it'll compile and run just fine.
