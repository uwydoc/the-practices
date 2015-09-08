package main

import (
	"time"
)

type Subscription interface {
	Updates() <-chan Item // stream of Items
	Close() error         // shutdown the stream
}

// converts Fetcher to a stream
func Subscribe(fetcher Fetcher) Subscription {
	sub := &fetcherSub{fetcher, make(chan Item), make(chan chan error)}
	go sub.loop()
	return sub
}

// merge sveral streams
func Merge(subs ...Subscription) Subscription {
	merge := &mergeSub{subs, make(chan Item), make(chan chan error, len(subs))}
	go merge.loop()
	return merge
}

///
type fetcherSub struct {
	fetcher Fetcher
	updates chan Item
	closing chan chan error
}

func (sub *fetcherSub) Updates() <-chan Item {
	return sub.updates
}

func (sub *fetcherSub) Close() error {
	errc := make(chan error)
	sub.closing <- errc
	return <-errc
}

func (sub *fetcherSub) loop() {
	const maxPending = 20
	var pending []Item
	seen := make(map[string]bool)
	var next time.Time
	var err error
	var fetchDone chan fetchResult
	for {
		var first Item
		var updates chan Item
		if len(pending) > 0 {
			first = pending[0]
			updates = sub.updates
		}

		var fetchDelay time.Duration
		if now := time.Now(); next.After(now) {
			fetchDelay = next.Sub(now)
		}

		var startFetch <-chan time.Time
		if fetchDone == nil && len(pending) < maxPending {
			startFetch = time.After(fetchDelay)
		}

		select {
		case errc := <-sub.closing:
			errc <- err
			close(sub.updates)
			return
		case <-startFetch:
			fetchDone = make(chan fetchResult, 1)
			go func() {
				items, next, err := sub.fetcher.Fetch()
				fetchDone <- fetchResult{items, next, err}
			}()
		case result := <-fetchDone:
			fetchDone = nil
			if result.err != nil {
				next = time.Now().Add(10 * time.Second)
				break
			}
			next = result.next
			for _, item := range result.items {
				if !seen[item.GUID] {
					pending = append(pending, item)
					seen[item.GUID] = true
				}
			}
		case updates <- first:
			pending = pending[1:]
		}
	}
}

type fetchResult struct {
	items []Item
	next  time.Time
	err   error
}

///
type mergeSub struct {
	subs    []Subscription
	updates chan Item
	closing chan chan error
}

func (m *mergeSub) Updates() <-chan Item {
	return m.updates
}

func (m *mergeSub) Close() error {
	var errcs []chan error
	for range m.subs {
		errc := make(chan error, 1)
		errcs = append(errcs, errc)
		m.closing <- errc
	}
	var errs []error
	for _, errc := range errcs {
		err := <-errc
		errs = append(errs, err)
	}
	close(m.updates)
	return nil //TODO merge |errs| and return them
}

func (m *mergeSub) loop() {
	for _, sub := range m.subs {
		go func(sub Subscription) {
			var updates <-chan Item
			updates = sub.Updates()
			for {
				select {
				case item := <-updates:
					m.updates <- item
				case errc := <-m.closing:
					err := sub.Close()
					errc <- err
					return
				}
			}
			for item := range sub.Updates() {
				m.updates <- item
			}
		}(sub)
	}
}
