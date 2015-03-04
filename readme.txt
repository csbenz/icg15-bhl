# hw2p1_spiral

For this homework, we had two differents "flowers" to create, 
the first ("spiral") will be printed if you change the `#define USE_SPIRAL` to `1`
the second ("fermat") will be printed by default.

We had different possibilities to display those image, but we choose the transformations method
because this is what the exercise session told we should do. (We prefere the transformations method more 
than the point to point method even if the last one can be more precise and 'brut')

We created the methods `spiral` and `fermat` that will display the two differents images.

For both images it's the combination of 3 transformations : Rotation, Translation and Scaling, 
in a certain order depending on the image we want. The factors for scaling, translating and rotating were finded at the beginning with calculus
but then approximatively to have something smoother (And nearer to the solution). Ex: `alpha = i * Pi/10` is the angle for the spiral, 
and we can have this angle with observation, but then the translation factor is due to some tries.

Note : We changed the `(TRIANGLE_STRIP, 0, 4)` to `(TRIANGLES, 0, 3)` in `Triangle.h` because it  was more appropriate for displaying triangles,
and added the cleanup that wasn't done yet. 

# hw2p2_checkerboard
## quad_fshader.glsl
### main()
Those are sinus in 3d and you need to compose with x and y values.
You add one and divide by two just to normalize between 0 and 1.
Then, we want 10 *loops*, which means 5 loops of red-green; and a period of a sinus is 2 * M_PI. This is how you get 10 * M_PI at the end.

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
