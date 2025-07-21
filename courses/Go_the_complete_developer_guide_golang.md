# GO: The complete developer's guide GOlang

## 1. Getting started

### 4. Setup environment

```bash
wget https://go.dev/dl/go1.19.5.linux-amd64.tar.gz
rm -rf /usr/local/go && tar -C /usr/local -xzf go1.19.5.linux-amd64.tar.gz

vim /etc/profile
:$
export PATH=$PATH:/usr/local/go/bin
:wq

```

Or

```bash
apt install golang-go
```

## 2. A Simple Start

Hello world example:

```go
package main

import "fmt"

func main() {
    fmt.Println("Hello world")
}
```

### 8. Five Important Questions

- Go CLI:
  - go build: compiles a bunch of go source code files.
  - go run: compiles and executes one or two files.
  - go fmt: Formats all the code in each file in the current directory.
  - go install: compiles and "installs" a package.
  - go get: downloads the raw source code of someone else's package.
  - go test: runs any test associated with the current project.

### 9. GO packages

- What does 'package main' mean?
  - Package == Project == Workspace
  - So a package can have many related files inside of it, each file ending with a file extension of GO.

- Types of Packages:
  - Executable: Generates a file that we can run.
  - Reuseable: Code used as 'helpers'. Good place to put reusable logic.

