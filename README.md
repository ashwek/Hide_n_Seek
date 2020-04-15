# Hide n Seek - Cryptography &amp; Steganography

This repository will contain code for cryptography &amp; steganography (sort of).

With traditional steganography, one of the _major issues is the maximum size of data that can be hidden_ inside another file.

Here, instead of hiding some data into another, already existing file, **we create a new file**. And since a new file is created, size of data to be hidden is no more an issue.

See [Audio](Audio/) for more details on how some file can be hidden inside a **.wav** file.

Creating a new audio file from scratch to hide data instead of hiding inside an already existing (genuine) audio file **has some limitations of its own**. Since the audio file dosen't realy have any audio data, but some other file encoded into it, when the audio file is played, **_it may produce some unplesent noise_**, which beats the point of steganography, but this is just a hoby project, not meant to be perfect.
