# Homeostasis

<h4>Bsc project to create a feedback synthesizer</h4>
The synth is based on Karplus-Strong.
Top chain instantiates processors that are to be fed back.
Bottom chain processes the output of the synth.

<h3> Signal Flow </h3>

![Signal Flow Chart](https://i.ibb.co/wS2CxNC/Homeostasis-signal-Fow.jpg)

<h3>NEED TO FIX:</h3>

* CPU spike when multiple processors are instantiated on the feedback path.
* Applying interpolation to the delay-line to provide equal-tempered notes.
* The Distortion's Drive knob needs a more sensative curve.