- Executable package { package main ---> Defines a package that cam be compiled and then *executed*. Must have a func called 'main'.

- Reuse package -- package calculator: Defines a package that can be used as a dependency (helper code)
                -- package uploader: Defines a package that can be used as dependency (helper code)
                -- etc.

### 10. Import Statements

- What does 'import "fmt"' mean?
- Import other packages that we need.

## 3. Deeper Into Go

### 13. Project Overview

- Cards
  - NewDeck ------> Create a list of playing cards. Essentially an array of strings.
  - print ------> Log out the contents of a deck of cards.
  - shuffle -----> Shuffles all the cards in a deck.
  - deal -----> Create a 'hand' of cards.
  - saveToFile -----> Save on a list of cars to a file on the local machine.
  - newDeckFromFile -----> Load a list of cards form the local machine.

### 14. Variable Declaration

```go
var x string
var y string = "Hello world"
z := "Welcome to the Hell"
var i, j int = 1, 2
```

- Static Types: C++, Java, GO
- Dynamic Types: Javascript, Ruby, Python

### 16. Function and return types

```go
func newCard() (string, bool) {
  return "Hello world", true
}
```

### 17. SLices and for loops

- Array: Fixed length list of things.

```go
cards := [2]string{"Ace of Diamonds", "Larva"}
var cards [2]string
```

- Slice:
  - An array that can grow or shrink.
  - Every element in a slice must be of same type.

```go
cards := []string{"Ace of Diamonds", "Larva"}
var cards []string
```

- Append to a slice:

```go
cards = append(cards, "Hello world")
```

- For loop:

```go
for index, card := range cards {
  fmt.Println(card)
}
```

- index: index of this element in the array.
- card: Current card we have iterating over.
- range cards: Take the slice of 'cards' and loop over it.
- fmt.Println(card): Run this one time for each card in the slice.

### 18. OO Approach and Go Approach

- Object Oriented Approach:
  - Deck Class ----> Deck instance: print(), shuffle(), saveToFile(), etc. methods.
- Go Approach:
  - Base Go types: string, integer, float, array, map.
  - We want to "extend" a base type and add some extra functionality to it.
    - type deck []string: Tell Go we want to create an array of strings and add a bunch of functions specifically made to work with it.
    - Functions with 'deck' as a 'receiver': A function with a receiver is like a "method" - a function that belongs to an "instance".

### 19. Custom type declaration

```go
type NewType []string
var x NewType = []string{"Hello"}
```

### 20. Receiver function

Golang receiver function sets a method on variables that we create. With Golang receiver functions, you don't have to mess around with classes or deal with inheritance. For example:

```go
// define a custom type
type int_custom int

func (x int_custom) print() {
  // make a copy of x inside this function.
  fmt.print(x)
}

func main() {
  var x int_custom = 10
  // Every variable of type 'deck' can call this function on itself
  x.print()
}
```

- NOTE: Can't define new method on local type: int, string, etc. Need to define custom type (with alias, etc.)
- NOTE: can use 'this' or 'self' inside receiver function, but it is breaking convention, Go avoids any mention of 'this' or 'self'.

### 21. Creating a new deck

### 22. Slice range syntax

```go
var array_1 []string

var array_2 []string = array_1[3:]
var array_3 []string = array_1[:5]
```

### 23. Multiple Return Values

```go
func getBookInfo() (string, int) {
    return "War and Peace", 1000
}
 
title, pages := getBookInfo()
```

### 24. byte slices

- For example:
  - string: "Hi there!" equal to byte slice: [72 105 32 116 104 101 114 101 33]

- package: [io/ioutil](https://pkg.go.dev/io/ioutil)

```go
func WriteFile(filename string, data []byte, perm fs.FileMode) error
```

- NOTE: type `byte` is equal to `unsigned char` or `uint8_t` in C/C++.

### 25. deck to string

- Explicit type conversion:

```go
[]byte("Hi there!")
```

### 26. Joining a slice of string

```go
import "strings"
strings.join([]string(d), ",")
```

### 29. Error handling

```go
import (
  "strings"
  "os"
  "io/ioutil"
)
func main() {
  bs, err := ioutil.ReadFile("file.txt")
  if err != nil {
    fmt.Println("Error:", err)
    os.Exit(1)
  }
}
```

### 30. Assign multiple variables

```go
var a int = 0
var b int = 0
var c int = 0
a, b, c = c, b, a
```

### 32. Testing with Go

- Go testing is not RSpec, mocha, jasmine, selenium, etc!
- To make a test, create a new file ending int `_test.go`
- To run all tests in a package, run the command...

```bash
go test
```

### 33. Writing useful tests

```go
func TestNewDeck(t *testing.T) {
  d := newDeck()
  if len(d) != 16 {
    t.Errorf("Expected deck length of 16, but got %v", len(d))
  }
}
```

## 4. Organizing Data With Structures

### 37. Structs in Go

### 38. defining structs

```go
type person struct {
  firstName string
  lastName string
}

func main() {
  alex := person{"Alex", "Anderson"}
  john := person{firstName: "John", lastName: "Jonathan"}
}
```

### 39. updating struct field

- string: ""
- int: 0
- float: 0
- bool: false

### 44. Struct with pointer

```go
type person struct {
  firstName string
  lastName string
}

func (pointerToPerson *person) updateName(newFirstName string) {
  (*pointerToPerson).firstName = newFirstName
}
```

### 45. Pointer Operations

- `&variable`: Give me the memory address of the value this variable is pointing at.
- `*pointer`: Give me the value this memory address is pointing it.

### 46. Pointer shortcuts

```go
type person struct {
  firstName string
  lastName string
}

func (pointerToPerson *person) updateName(newFirstName string) {
  (*pointerToPerson).firstName = newFirstName
}

func main() {
  var Jim person
  PointerToJim := &Jim
  // normal
  PointerToJim.updateName("Jim")
  // is equal to this shortcut:
  Jim.updateName("Jim")
}
```

- Go will automatically assume that even thought `Jim.updateName("Jim")` is using a value type we probably meant to pass in a pointer to the `person struct`.

### 47. Gotchas with pointer

- Same as C, slices (did not specific length of array) are equal to pointer.

```go
func updateSlice(s []string) {
  s[0] = "Bye"
}

func main() {
  mySlice := []string{"Hi", "There", "How", "are", "you"}
  updateSlice(mySlice)
  fmt.println(mySlice)
}

// Output: [Bye There How Are You]

```

### 48. Reference vs Value type

- Arrays:
  - Primitive data structure.
  - Can't be resized.
  - Rarely used directly.
- Slices:
  - Can grow and shrink.
  - Used 99% of the time for lists of elements.
  - An array and a structure that records the length of the slice, the capacity of the slice, and a reference to the underlying array.
  - contains:
    - pointer to head.
    - capacity.
    - length.

- Value types: Use pointers to change these things in a function.
  - int
  - float
  - string
  - bool
  - structs

- With 'value types' in Go, we have to worry about pointers if we want to pass a value to a function and modify the original value inside the function.

- Reference types: don't worry about pointers with these.
  - slices: because a slice contains a reference to the actual underlying list of records.
  - maps.
  - channels.
  - pointers.
  - functions.

## 6. Interface

### 53. Purpose of Interfaces

- We know that ...:
  - Every value has a type.
  - Every function has to specify the type of its arguments.
- So does that mean ...:
  - Every function we ever write has to rewritten to accommodate different types even if the logic in it is identical?
  - for example:
    - func (d deck) shuffle(): Can only shuffle a value of type 'deck'
    - func (s []float64) shuffle(): Can only shuffle a value of type '[]float64'
    - func (s []string) shuffle(): Can only shuffle a value of type '[]string'

### 54. Problems without interfaces

- In Go language, the interface is a custom type is used to specify a set of one or more method signatures and the interface is abstract, so you are not allowed to create an instance of the interface. But you are allowed to create a variable of an interface type and this variable can be assigned with a concrete type value that has the methods the interface requires. Or in other words, the interface is a collection of methods as well as it is a custom type.

- How to create an interface?

```go
type interface_name interface{
  // Method signatures
}
```

- For Example:

```go
// Creating an interface
type myInterface interface{

// Methods
fun1() int
fun2() float64
}
```

- Important Points:
  - The zero value of the interface is nil.
  - When an interface contains zero methods, such types of interface is known as the empty interface. So, all the types implement the empty interface.
  - syntax: `interface{}`
  - Interface Types: The interface is of two types one is static and another one is dynamic type. The static type is the interface itself. But interface does not have a static value so it always points to the dynamic values.

  - Type Assertions: In Go language, type assertion is an operation applied to the value of the interface. Or in other words, type assertion is a process to extract the values of the interface.
    - Syntax: `a.(T)`
    - Here, `a` is the value or the expression of the interface and `T` is the type also known as asserted type.
    - The type assertion is used to check that the dynamic dynamic type of its operand will match the asserted type or not. If the `T` is concrete type, then the type assertion checks proceeds successfully, then the type assertion returns the dynamic value of `a`. Or if the checking fails, then the operation will panics. If the `T` is of an interface type, then the type assertion checks the given dynamic type of a satisfies `T`, here if the checking proceeds successfully, then the dynamic value is not extracted.

```go
// Go program to illustrate
// the type assertion
package main
  
import "fmt"
  
func myfun(a interface{}) {
  
    // Extracting the value of a
    val := a.(string)
    fmt.Println("Value: ", val)
}
func main() {
  
    var val interface {
    } = "Hello world"
      
    myfun(val)

    var str string = "Hello Hell"
    myfun(str)
}
```

### 57. Extras interface notes

- Interfaces are not generic types: Other languages have 'generic' types - go (famously) does not.
- Interfaces are 'implicit': We don't manually have to say that our custom type satisfies some interface.
- Interfaces are a contract to help us manage types: GARBAGE IN -> GARBAGE OUT. If our custom type's implementation of a function is broken then interfaces won't help us.
- Interfaces are tough. step #1 is understanding how to read them: Understand how to read interfaces in the standard lib. Writing your own interfaces is tough are requires experience.

### 58. The HTTP Package

```go
package main

import (
  "fmt"
  "net/http"
  "os") 

func main() {
  resp, err := http.Get("http://google.com")
  if err != nil {
    os.Exit(1)
  }
  bs := make([]byte, 9999)
  resp.Body.Read(bs)
  fmt.Println(string(bs))
  fmt.Println(resp.StatusCode)
}
```

### 62. The reader interface

- Source of Input:
  - HTTP Request Body.
  - Text file on hard drive.
  - Image file on hard drive.
  - User entering text into command line.
  - Data from analog sensor plugged into machine.

### 65. The writer interface

- writer interface describes something that can take info and send it outside of our program.

|[]byte| ---> |writer|---> |source of Output|

- source of Output: We need to find something in the standard library that "implements" the Writer interface, and use that to log out all the that we are receiving from the Reader.
  - Outgoing HTTP request
  - Text file on hard drive.
  - Image file on hard drive.
  - Terminal.

### 66. The IO copy function

```go
func Copy(dst Writer, src Reader) (written int64, err error)
```

## 7. Channels and Go Routines

- A goroutine is a **lightweight thread** of execution managed by the Go runtime.

```go
go f(x, y, z)
// starts a new goroutine running: f(x, y, z)
```

- The evaluation of `f`, `x`, `y`, `z` happens in the current goroutine and the execution of `f` happens in the new goroutine.

- Go-routines run in the same address space, so access to shared memory must be synchronized. The `sync` package provides useful primitives, although you won't need them much in Go as there are other primitives.

- **Channels** are a typed conduit though which you can send and receive values with the channel operator, `<-`.

```go
ch <- v    // Send v to channel ch.
v := <-ch  // Receive from ch, and
           // assign value to v.
```

- Like maps and slices, channels must be created before use:

```go
ch := make(chan int)
```

- by default, sends and receives block until the other side is ready. This allows GO-routines to synchronize without explicit locks or condition variables.

- **Buffered Channels**
  - channels can be buffered. Provide the buffer length as the second argument to `make` to initialize a buffered channel:
  
  ```go
  ch := make(chan int, 100)
  ```

  - Sends to a buffered channel block only when the buffer is full. Receives block when the buffer is empty.

  ```go
  package main

  import "fmt"

  func main() {
    ch := make(chan int, 2)
    ch <- 1
    ch <- 2
    fmt.Println(<-ch)
    fmt.Println(<-ch)
  }
  ```

- **Select**
  - The `select` statement lets a goroutine wait on multiple communication operations.
  - A `select` blocks until one of its case can run, then it executes that case. It chooses one at random if multiple are ready.

  ```go
  package main

  import "fmt"

  func fibonacci(c, quit chan int) {
    x, y := 0, 1
    for {
      select {
      case c <- x:
        x, y = y, x+y
      case <-quit:
        fmt.Println("quit")
        return
      }
    }
  }

  func main() {
    c := make(chan int)
    quit := make(chan int)
    go func() {
      for i := 0; i < 10; i++ {
        fmt.Println(<-c)
      }
      quit <- 0
    }()
    fibonacci(c, quit)
  }
  ```

  - **Default Selection** The default case in a select is run if no other case is ready.
  - Use a default case to try a send or receive without blocking:

  ```go
  select {
  case i := <-c:
      // use i
  default:
      // receiving from c would block
  }
  ```

- **sync.Mutex**
  - We've seen how channels are great for communication among go-routines.
  - But what if we don't need communication? What if we just want to make sure only one goroutine can access a variable at a time to avoid conflicts?
  - This concept is called *mutual exclusion*, and the conventional name for the data structure that provides it is *mutex*.

  - Go's standard library provides mutual exclusion with `sync.Mutex` and its two methods:
    - Lock
    - Unlock
  - We can define a block of code to be executed in mutual exclusion by surrounding it with a call to `Lock` and `Unlock` as shown on the `Inc` method.

We can also use `defer` to ensure the mutex will be unlocked as in the `Value` method.

  ```go
  package main

  import (
    "fmt"
    "sync"
    "time"
  )

  // SafeCounter is safe to use concurrently.
  type SafeCounter struct {
    mu sync.Mutex
    v  map[string]int
  }

  // Inc increments the counter for the given key.
  func (c *SafeCounter) Inc(key string) {
    c.mu.Lock()
    // Lock so only one goroutine at a time can access the map c.v.
    c.v[key]++
    c.mu.Unlock()
  }

  // Value returns the current value of the counter for the given key.
  func (c *SafeCounter) Value(key string) int {
    c.mu.Lock()
    // Lock so only one goroutine at a time can access the map c.v.
    defer c.mu.Unlock()
    return c.v[key]
  }

  func main() {
    c := SafeCounter{v: make(map[string]int)}
    for i := 0; i < 1000; i++ {
      go c.Inc("somekey")
    }

    time.Sleep(time.Second)
    fmt.Println(c.Value("somekey"))
  }
  ```

### 79. Alternative loop syntax

```go
for {
  // run forever.
}
```

### 81. Function literal

- Javascript: Anonymous Function.
- Ruby: lambda
- python: lambda
- GO: FUnction literal.

```go
func(args arg_type) {
  // do some thing. with args is referenced.
}(args)
```
