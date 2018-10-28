# How does WC work?

## Overriding operator new

Imagine that there is a C++ object held in your memory. Full of values of different type and pointers to each other. The object may use distant blocks of memory, pointing to each other.

                                                 ↓
    #bytes  0 1 2 3 4 5 6 7 8 9 101112 ...       5051525354555657585960 ...     7071
            -------+-------+-------+----       -+-------+---+-------+--      --+----
    content   int  |  ptr  | float |            |  ptr  |   |  ptr  |          |
            -------+-------+-------+---        -+-------+---+-------+--      --+----
                        │   ↑                       │        ↑  │               ↑
                        │   │                       └────────┘  │               │
                        │   └───────────────────────────────────┘               │
                        └───────────────────────────────────────────────────────┘

The down arrow is the `this` pointer of the object to serialize.
The pointed value of the pointers (targets) are marked with up arrows.

To serialize such a tangled object, the _`operator new` is overridden_.
We call a _copy constructor_ during which
the new `operator new` makes sure that only consecutive, adjacent blocks are allocated.
As a result, the copied object is now packed in a single block of memory.

            ↓                                     
    #bytes  90919293949596979899000102030405060708091011121314
            +-------+---+-------|-------+-------+-------|---- 
    content |  ptr  |   |  ptr  |  int  |  ptr  | float |     
            +-------+---+-------|-------+-------+-------|---  
                │        ↑  │               │    ↑       ↑
                └────────┘  │               └────┼───────┘
                            └────────────────────┘

If one does not want to serialize then the original (non-chipped) copy constructor is used.

Note that the target of the pointers are irrelevant, as long as the copy constructor is not ill-written.
Note that solely the chipped `operator new` can serialize your objects during a copy constructor.

    //switch to the chipped `operator new`
    MyClass* serialized = new MyClass(*object_to_serialize);
    //switch back to the original `operator new`

### Alignment
The chipped `operator new` always rounds up the allocated space to ensure alignment.
But only `alignof(std::max_align_t)` alignment is supported.

The padding of the members are managed by the compiler, those paddings do not interfere with `wc`.

## Stitching
After the aforementioned process, `wc` does a so called _Stitching_ during which

* pointers are converted into a relative format, in this way you can use them across processes.
  * nullptr is a bit special
* fundamental members are converted into network byte order (optional)
* custom callbacks may be called

### Pointers
During stitching every pointer is converted like this:

    void memory2buffer(void** p) { *p = *p - p; }

Which means that the pointer becomes relative to its position. If a pointer points to itself, then becames `0`.
The relative pointer `2` means that the pointer points two bytes after itself. Negative values mean that the pointer points somewhere before itself.

In this way, the serialized object can be dumped into a file, and later its pointer became valid again (by converting back after read into memory again).

    void buffer2memory(void** p) { *p = *p + p; }

#### nullptr
The nullptr is convreted to `std::numeric_limits<std::ptrdiff_t>::min()` which means that it is less than any other pointers.
This trick fails if you use more than `std::numeric_limits<size_t>::max() / 2` memory!

### Members
Converting the pointers and the members (network byte order) requires the knowledge of the position of the members.

This is done by the user who has to specialize a template class, which communicates the position (and type) of the members with `wc`.

    template<>
    wc::MembersOf<MyClass> ...

You can define three type of members
* plain member
* pointer
* responsible pointer

The members of any class is a special list of these:

    template<>
    struct wc::MembersOf<MyClass>
        : wc::Members<MyClass,
            wc::Member<...>,
            wc::Pointer<...>,
            wc::Responsible<...>
        >
    {};

A member template class has three template parameters:
* the class for which it is a member of
* its offset (`offsetof`)
* its type

For example

    wc::Member<MyClass, 0, int>

This means that your class has an `int` member at offset `0`. Or, lets say the member is `int MyClass::i` then

    wc::Member<MyClass, offsetof(MyClass, i), decltype(i)>

## Fragments
