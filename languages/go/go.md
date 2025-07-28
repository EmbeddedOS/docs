# Go summary

## 1. Defer

- Adding a callback when scope exit.

```go
{
  defer fmt.Println("Good bye!")

  fmt.Println("Hi there!")
  // Do some thing.
}
```

## 2. struct and method

Methods can be made outside the struct make it very flexible, don't have to change the struct itself while we can add custom methods for different modules without knowing about others.

```go
type Point struct
{
	x, y float64
}

// You don't want to change original point.
func (p Point) sum() float64 {
	return p.x + p.y
}

// you can change original point.
func (p *Point) sum() float64 {
	return p.x + p.y
}
```

## 3. Interface

An interface is a type. That is similar like a pointer to a abstract class in C++, that you can point to derived classes, convert and act like children.
This feature is how abstraction and polymorphism work in Go.

Let say you have 2 structs that have same one kind of interface, for example, dog run and cat run:

```go
type Dog struct
{
	age int
	name string
}

type Cat struct
{
	age int
	name string
}

type Animal interface {
	run()
}
```

Now you implement two methods for those structs:

```go
type Animal interface {
	run()
}

func (d *Dog) run() {
	fmt.Println("Dog run")
}

func (d *Cat) run() {
	fmt.Println("Cat run")
}

func main() {
	dog:= Dog{}

	var animal Animal
	animal = &dog

	animal.run()
}
```

The better than C++ is that, you don't have to inherit an abstract class, to make an interface, for now you can make interfaces outside of the struct without changing the struct, that make it very flexible. Different modules can define different interfaces without knowing each other.

## 4. Go routine

## 5. Memory allocation
