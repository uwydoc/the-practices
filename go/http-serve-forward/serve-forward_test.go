package main

import (
	"bytes"
	"encoding/json"
	"io/ioutil"
	"net/http"
	"net/http/httptest"
	"os"
	"testing"
	"time"
)

func TestServer(t *testing.T) {
	dummy := httptest.NewServer(http.HandlerFunc(testHandler))
	defer dummy.Close()

	ruleFile := writeRules([]*Rule{
		{Host: "example.com", Forward: dummy.Listener.Addr().String()},
		{Host: "example.org", Serve: "testdata"},
	})
	defer os.Remove(ruleFile)

	s, err := NewServer(ruleFile, time.Hour)
	if err != nil {
		t.Fatal(err)
	}

	var tests = []struct {
		url  string
		code int
		body string
	}{
		{"http://example.com/", 200, "OK"},
		{"http://foo.example.com/", 200, "OK"},
		{"http://example.org/", 200, "welcome to testdata\n"},
		{"http://example.net/", 404, "Not found.\n"},
		{"http://fooexample.com/", 404, "Not found.\n"},
	}

	for _, test := range tests {
		req, _ := http.NewRequest("GET", test.url, nil)
		rw := httptest.NewRecorder()
		rw.Body = new(bytes.Buffer)
		s.ServeHTTP(rw, req)
		if g, w := rw.Code, test.code; g != w {
			t.Errorf("%s: code = %d, want %d", test.url, g, w)
		}
		if g, w := rw.Body.String(), test.body; g != w {
			t.Errorf("%s: body = %q, want %q", test.url, g, w)
		}
	}
}

func testHandler(w http.ResponseWriter, r *http.Request) {
	w.Write([]byte("OK"))
}

func writeRules(rules []*Rule) string {
	f, err := ioutil.TempFile("", "sf-rules")
	if err != nil {
		panic(err)
	}
	defer f.Close()
	err = json.NewEncoder(f).Encode(rules)
	if err != nil {
		panic(err)
	}
	return f.Name()
}
