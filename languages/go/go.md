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

Methods can be made outside the struct make it very flexible.

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

## 4. Go routine
