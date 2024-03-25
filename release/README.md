# filesearch

Find files quickly on Windows (requires index)

Note: An administrator is required to run the program because it uses the USN Journal API.

``` python
import filesearch_for_win.filesearch as fs
obj = fs.filesearch()
obj.wait(1000) # Wait 1000 milliseconds and return true if indexing is complete
obj.search("kernel") # Search files
obj.result() # Get file list
```

