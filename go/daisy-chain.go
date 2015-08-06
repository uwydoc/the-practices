package main

import "fmt"

func main() {
	f := func(left, right chan int) {
		left <- 1 + <-right
	}
	leftmost := make(chan int)
	left := leftmost
	right := left
	for i := 0; i < 10000; i++ {
		right = make(chan int)
		go f(left, right)
		left = right
	}
	go func() { right <- 1 }()
	fmt.Println(<-leftmost)
}
