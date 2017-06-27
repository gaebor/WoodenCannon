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

### Padding
Note that 

The bytes of the serialized object are packed, but 
## Stitching

## Fragments
