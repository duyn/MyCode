// Echo2 prints its command-line arguments.
package main

import (
    "fmt"
    "os"
)

func main() {
    i := 1
    for _, arg := range os.Args[i:] {
        fmt.Printf("%d: %s\n", i, arg)
        i++
    }
}
