package main

import (
	"github.com/SlyMarbo/rss"
	"time"
)

type Fetcher interface {
	Fetch() (items []Item, next time.Time, err error)
}

type Item struct {
	Channel, Title, GUID string // a subset of RSS feed
}

// fetches Items from domain
func Fetch(domain string) Fetcher {
	return fetch{domain}
}

type fetch struct {
	url string
}

func (f fetch) Fetch() ([]Item, time.Time, error) {
	feed, err := rss.Fetch(f.url)
	if err != nil {
		return nil, time.Now(), err
	}
	items := make([]Item, len(feed.Items))
	for i, it := range feed.Items {
		items[i] = Item{f.url, it.Title, it.ID}
	}
	return items, feed.Refresh, nil
}
