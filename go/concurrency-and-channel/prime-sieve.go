// prime-sieve.go
//
// Generate prime numbers through chained filters implemented with channel
//
package main

import "fmt"

// Generate number sequence: 2, 3, 4, ...
// serve as the 'base' filter that output numbers
func Generate(ch chan<- int) {
	for i := 2; ; i++ {
		ch <- i
	}
}

// Filter numbers received from one channel, send number that could not
// divide |prime| to another channel, thus, connect two channels
func Filter(in <-chan int, out chan<- int, prime int) {
	for {
		if n := <-in; n%prime != 0 {
			out <- n
		}
	}
}

// The prime sieve: Daisy-chain Filter processes
func PrimeSieve(out chan<- int) {
	ch := make(chan int)
	go Generate(ch)
	for i := 0; i < 10; i++ {
		prime := <-ch
		out <- prime
		ch1 := make(chan int)
		go Filter(ch, ch1, prime)
		ch = ch1
	}
}

func main() {
	ch := make(chan int)
	go PrimeSieve(ch)
	for i := 0; i < 10; i++ {
		fmt.Println(<-ch)
	}
}
