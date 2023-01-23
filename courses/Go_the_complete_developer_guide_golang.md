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

### 53. Purpose o Interfaces
