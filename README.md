# C++ Concurrent Programming 
C++ Example Code: Creating and Joining a Thread with Functor in a Class.
It is a very  useful  pattern because now what we're going to do is create the construction and the launch of a thread inside of its own class.

This code demonstrates different ways to create and spawn threads in a C++ class. It defines a class Launch that inherits from BannerBase and shows various methods for starting a thread within the class.

# Key points:

The Launch class has a constructor taking a const char* argument and a private std::thread member variable named thread_.

The operator() function of the Launch class runs a loop that prints the count until a specified system_clock::time_point has been reached.

The Launch class has five different methods for spawning a thread: SpawnA, SpawnB, SpawnC, SpawnD, and SpawnE. Each method demonstrates a different way to start a thread while avoiding issues with non-copyable objects and ensuring that the Launch object is passed correctly.

SpawnA: Uses a lambda function that captures this and tToc and calls the operator() function.

SpawnB: Similar to SpawnA, but with an intermediate std::thread object before moving it to the member variable thread_.

SpawnC: Uses a lambda function with a shorter syntax.

SpawnD: Uses std::ref to pass the Launch object by reference to the std::thread constructor.

SpawnE: Directly passes the Launch object's member function pointer and this to the std::thread constructor.

The destructor of the Launch class checks if the thread_ is joinable and joins it if necessary.

In the main function, five Launch objects are created with different names, and each object uses a different Spawn method to start the thread. This demonstrates the different ways to create threads in a class while managing object lifetimes and references correctly.

# Using std::ref to Pass Object References to Threads in C++.

In SpawnD, std::ref is used to create a reference wrapper around the Launch object, which allows passing the object by reference to the std::thread constructor. This is necessary because std::thread copies its arguments by default, and Launch objects contain a non-copyable member variable (std::thread thread_). By using std::ref, you can ensure that the Launch object is passed by reference and not copied.

When std::ref(*this) is used, a reference wrapper is created around the Launch object, which allows the object to be passed by reference to the std::thread constructor. This way, the original object is used in the thread, avoiding any issues related to copying non-copyable objects like std::thread.


# Move Assignment with std::thread and Non-Copyable Objects
This line uses the assignment operator to create a new std::thread object and assign it to the thread_ member variable of the class:
thread_ = std::thread(std::ref(*this), tToc);
Here, the assignment operator (=) assigns the newly created std::thread object to thread_. The std::thread constructor takes two arguments: std::ref(*this) and tToc. The std::ref function creates a reference wrapper for the *this object, allowing it to be passed to the thread without copying. tToc is the time point at which the thread should stop executing.

std::thread objects are not copyable. However, they are movable. The assignment operator in this case performs a move assignment, not a copy assignment.

When you create a new std::thread object, you are constructing a temporary object. 

The move assignment operator transfers the ownership of the thread's resources from the temporary object to the thread_ member variable. After the move assignment, the temporary object is left in a "valid but unspecified state", and the thread_ member variable holds the resources of the thread.

In C++11 and later, move semantics make it possible to efficiently transfer resources between objects, even if those objects are not copyable.
