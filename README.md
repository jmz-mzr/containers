# Containers

<br>

<p align="center">
  <img alt="A cargo ship carrying containers on the sea, its name written on its side is CPP" src="https://github.com/jmz-mzr/containers/assets/60391881/da47fb52-3fc2-4de2-9a0c-d75bb9ff9976">
</p>

<details align="center">
<summary><b>Table of Contents</b></summary>
<div align="left">
<br>

- [Overview](#overview)
  - [A word about Coding Style](#a-word-about-coding-style)
- [Key Features](#key-features)
- [Testing](#testing)
  - [Video](#tester-video)
- [Core Concepts](#core-concepts)
- [Vector](#vector)
- [Stack](#stack)
  - [An interesting finding](#an-interesting-finding)
- [Map](#map)
- [Set](#set)
- [The Red / Black Tree](#the-red-black-tree)
  - [Video](#tree-video)
- [Documentation](#documentation)

</div>
</details>

<br>

## Overview

“Containers“ is a C++ project aimed at recreating a handful of containers from the C++ standard library: [vector](#vector), [stack](#stack), [map](#map), and [set](#set).  
This project is not just about coding; it's about delving deep into the essence of C++, crafting performance-efficient, memory-safe containers with a clean coding style.

To make it fun & useful, two details matter:

- The containers must follow the [C++98 standard](https://www.lirmm.fr/~ducour/Doc-objets/ISO+IEC+14882-1998.pdf), and re-implement every single feature they had back then, even deprecated ones. No C++11, no external libraries, just pure & beautiful basics.
- They need to be memory safe, efficient, and closely mirror or outperform the performances of the STD library. For map & set, that means implementing a [Red / Black binary search tree](#the-red-black-tree).

To achieve our goal, what best start other than digging into the [LIBSTDC++](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3/include/bits) and [LIBC++](https://github.com/llvm/llvm-project/tree/main/libcxx/include) codebases?  
Being able to read and make sense of complex code is an essential skill — and it sometimes leads to little gems (see my findings on [stack](#stack)).

### A word about Coding Style

C++ is more verbose than C. However, being a strong advocate for [Clean Code](https://www.amazon.com/Clean-Code-Handbook-Software-Craftsmanship/dp/0132350882) & [Clean Architecture](https://www.amazon.com/dp/0134494164) practices — and to pay homage to the [42 School’s Norminette](https://github.com/42School/norminette) coding style — I made a point on delivering an explicit, clean, readable and well organised code. Have a look at [`vector`](https://github.com/jmz-mzr/containers/blob/master/includes/vector.hpp) for a start.

<br>

## Key Features

- **Complete Implementation**: Faithfully recreated vector, stack, map, and set as per C++98 standards
- **High Performance**: Performances even better than standard library containers
- **Memory Safety**: Carefully crafted to prevent memory leaks and ensure safe memory operations
- **Red / Black Tree**: Advanced Red / Black binary search tree implementation for map and set
- **Coding Style**: Emphasis on clean, self-explanatory, readable and organized code

<br>

## Testing

Nothing can be great until it’s well tested!

```
make test
```

Two testers are provided: the first is an [`introduction one`](https://github.com/jmz-mzr/containers/blob/master/tests/42_main.cpp), made by 42.  
The other is a [`comprehensive test suite`](https://github.com/jmz-mzr/containers/tree/master/tests/srcs), running small and then memory-heavy operations on both our containers (in the ‘ft’ namespace), and the containers in the STD library.  
The output and the time performances are compared: the output must be the same, and the time performances as well (Kudos, ours are even faster! 🎉)

Everything is tested, even the subtle requirements, for volatile, const and non-const versions:

- Containers created empty (even with types that cannot be default constructed)
- Working with custom allocators
- Constructors and destructors
- Typedefs
- Member operators
- Iterators
- Element access functions
- Capacity functions
- Modifiers functions
- Lookup functions
- Observers functions
- Non-member functions & operators  
  <span id="tester-video"></span>
  <br>

<div align="center">
  <video src="https://github.com/jmz-mzr/containers/assets/60391881/d44f8f37-a16a-4fe5-9116-69c911f4e5ad" height="1080" width="1920" />
</div>

<br>

## Core Concepts

Core C++ concepts and techniques — [SFINAE](https://en.wikipedia.org/wiki/Substitution_failure_is_not_an_error), [Template Metaprogramming](https://en.wikipedia.org/wiki/Template_metaprogramming), etc — are the heart of the containers. They are illustrated in the recreation of some handy tools: [`enable_if`](https://github.com/jmz-mzr/containers/blob/main/includes/enable_if.hpp), [`is_integral`](https://github.com/jmz-mzr/containers/blob/main/includes/is_integral.hpp), [`iterator_traits`](https://github.com/jmz-mzr/containers/blob/main/includes/iterator_traits.hpp), [`reverse_iterator`](https://github.com/jmz-mzr/containers/blob/main/includes/reverse_iterator.hpp), [`is_integral`](https://github.com/jmz-mzr/containers/blob/main/includes/is_integral.hpp), [`equal`](https://github.com/jmz-mzr/containers/blob/main/includes/equal.hpp), [`lexicographical_compare`](https://github.com/jmz-mzr/containers/blob/main/includes/lexicographical_compare.hpp), [`pair`](https://github.com/jmz-mzr/containers/blob/main/includes/pair.hpp), and [`make_pair`](https://github.com/jmz-mzr/containers/blob/main/includes/make_pair.hpp).

Some are hard to grasp, and it’s easier with examples: take a look at the tests written for them [`here`](https://github.com/jmz-mzr/containers/tree/main/tests/srcs).

#### Useful links:

- A handy FAQ about [C++ Templates](https://isocpp.org/wiki/faq/templates)
- More on [SFINAE](https://h--deb-clg-qc-ca.translate.goog/Sujets/Divers--cplusplus/SFINAE.html?_x_tr_sl=auto&_x_tr_tl=en&_x_tr_hl=fr&_x_tr_pto=wapp) with examples
- The complete [C++ Metaprogramming Library](https://en.cppreference.com/w/cpp/meta)

<br>

## Vector

```
template <
    typename T,
    class Allocator = std::allocator<T>
> class vector;
```

Vectors represent arrays that can change in size. They use contiguous storage locations, so their elements can be accessed with offsets on regular pointers as efficiently as in arrays. But their size can change dynamically, and their storage is handled automatically.

Compared to arrays, vectors consume more space, with extra memory allocated for future growth. But they grow in an efficient way: they reallocate only when their memory is exhausted. Reallocations happen at logarithmically growing intervals of their size, so insertions at the end are provided with [amortized constant time complexity](https://en.wikipedia.org/wiki/Amortized_analysis#Dynamic_array).

The total amount of allocated memory can be queried using [capacity()](https://en.cppreference.com/w/cpp/container/vector/capacity).  
The [reserve()](https://en.cppreference.com/w/cpp/container/vector/reserve) function can be used to eliminate reallocations if the number of elements is known beforehand.

Like arrays, vectors are very efficient at accessing elements, and adding / removing elements from their end.  
For inserting / removing elements other than at the end, containers like [deque](https://en.cppreference.com/w/cpp/container/deque) or [list](https://en.cppreference.com/w/cpp/container/list) perform better.

Internally, the implementation leverages SFINAE (e.g. to recognize pointers in [`insert()`](https://github.com/jmz-mzr/containers/blob/main/includes/vector.hpp#L626-L633) or [`assign()`](https://github.com/jmz-mzr/containers/blob/main/includes/vector.hpp#L497-L504)), the allocator’s [allocate()](https://en.cppreference.com/w/cpp/memory/allocator/allocate) & [deallocate()](https://en.cppreference.com/w/cpp/memory/allocator/deallocate) functions, or the [std::uninitialized_fill()](https://en.cppreference.com/w/cpp/memory/uninitialized_fill), [std::uninitialized_copy()](https://en.cppreference.com/w/cpp/memory/uninitialized_copy), and [std::copy_backward()](https://en.cppreference.com/w/cpp/algorithm/copy_backward) functions.  
As with the other containers, a clever architecture allows for core private functions (like [`_construct_at_end()`](https://github.com/jmz-mzr/containers/blob/main/includes/vector.hpp#L372-L398), [`_reallocate()`](https://github.com/jmz-mzr/containers/blob/main/includes/vector.hpp#L231-L301)) to be reused in other private & public ones.

#### Useful links:

- More [documentation](https://en.cppreference.com/w/cpp/container/vector) from _CPP Reference_
- The [`code`](https://github.com/jmz-mzr/containers/blob/main/includes/vector.hpp)

<br>

## Stack

```
template <
    typename T,
    class Container = ft::vector<T>
> class stack;
```

Stacks are container adaptors, a [LIFO (last-in-first-out)](<https://en.wikipedia.org/wiki/Stack_(abstract_data_type)>) data structure where elements are inserted and extracted only from one end. Stacks wrap an underlying container (by default here a vector), and push and pop the elements from the back of the container, known as the top of the stack.

### An interesting finding

Stacks leverage two comparison operators to achieve [all comparisons](https://en.cppreference.com/w/cpp/container/stack/operator_cmp): `operator==` and `operator<`.  
As stacks are template containers, in order for the compiler & linker to find the instantiated operators definitions, we either:

- Forward declare both the class and the operators templates, before declaring the operators as friend function template specializations (if not defining them directly inside the class template body)
- Or we declare all the instantiations of the operator templates as friend

These two methods have different implications, well detailed [here](https://stackoverflow.com/questions/4660123/overloading-friend-operator-for-class-template/4661372#4661372).  
One of them allows the operator to access a stack of any type, not just the `T` type of the stack currently compared!

Looking at their codebase, we observe that [LIBC++](https://github.com/llvm/llvm-project/blob/main/libcxx/include/stack#L278-L283) and [LIBSTDC++](https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/stl_stack.h#L111-L117) are not protected against this resulting potential vulnerability. Even more interesting, when we go back in LIBSTDC++ commit history, we see that the most secure way to grant this friendship [was there](https://github.com/gcc-mirror/gcc/blob/725dc051cada61aca8127f1062ec97a31acf79f1/libstdc%2B%2B-v3/bits/stl_stack.h#L38-L74), commented, before being [partially](https://github.com/gcc-mirror/gcc/blob/d53d7f6e612a61bfabc102466ec4203ac36a338e/libstdc%2B%2B-v3/include/bits/stl_stack.h), then [fully deleted later](https://github.com/gcc-mirror/gcc/blob/7ffb61d5030d7299320beb9b16978a66d6513d86/libstdc%2B%2B-v3/include/bits/stl_stack.h).

It might be hard to exploit, however people tend to get smart when trying to hack stuff ;)  
Launch the [tester](#testing) to see a live demo.

#### Useful links:

- More [documentation](https://en.cppreference.com/w/cpp/container/stack) from _CPP Reference_
- The [`code`](https://github.com/jmz-mzr/containers/blob/main/includes/stack.hpp)

<br>

## Map

```
template <
    typename Key,
    typename T,
    class Compare = std::less<Key>,
    class Allocator = std::allocator<ft::pair<const Key, T> >
> class map;
```

Maps are associative containers, storing elements as pairs of `key value` & `mapped value`, with unique keys.

Key values are sorted using the `Compare` function. The mapped values store the content associated to a key.  
The content can be accessed directly with the corresponding key using the [bracket operator[]](https://en.cppreference.com/w/cpp/container/map/operator_at) (which creates a new element if the key doesn't already exist).

Since the elements are sorted, search, removal, and insertion operations have logarithmic complexity.  
Maps are typically implemented as binary search trees — here it is a [Red / Black tree](#the-red-black-tree).

#### Useful links:

- More [documentation](https://en.cppreference.com/w/cpp/container/map) from _CPP Reference_
- The [`code`](https://github.com/jmz-mzr/containers/blob/main/includes/map.hpp)

<br>

## Set

```
template <
    typename Key,
    class Compare = std::less<Key>,
    class Allocator = std::allocator<Key>
> class set;
```

Sets are containers that store unique elements of type `Key`.

The elements are sorted using the `Compare` function. They cannot be modified once in the container (the values are always `const`), but they can be inserted and removed from the container.

Since the elements are sorted, search, removal, and insertion operations have logarithmic complexity.  
Sets are typically implemented as binary search trees — here it is a [Red / Black tree](#the-red-black-tree).

#### Useful links:

- More [documentation](https://en.cppreference.com/w/cpp/container/set) from _CPP Reference_
- The [`code`](https://github.com/jmz-mzr/containers/blob/main/includes/set.hpp)

<br>

<span id="the-red-black-tree"></span>

## The Red / Black Tree

[Map](#map) and [Set](#set) both rely on an underlying [Red / Black binary search tree](https://en.wikipedia.org/wiki/Red–black_tree).

It is a self-balancing binary tree, where each node has an extra information of color (red or black) used in the [`insertion`](https://github.com/jmz-mzr/containers/blob/main/includes/_tree.hpp#L110-L170) & [`removal`](https://github.com/jmz-mzr/containers/blob/main/includes/_tree.hpp#L172-L314) algorithms to satisfy the tree’s [core properties](https://medium.com/@ben.tom.vincent/why-use-a-red-black-tree-over-a-regular-binary-search-tree-a63857916907#:~:text=Properties%20%E2%80%94%20all%20must,of%20black%20nodes).  
These algorithms keep the tree balanced, and ensure that the time complexity for insertion, deletion, and searching is always O(log n).

Internally, the tree’s `root` is the left child of its `end_node`. So if `root != NULL`, `end_node->left` points to `root`, and `root->parent` points to `end_node`.  
To optimize memory usage and minimize complexity, the [`node_base`](https://github.com/jmz-mzr/containers/blob/main/includes/_tree.hpp#L371-L387) class manages the pointers and the color bit, while the deriving [`node`](https://github.com/jmz-mzr/containers/blob/main/includes/_tree.hpp#L389-L405) class contains the actual node’s value.

The nature of the tree allows the use of recursion, for example in [`_structural_copy()`](https://github.com/jmz-mzr/containers/blob/main/includes/_tree.hpp#L796-L820), in [`_destroy()`](https://github.com/jmz-mzr/containers/blob/main/includes/_tree.hpp#L850-L859) or in [`_print()`](https://github.com/jmz-mzr/containers/blob/main/includes/_tree.hpp#L921-L943). This print function is used to visualize the content of the tree, especially in the context of...

#### The tree tester:

```
make tree && ./_tree_tests
```

A custom tester, showing what the current tree looks like, along with the expected results.  
As with the [containers’ tester](#testing), everything is tested, from the ability to create an empty tree, with custom allocators, to all the member functions & operators.
<span id="tree-video"></span>  
<br>

<div align="center">
  <video src="https://github.com/jmz-mzr/containers/assets/60391881/aeebc974-49a9-4efd-ad34-2ecbc7b5bcc4" height="1080" width="1920" />
</div>

<br>

## Documentation

- [Bjarne Stroustrup's website](https://www.stroustrup.com/index.html)
- [IsoCPP Tour](https://isocpp.org/tour)
- [IsoCPP Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
- [CPP Rererence](https://en.cppreference.com/w/cpp)
- [CPlusPlus Reference](https://cplusplus.com/reference/)
- [CPlusPlus Tutorials](https://cplusplus.com/doc/tutorial/)
- [LearnCPP Tutorials](https://www.learncpp.com/)
- [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
