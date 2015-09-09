// urlpoll.go
package main

import (
	"log"
	"net/http"
	"time"
)

// constants
const (
	numPollers     = 2                // number of Poller goroutines to launch
	pollInterval   = 20 * time.Second // how often to poll each url
	statusInterval = 10 * time.Second // how often to log status to stdout
	errTimeout     = 10 * time.Second // back-off timeout on error
)

var urls = []string{
	"http://www.google.com/",
	"http://golang.org/",
	"http://www.baidu.com/",
}

type State struct {
	url, status string
}

func StateMonitor(du time.Duration) chan<- State {
	updates := make(chan State)
	status := make(map[string]string)
	ticker := time.NewTicker(du)
	go func() {
		for {
			select {
			case <-ticker.C:
				logState(status)
			case s := <-updates:
				status[s.url] = s.status
			}
		}
	}()
	return updates
}

func logState(status map[string]string) {
	log.Println("Current state:")
	for k, v := range status {
		log.Printf("%s %s", k, v)
	}
}

type Resource struct {
	url      string
	errCount int
}

func (r *Resource) Poll() string {
	resp, err := http.Head(r.url)
	if err != nil {
		log.Println("Error", r.url, err)
		r.errCount++
		return err.Error()
	}
	r.errCount = 0
	return resp.Status
}

func (r *Resource) Sleep(done chan<- *Resource) {
	time.Sleep(pollInterval + errTimeout*time.Duration(r.errCount))
	done <- r
}

func Poller(in <-chan *Resource, out chan<- *Resource, status chan<- State) {
	for r := range in {
		s := r.Poll()
		status <- State{r.url, s}
		out <- r
	}
}

func main() {
	pending, complete := make(chan *Resource), make(chan *Resource)
	// launch the statemonitor
	status := StateMonitor(statusInterval)
	// launch some Poller goroutines
	for i := 0; i < numPollers; i++ {
		go Poller(pending, complete, status)
	}
	// send some resources to the pending queue
	go func() {
		for _, url := range urls {
			pending <- &Resource{url: url}
		}
	}()
	go func() {
		for r := range complete {
			go r.Sleep(pending)
		}
	}()
	time.Sleep(3 * pollInterval)
}
