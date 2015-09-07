package main

import (
	"log"
	"time"
)

type Subscription interface {
	Updates() <-chan Item // stream of Items
	Close() error         // shutdown the stream
}

// converts Fetcher to a stream
func Subscribe(fetcher Fetcher) Subscription {
	sub := subscription{make(chan Item), false}
	go func() {
		for !sub.closed {
			items, next, err := fetcher.Fetch()
			if err != nil {
				log.Fatal(err)
				return
			}
			for _, item := range items {
				sub.ch <- item
			}
			now := time.Now()
			if next.After(now) {
				time.Sleep(next.Sub(now))
			} else {
				time.Sleep(10 * time.Second)
			}
		}
	}()
	return sub
}

// merge sveral streams
func Merge(subs ...Subscription) Subscription {
	merge := subscription{make(chan Item), false}
	for _, sub := range subs {
		go func(s Subscription) {
			for item := range s.Updates() {
				merge.ch <- item
			}
		}(sub)
	}
	return merge
}

///
type subscription struct {
	ch     chan Item
	closed bool
}

func (sub subscription) Updates() <-chan Item {
	return sub.ch
}

func (sub subscription) Close() error {
	sub.closed = false
	close(sub.ch)
	return nil
}
