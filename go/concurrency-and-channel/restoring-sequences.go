package main

import (
	"fmt"
	"math/rand"
	"time"
)

type Message struct {
	str  string
	wait chan bool
}

var waitForIt = make(chan bool)

func Sequence(msg string) chan Message {
	ch := make(chan Message)
	go func() {
		for i := 0; ; i++ {
			ch <- Message{fmt.Sprintf("%s %v", msg, i), waitForIt}
			time.Sleep(time.Duration(rand.Intn(1e3)) * time.Millisecond)
			<-waitForIt
		}
	}()
	return ch
}

func main() {
	ch1 := Sequence("Ann")
	ch2 := Sequence("Joe")
	for i := 0; i < 5; i++ {
		m1, m2 := <-ch1, <-ch2
		fmt.Println(m1.str)
		fmt.Println(m2.str)
		m1.wait <- true
		m1.wait <- true
	}
}
