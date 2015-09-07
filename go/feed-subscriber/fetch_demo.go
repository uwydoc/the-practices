package main

import (
	"fmt"
	"log"
)

func main() {
	fetcher := Fetch("http://ericniebler.com/feed/")
	items, next, err := fetcher.Fetch()
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println(items, next)
}
