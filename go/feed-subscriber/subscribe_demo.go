package main

import (
	"fmt"
	"time"
)

func main() {
	// Subscribe to several feeds and create a merged update stream
	merged := Merge(
		Subscribe(Fetch("http://ericniebler.com/feed/")),
		Subscribe(Fetch("http://ericniebler.com/comments/feed/")))

	// Close the subscription after some time
	time.AfterFunc(10*time.Second, func() {
		fmt.Println("closed:", merged.Close())
	})

	// Print the stream
	for it := range merged.Updates() {
		fmt.Println(it.Channel, it.Title)
	}

	panic("show me the stacks")
}
