# hw2p3_planets

## main.cpp

### void init()
We just init-ed all textures that we were using in our animation

### void main()
One should not forget to cleanup our texture before returning

### void display()
We separated any drawing in 3 subfunctions for each textures.
Drawing here will depend on `time_s`. Since we do use the real time, we do not
have any problems with some vertical syncing and variation in rendering speed.

### void drawSun()
This is where we draw the sun.
We firstly shift it a bit on the right, then make it rotate with a speed
depending to the time for having a animation, then we scale it with a function
that depends of the time.
The function may look weird to get, but it's simple at the end. Basically, we
just scale our cosine to have all values near 1 that we are then scaling at the
size of our canvas for avoiding it to be too big.

### void drawEarth()
First we roate the earth on itself, then move it to the radius from the sun,
then rotate it again.
All those transformations are time-dependent.
We do use the same cosine trick as before for the radius to create a nice
ellipse.

### void drawMoon()
The moon is basically the same as earth, but with 2 more transformations that
are basically the radius from earth and the rotation around it.

## Quad.h

### void cleanup()
This is the last function we had to touch. We only look for the class members
and free them like what we have seen in the previous examples.
