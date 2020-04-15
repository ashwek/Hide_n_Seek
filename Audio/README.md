# WAV

[Wav.cpp](https://github.com/ashwek/Hide_n_Seek/blob/master/Audio/Wav.cpp) takes any file as input, and creates an encrypted WAV file from it or vice versa.


----


## Hide Process

[Wav.cpp](https://github.com/ashwek/Hide_n_Seek/blob/master/Audio/Wav.cpp) takes a key _(string)_ and a file as input, creates a wav file of sampling rate 41,000Hz, 2 channels, & 16 bits per channel _(these values are set as default & can be changed through code)_. **Key** is used to seed random generator. The input file is read, encrypted using XOR cipher and written to the WAV file.

This WAV file is similar to any other WAV file and can be played thorugh any media player that supports .wav format, but since the data in this .wav file would be the file hidden in it, the .wav file may produce unplesent noise.

<table>
  <tr>
    <th colspan="2" align="center">Demo</th>
  </tr>
  <tr>
    <th>Input</th>
    <td><a href="https://github.com/ashwek/Hide_n_Seek/blob/master/sample_data/sample_pdf_file.pdf">sample_data / sample_pdf_file.pdf</a></td>
  </tr>
  <tr>
    <th>Output</th>
    <td><a href="https://github.com/ashwek/Hide_n_Seek/blob/master/Audio/demo/Wed%20Apr%2015%2016:43:23%202020.wav">demo / Wed Apr 15 16:43:23 2020.wav</a></td>
  </tr>
  <tr>
    <th>Output wav file - wave diagram</th>
    <td><img src="https://raw.githubusercontent.com/ashwek/Hide_n_Seek/master/Audio/demo/Wed%20Apr%2015%2016%3A43%3A23%202020%20-%20Wave%20diagram.png" height="100" /></td>
  </tr>
</table>
  

----


## Seek Process

**Seek** is the reverse process of **Hide**. After a file is hidden inside a .wav file, the orignal (hidden) file can be retrieved. 

[Wav.cpp](https://github.com/ashwek/Hide_n_Seek/blob/master/Audio/Wav.cpp) takes a key _(string)_ and a .wav file as input. The sampling rate, number of channels, & bits per channel is read from the .wav file. **Key** is used to seed random generator. If the correct key is used, the random sequence generated during the **Hide** process is generated again, & by reapplying the XOR function, the cipher is removed. The orignal (hidden) file is recovered.

<table>
  <tr>
    <th colspan="2" align="center">Demo</th>
  </tr>
  <tr>
    <th>Input</th>
    <td><a href="https://github.com/ashwek/Hide_n_Seek/blob/master/Audio/demo/Wed%20Apr%2015%2016:43:23%202020.wav">demo / Wed Apr 15 16:43:23 2020.wav</a></td>
  </tr>
  <tr>
    <th>Output<br />Correct Key</th>
    <td><a href="https://github.com/ashwek/Hide_n_Seek/blob/master/Audio/demo/sample_pdf_file.pdf">demo / sample_pdf_file.pdf</a></td>
  </tr>
  <tr>
    <th>Output<br />Incorrect Key</th>
    <td><a href="https://github.com/ashwek/Hide_n_Seek/blob/master/Audio/demo/invalid%20file">demo / invalid file</a></td>
  </tr>
</table>

----


### Details

A file with the *.wav* or *.wave* file extension is a *Waveform Audio File* Format. It's a container audio file that stores data in segments. The format uses containers to store audio data, track numbers, sample rate, and bit rate. WAV files are **uncompressed lossless audio** with a maximum file size of 4 GB. <sup>[\[1\]](#1)</sup>

.wav files are ideal for steganography because they are lossless files.

![WAV File Fomat image](http://soundfile.sapp.org/doc/WaveFormat/xwav-sound-format.gif.pagespeed.ic.tIS-Bqb8Y1.png)


----


### References

<a name="1" />[1. What are wav and wave files - howtogeek.com](https://www.howtogeek.com/392504/what-are-wav-and-wave-files-and-how-do-i-open-them)<br />
<a name="2" />[2. Microsoft WAVE soundfile format - soundfile.sapp.org](http://soundfile.sapp.org/doc/WaveFormat/)<br />
<a name="3" />[3. Wav (RIFF) File Format Tutorial - topherlee.com](http://www.topherlee.com/software/pcm-tut-wavformat.html)<br />
