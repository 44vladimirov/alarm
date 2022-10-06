# [sndio](https://sndio.org) alarm utility

```
alarm [-r count] [-R] [-f] [-t timeout] [sound]
```

* `-r count` repeating alarm count times
* `-R` non-repeating alarm
* `-f` infinite repeating alarm
* `-t timeout` repeating timeout, seconds
* `sound` alarm sound file name

`sndio` library is required. \
Sound file format is expected to be [waveform](https://en.wikipedia.org/wiki/WAV). \
If no sound is specified, `/usr/local/etc/alarm` is tried to play.
