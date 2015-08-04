// fix-db-filepath.go
package main

import (
	"database/sql"
	"flag"
	_ "github.com/mattn/go-sqlite3"
	shutil "github.com/termie/go-shutil"
	"log"
	"os"
	"path"
	"path/filepath"
)

var (
	runtimedir                       string
	videodir, imagedir               string
	videodirpattern, imagedirpattern string
	origdbfile, dbfile               string
)

const (
	defaultvideodirpattern = `E:\视频验收数据`
	defaultimagedirpattern = `E:\卡口数据\2015年1月22日\卡口机动车`
)

func parseConfig() {
	pwd, _ := os.Getwd()
	flag.StringVar(&runtimedir, "runtimedir", "runtime-x64", "Runtime directory, where launcher.exe is")
	flag.StringVar(&videodir, "videodir", pwd, "Parent directory of videos")
	flag.StringVar(&videodirpattern, "videodirpattern", defaultvideodirpattern, "Path pattern to be replaced for video filepath and thumbnail path")
	flag.StringVar(&imagedir, "imagedir", "", "Parent directory of images")
	flag.StringVar(&imagedirpattern, "imagedirpattern", defaultimagedirpattern, "Path pattern to be replaced for image filepath")
	flag.Parse()
	// get absolute path
	runtimedir, _ = filepath.Abs(runtimedir)
	videodir, _ = filepath.Abs(videodir)
	if len(imagedir) == 0 {
		imagedir = videodir
	} else {
		imagedir, _ = filepath.Abs(imagedir)
	}
	if len(videodirpattern) == 0 || len(imagedirpattern) == 0 {
		log.Fatal("pattern must not be empty")
	}
	// update |origdbfile| and |dbfile| according to |runtimedir|
	origdbfile = path.Join(runtimedir, "data", "cameravideo.db.orig")
	dbfile = path.Join(runtimedir, "data", "cameravideo.db")
}

func restoreDBFile() {
	if _, err := os.Stat(dbfile); err == nil {
		os.Remove(dbfile)
	}
	if _, err := os.Stat(origdbfile); err != nil {
		log.Fatal("orig dbfile not found:", origdbfile)
	}
	err := shutil.CopyFile(origdbfile, dbfile, true)
	if err != nil {
		log.Fatal("failed to restore db file:", err.Error())
	}
}

// Sql to update filepath(video, image, thumbnail) in db(video, videoclip, image)
const (
	fixvideosql = `
update video set filepath = replace(filepath, $1, $2), thumbnailpath = replace(thumbnailpath, $1, $2) where instr(filepath, $1) = 1;
`
	fixvideoclipsql = `
update videoclip set thumbnailpath = replace(thumbnailpath, $1, $2) where instr(thumbnailpath, $1) = 1;
`
	fiximagesql = `
update image set filepath = replace(filepath, $1, $2) where instr(filepath, $1) = 1;
`
)

func updateDB() {
	db, err := sql.Open("sqlite3", dbfile)
	if err != nil {
		log.Fatal("failed to open db:", err.Error())
	}
	defer db.Close()

	txn, _ := db.Begin()
	execAndCheck := func(query string, args ...interface{}) {
		stmt, err := txn.Prepare(query)
		if err != nil {
			log.Fatal("failed to prepare query:", err.Error())
		}
		defer stmt.Close()
		if _, err = stmt.Exec(args...); err != nil {
			log.Fatal("failed to perform the requested update:", err.Error())
		}
	}
	execAndCheck(fixvideosql, videodirpattern, videodir)
	execAndCheck(fixvideoclipsql, videodirpattern, videodir)
	execAndCheck(fiximagesql, imagedirpattern, imagedir)
	if err := txn.Commit(); err != nil {
		log.Fatal("failed to commit the transaction:", err.Error())
	}
}

func main() {
	parseConfig()
	log.Println("[info] restore db file")
	restoreDBFile()
	log.Println("[info] perform db update")
	updateDB()
	log.Println("[info] done")
}
