// print-filepath.go
//
// Output the first row of the table |videoclip| and |image| in the |cameravideo| db
//
package main

import (
	"database/sql"
	"flag"
	_ "github.com/mattn/go-sqlite3"
	"log"
)

func main() {
	dbfile := flag.String("dbfile", "cameravideo.db", "Path of the cameravideo.db")
	flag.Parse()

	db, err := sql.Open("sqlite3", *dbfile)
	if err != nil {
		log.Fatal("failed to open db:", err.Error())
	}
	defer db.Close()

	row := db.QueryRow("SELECT thumbnailpath FROM videoclip LIMIT 1")
	var filepath string
	if err := row.Scan(&filepath); err != nil {
		log.Fatal("query failed: ", err.Error())
	}
	log.Printf("video filepath: %v\n", filepath)

	row = db.QueryRow("SELECT filepath FROM image LIMIT 1")
	if err := row.Scan(&filepath); err != nil {
		log.Fatal("query failed: ", err.Error())
	}
	log.Printf("image filepath: %v\n", filepath)
}
